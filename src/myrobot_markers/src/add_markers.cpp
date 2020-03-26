#include "ros/ros.h"
#include <tf/tf.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include "myrobot_pick/DriveToGoal.h"
#include <vector>
#include <random>
#include <cmath>

#define M_PI    3.14159265358979323846  /* pi */

// Random Generators
std::random_device rd;
std::mt19937 gen( rd() );

// Iterator to create spiral coordinates in 2D grid for placing goal boxes
// https://stackoverflow.com/questions/3706219/algorithm-for-iterating-over-an-outward-spiral-on-a-discrete-2d-grid-from-the-or
class SpiralOut{
protected:
    unsigned layer;
    unsigned leg;
public:
    int x, y; //read these as output from next, do not modify.
    SpiralOut():layer(1),leg(0),x(0),y(0){}
    void goNext(){
        switch(leg){
        case 0: ++x; if(x  == layer)  ++leg;                break;
        case 1: ++y; if(y  == layer)  ++leg;                break;
        case 2: --x; if(-x == layer)  ++leg;                break;
        case 3: --y; if(-y == layer){ leg = 0; ++layer; }   break;
        }
    }
};

class MarkerManager {
    private:
    // Publish marker vizualisation to rviz
    ros::Publisher marker_publisher_;
    // Subscribe to robot pose information from AMCL
    ros::Subscriber robot_pose_subscriber_;
    // Client capable of requesting services from myrobot_pick
    ros::ServiceClient pick_client_;
    // Record marker state 0 = not started, 1 = at start, 2 = pickup, 3 = being carried, 4 = drop off, 5 = at goal
    int marker_state_ = 0;
    // Current pick
    int current_pick_id_;
    // goal spiral iterator
    SpiralOut spiral_;
    // Spacing between box placement at goal
    double box_spacing_ = 0.6;

    public:
    double central_end_goal_[3] = {-1.527110, -0.289609, 2*M_PI};
    double end_goal_[3];
    double start_goal_[3];


    MarkerManager(ros::NodeHandle *n) {
        // Inform ROS master that we will be publishing a message of type visualization_msgs::Marker
        marker_publisher_ = n->advertise<visualization_msgs::Marker>("visualization_marker", 1);

        robot_pose_subscriber_ = n->subscribe("/amcl_pose", 10, 
            &MarkerManager::robot_pose_callback, this);

        pick_client_ = n->serviceClient<myrobot_pick::DriveToGoal>("/myrobot_pick/goal");
    }

    // Send request to vaigate robot towards goal
    void send_pick_request(double goal[3]) {
            myrobot_pick::DriveToGoal srv;
            srv.request.x = goal[0];
            srv.request.y = goal[1];
            srv.request.theta = goal[2];
            if (!pick_client_.call(srv))
                ROS_ERROR("Failed to call service pick request");
    }
    
    void robot_pose_callback(const geometry_msgs::PoseWithCovarianceStamped pose_msg) {
        // double q[] = {pose_msg.pose.orientation.x, pose_msg.pose.orientation.y, pose_msg.pose.orientation.z, pose_msg.pose.orientation.w};
        tf::Quaternion q = tf::Quaternion(
            pose_msg.pose.pose.orientation.x,
            pose_msg.pose.pose.orientation.y,
            pose_msg.pose.pose.orientation.z,
            pose_msg.pose.pose.orientation.w);
        tf::Matrix3x3 R = tf::Matrix3x3(q);  // Rotation matrix
        double roll, pitch, yaw;
        R.getRPY(roll, pitch, yaw);

        double robot_pose[3] = {pose_msg.pose.pose.position.x, pose_msg.pose.pose.position.y, yaw};
        std::cout << "Pose: [" << pose_msg.pose.pose.position.x <<", " << pose_msg.pose.pose.position.y << ", " << yaw << "]" << std::endl;

        if (marker_state_ == 1) { // If marker at start
            // Check if robot at start position
            if ( position_close(robot_pose, start_goal_) ) {
                marker_state_ = 2;  // Pickup
                ROS_INFO("Picking up...");
                // Sleep to simulate pickup
                ros::Duration(5).sleep();  // Sleep time in seconds
                remove_marker();
                marker_state_ = 3;  // Being carried
                // Send goal to pick package
                end_goal_[0] = central_end_goal_[0] + spiral_.x * box_spacing_;
                end_goal_[1] = central_end_goal_[1] + spiral_.y * box_spacing_;
                end_goal_[2] = 2 * M_PI;
                spiral_.goNext();
                send_pick_request(end_goal_);
            }
        } else if (marker_state_ == 3) {  // Being carried
            // Check if at goal position
            if ( position_close(robot_pose, end_goal_) ) {
                marker_state_ = 2;  // Drop-off
                ROS_INFO("Dropping off...");
                // Sleep to simulate drop-off
                ros::Duration(5).sleep();  // Sleep time in seconds
                marker_state_ = 5;  // At goal
                add_marker(end_goal_[0], end_goal_[1], end_goal_[2]);
                marker_state_ = 0;  // Not started
                current_pick_id_ += 1;
                add_random_marker();
                marker_state_ = 1;  // Marker at start
                // Send goal to pick package
                send_pick_request(start_goal_);

            } else {
                // Show indicator that robot is carrying marker
                ROS_INFO("Carrying marker");
            }
        }
    }

