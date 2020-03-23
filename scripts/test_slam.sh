#!/bin/sh

gnome-terminal --tab -e "sh -c 'source devel/setup.bash; roslaunch myrobot_main main.launch'"
gnome-terminal --tab -e "sh -c 'rosrun teleop_twist_keyboard teleop_twist_keyboard.py cmd_vel:=/myrobot_diff_drive_controller/cmd_vel; sh'"
