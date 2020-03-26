# Project 5, Home Service Robot
## Udacity Robotics Software Engineer Nanodegree
Home Service Robot project for the Robotics Software Engineer Nanodegree program from Udacity.

The robot navigates to randomly selected goals to pick up objects, respresented as markers in Rviz. The markers are collected and returned to the goal area, placed in a spiral grid pattern.
The ROS navigation stack is used (amcl, move_base) to operate the robot within it's environment. 

## How to Launch the simulation?
#### Clone the package
```sh
$ cd /home/<project folder>/ 
$ git clone https://github.com/willhunt/robo_nd_project5.git
```

#### Build the `robo_nd_project5` package
```sh
$ catkin_make
```

#### Make sure to check and install any missing dependencies
```sh
$ rosdep install --from-paths src --ignore-src -r -y
```

#### Launching from script
```sh
$ ./scripts/home_service.sh
```

#### Launching nodes manually:
In 3 seperate terminals/tabs run:
```sh
$ source devel/setup.bash
$ roslaunch myrobot_main main.launch
```
```sh
$ source devel/setup.bash
$ roslaunch myrobot_pick main.launch
```
```sh
$ source devel/setup.bash
$ roslaunch myrobot_markers markers.launch
```

## Evidence
            |  
:-------------------------:|:-------------------------:
<img src="/media/screenshot1.png" alt="" width="auto" height="250">  |  <img src="/media/screenshot2.png" alt="world image" width="auto" height="250"> |  <img src="/media/screenshot3.png" alt="world image" width="auto" height="250">