    void init_marker() {
        // Start marker
        ROS_INFO("Publishing first marker at start...");
        current_pick_id_ = 0;
        add_random_marker();
        marker_state_ = 1;  // Marker at start
        // Send goal to pick package
        send_pick_request(start_goal_);
    }

    // Normalise angles between 0 and 2*pi
    double normalise_angle(double theta) {
        double theta_norm = std::fmod(theta, 2 * M_PI);
        if (theta_norm < 0) {
            theta_norm += 2 * M_PI;
        }
        return theta_norm;
    }

    // Decided if 2 poses are close to each other in terms of x, y and theta
    // Inputs
    //      pose1: [x(m), y(m), theta(rad)]
    //      pose2: [x(m), y(m), theta(rad)]
    bool position_close(double pose1[3], double pose2[3]) {
        double dist_tolerance = 0.2;
        double angle_tolerance = 1.0;
        double theta1 = normalise_angle(pose1[2]);
        double theta2 = normalise_angle(pose2[2]);

        if (abs(pose1[0] - pose2[0]) < dist_tolerance &&
                abs(pose1[1] - pose2[1]) < dist_tolerance &&
                (M_PI - abs(abs(theta1 - theta2) - M_PI)) < angle_tolerance) {
            return true;
        } else {
            return false;
        }
    }

    int getNumSubscribers() {
        return marker_publisher_.getNumSubscribers();
    }

    void add_marker(double x, double y, double theta) {
        // Marker message
        visualization_msgs::Marker marker_msg;
        // Set the frame ID and timestamp.  See the TF tutorials for information on these.
        marker_msg.header.frame_id = "/map";
        marker_msg.header.stamp = ros::Time::now();
        // Set the namespace and id for this marker.  This serves to create a unique ID
        // Any marker sent with the same namespace and id will overwrite the old one
        marker_msg.ns = "target_shapes";
        marker_msg.id = current_pick_id_;
        // Set the marker type. 
        marker_msg.type = visualization_msgs::Marker::CUBE;
        // Set the marker action.  Options are ADD, DELETE, DELETEALL
        marker_msg.action = visualization_msgs::Marker::ADD;
        // Set the scale of the marker -- 1x1x1 here means 1m on a side
        marker_msg.scale.x = 0.25;
        marker_msg.scale.y = 0.25;
        marker_msg.scale.z = 0.25;
        // Set the pose of the marker_msg.  This is a full 6DOF pose relative to the frame/time specified in the header
        marker_msg.pose.position.x = x;
        marker_msg.pose.position.y = y;
        marker_msg.pose.position.z = marker_msg.scale.z / 2;
        // double q[] = tf::quaternion_from_euler(0, 0, theta);
        tf::Quaternion q = tf::Quaternion();
        q.setRPY(0, 0, theta);
        marker_msg.pose.orientation.x = q[0];
        marker_msg.pose.orientation.y = q[1];
        marker_msg.pose.orientation.z = q[2];
        marker_msg.pose.orientation.w = q[3];
        // Set the color -- be sure to set alpha to something non-zero!
        marker_msg.color.r = 0.0f;
        marker_msg.color.g = 1.0f;
        marker_msg.color.b = 0.0f;
        marker_msg.color.a = 1.0;

        marker_msg.lifetime = ros::Duration();

        // Publish
        marker_publisher_.publish(marker_msg);
    }

    void add_random_marker() {
        // Define list of possible goals for simplicity
        std::vector<std::vector<double>> goal_list = {
            // {x-position, y-position, orientation(rad)}
            {-2.0, -1.78, -2.39},
            {2.31, -4.23, -2.39},
            {5.67, -4.68, -1.55},
            {5.51, -1.43, 1.22},
            {6.04, 1.72, 0.98}
        };
        // Select goal randomly
        std::uniform_int_distribution<int> int_dist( 0, goal_list.size()-1 );
        int random_index = int_dist(gen);
        ROS_INFO("Random index: %d", random_index);
        std::vector<double> goal = goal_list[ random_index ];
        // Save to object member variable
        start_goal_[0] = goal[0];    start_goal_[1] = goal[1];    start_goal_[2] = goal[2];
        add_marker(goal[0], goal[1], goal[2]);
    }

    void remove_marker() {
        // Marker message
        visualization_msgs::Marker marker_msg;
        // Set the frame ID and timestamp.  See the TF tutorials for information on these.
        marker_msg.header.frame_id = "/map";
        marker_msg.header.stamp = ros::Time::now();
        marker_msg.ns = "target_shapes";
        marker_msg.id = current_pick_id_;
        marker_msg.action = visualization_msgs::Marker::DELETE;
        // Publish
        marker_publisher_.publish(marker_msg);
    }
};

int main(int argc, char** argv) {
    // Initialize a ROS node
    ros::init(argc, argv, "add_markers");
    // Create a ROS NodeHandle object
    ros::NodeHandle n;
    // Instantiate RobotDrive object
    MarkerManager marker_manager = MarkerManager(&n);

    // Publish the marker
    while (marker_manager.getNumSubscribers() < 1)
    {
        if (!ros::ok()) {
            return 0;
        }
        ROS_WARN_ONCE("Please create a subscriber to the marker");
        sleep(1);
    }

    marker_manager.init_marker();

    ros::spin();

}