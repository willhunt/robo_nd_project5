#include "ros/ros.h"
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include "myrobot_pick/DriveToGoal.h"
#include <stdio.h>
#include <tf/tf.h>


typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

class RobotPick {
    private:
    ros::ServiceServer pick_service_;

    public:
    RobotPick(ros::NodeHandle *n) {
        // Service server to respond to service (from client) with  callback function
        pick_service_ = n->advertiseService("/myrobot_pick/goal", &RobotPick::handle_pick_request, this);
        ROS_INFO("Ready to receive pick requests");

    }

    // Callback function that executes whenever a pick_service_ service is requested
    bool handle_pick_request(myrobot_pick::DriveToGoal::Request& req, myrobot_pick::DriveToGoal::Response& res) {

        ROS_INFO("DriveToGoal received - x:%1.2f, y:%1.2f, theta:%1.2f", (float)req.x, (float)req.y, (float)req.theta);

        // Tell the action client that we want to spin a thread by default
        MoveBaseClient mbc("move_base", true);
        //wait for the action server to come up
        while (!mbc.waitForServer(ros::Duration(5.0)))
        {
            ROS_INFO("Waiting for the move_base action server to come up");
        }

        move_base_msgs::MoveBaseGoal goal_msg;
        //we'll send a goal to the robot to move 1 meter forward
        goal_msg.target_pose.header.frame_id = "map";
        goal_msg.target_pose.header.stamp = ros::Time::now();

        goal_msg.target_pose.pose.position.x = req.x;
        goal_msg.target_pose.pose.position.y = req.y;
        tf::Quaternion q;
        q.setRPY(0, 0, req.theta);
        goal_msg.target_pose.pose.orientation.x = q[0];
        goal_msg.target_pose.pose.orientation.y = q[1];
        goal_msg.target_pose.pose.orientation.z = q[2];
        goal_msg.target_pose.pose.orientation.w = q[3];

        ROS_INFO("Sending goal");

        
        mbc.sendGoal(goal_msg);
        mbc.waitForResult();

        if (mbc.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
            ROS_INFO("Hooray, the robot reached goal");
        else
            ROS_INFO("The robot failed to reach goal");

        // res.msg_feedback = std::sprintf( "Goal set to: x:%1.2f, y:%1.2f, theta:%1.2f", (float)req.x, (float)req.y, (float)req.theta )
        res.msg_feedback = "Goal set.";
        // ROS_INFO_STREAM(res.msg_feedback);

        return true;
    }
};

int main(int argc, char** argv) {
    // Initialize a ROS node
    ros::init(argc, argv, "pick_goal");
    // Create a ROS NodeHandle object
    ros::NodeHandle n;
    // Instantiate RobotPick object
    RobotPick robot_pick = RobotPick(&n);

    ros::spin();
}