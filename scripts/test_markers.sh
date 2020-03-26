#!/bin/sh

gnome-terminal \
--tab -e "sh -c 'roscore; sh'" \
--tab -e "sh -c 'sleep 2; source devel/setup.bash; roslaunch myrobot_main main.launch; sh'" \

gnome-terminal --tab -e "sh -c 'rosrun myrobot_markers add_markers_quiz; sh'"
