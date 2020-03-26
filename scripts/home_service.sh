#!/bin/sh

gnome-terminal \
--tab -e "sh -c 'roscore; sh'" \
--tab -e "sh -c 'sleep 2; source devel/setup.bash; roslaunch myrobot_main main.launch; sh'" \
--tab -e "sh -c 'sleep 2; roslaunch myrobot_markers markers.launch; sh'" \
--tab -e "sh -c 'sleep 2; roslaunch myrobot_pick pick.launch; sh'"
