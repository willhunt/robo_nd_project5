#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <vector>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "simple_navigation_goals");

    //tell the action client that we want to spin a thread by default
    MoveBaseClient ac("move_base", true);

    //wait for the action server to come up
    while (!ac.waitForServer(ros::Duration(5.0)))
    {
        ROS_INFO("Waiting for the move_base action server to come up");
    }

    move_base_msgs::MoveBaseGoal goal;

    // Create list of goals
    std::vector< std::vector<double> > goal_list = {
        {1.0, 1.0, 1.0},
        {2.0, 1.0, 1.0}
    };
    for (int i = 0; i < goal_list.size(); i++) {
        //we'll send a goal to the robot to move 1 meter forward
        goal.target_pose.header.frame_id = "map";
        goal.target_pose.header.stamp = ros::Time::now();

        goal.target_pose.pose.position.x = goal_list[i][0];
        goal.target_pose.pose.position.y = goal_list[i][1];
        goal.target_pose.pose.orientation.w = goal_list[i][2];

        ROS_INFO("Sending goal");
        ac.sendGoal(goal);

        ac.waitForResult();

        if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
            ROS_INFO("Hooray, the base reached goal %d", i+1);
        else
            ROS_INFO("The base failed to reach goal %d", i+1);
    }
    
    return 0;
}