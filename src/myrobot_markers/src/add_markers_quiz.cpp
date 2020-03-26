#include "ros/ros.h"
#include <visualization_msgs/Marker.h>

class MarkerManager {
    private:
    ros::Publisher marker_publisher_;

    public:
    MarkerManager(ros::NodeHandle *n) {
        // Inform ROS master that we will be publishing a message of type visualization_msgs::Marker
        marker_publisher_ = n->advertise<visualization_msgs::Marker>("visualization_marker", 1);
    }

    int getNumSubscribers() {
        return marker_publisher_.getNumSubscribers();
    }

    void add_marker(int id, double x, double y) {
        // Marker message
        visualization_msgs::Marker marker_msg;
        // Set the frame ID and timestamp.  See the TF tutorials for information on these.
        marker_msg.header.frame_id = "/map";
        marker_msg.header.stamp = ros::Time::now();
        // Set the namespace and id for this marker.  This serves to create a unique ID
        // Any marker sent with the same namespace and id will overwrite the old one
        marker_msg.ns = "target_shapes";
        marker_msg.id = id;
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
        marker_msg.pose.orientation.x = 0.0;
        marker_msg.pose.orientation.y = 0.0;
        marker_msg.pose.orientation.z = 0.0;
        marker_msg.pose.orientation.w = 1.0;
        // Set the color -- be sure to set alpha to something non-zero!
        marker_msg.color.r = 0.0f;
        marker_msg.color.g = 1.0f;
        marker_msg.color.b = 0.0f;
        marker_msg.color.a = 1.0;

        marker_msg.lifetime = ros::Duration();

        // Publish
        marker_publisher_.publish(marker_msg);
    }

    void remove_marker(int id) {
        // Marker message
        visualization_msgs::Marker marker_msg;
        // Set the frame ID and timestamp.  See the TF tutorials for information on these.
        marker_msg.header.frame_id = "/map";
        marker_msg.header.stamp = ros::Time::now();
        marker_msg.ns = "target_shapes";
        marker_msg.id = id;
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

    // Don't need this unles we add a subrcriber
    // ros::spin();

    // ros::Rate loop_rate(0.05);  // Hz
    while (ros::ok()) {
        // Publish the marker
        while (marker_manager.getNumSubscribers() < 1)
        {
            if (!ros::ok())
            {
                return 0;
            }
            ROS_WARN_ONCE("Please create a subscriber to the marker");
            sleep(1);
        }
        
        int marker_id = 0;
        // Start marker
        ROS_INFO("Publishing marker at start...");
        marker_manager.add_marker(marker_id, 5.0, 1.0);
        ros::Duration(5).sleep();  // Sleep time in seconds
        marker_manager.remove_marker(marker_id);
        ros::Duration(5).sleep();  // Sleep time in seconds
        // Goal marker
        ROS_INFO("Publishing marker at goal...");
        marker_manager.add_marker(marker_id, -2.8, -1.2);
        ros::Duration(5).sleep();  // Sleep time in seconds
        marker_manager.remove_marker(marker_id);
        ros::Duration(5).sleep();  // Sleep time in seconds

        // loop_rate.sleep();
    }
    return 0;
}