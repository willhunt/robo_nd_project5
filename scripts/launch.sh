#!/bin/sh
# gnome-terminal  -e  " gazebo " &
# sleep 5
# gnome-terminal  -e  " roscore" & 
# sleep 5
# gnome-terminal  -e  " rosrun rviz rviz" 

gnome-terminal --tab -e " gazebo" --tab -e "sh -c 'sleep 3; roscore'" --tab -e "sh -c 'sleep 3; rosrun rviz rviz'"
