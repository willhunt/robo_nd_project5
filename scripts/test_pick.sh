#!/bin/sh

gnome-terminal --tab -e "sh -c 'source devel/setup.bash; roslaunch myrobot_main main.launch'"
gnome-terminal --tab -e "sh -c 'rosrun myrobot_pick pick_objects; sh'"
