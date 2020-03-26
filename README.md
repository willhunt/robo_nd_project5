# Project 5, Home Service Robot
## Udacity Robotics Software Engineer Nanodegree
Home Service Robot project for the Robotics Software Engineer Nanodegree program from Udacity.

The robot navigates to randomly selected goals to pick up objects, respresented as markers in Rviz. The markers are collected and returned to the goal area, placed in a spiral grid pattern.
The ROS navigation stack is used to operate the robot within it's environment. First a map was created using SLAM in the gmapping package. The map was then loaded using the map server for the home service function. AMCL localises the robot within the environment and move_base is used to navigate towards goals. The myrobot_markers node controls the marker visualization based upon robot position by subscribing to the /amcl_pose topic. The myrobot_pick node accepts goal commands via service calls from the myrobot_markers node and submits them to the move base client.

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

### Main Program
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
### Additional Quiz's
During the project certain functionality was required as building block for the project
#### Add Markers Quiz
```sh
$ ./scripts/test_markers.sh
```
#### Add Markers Quiz
```sh
$ ./scripts/test_markers.sh
```
## Pickup Quiz
```sh
$ ./scripts/test_pick.sh
```

## Evidence
2 pick's complete | 4 pick's complete | 7 pick's complete  
:----------------:|:-----------------:|:-----------------:
<img src="/media/screenshot1.png" alt="" width="auto" height="250">  |  <img src="/media/screenshot2.png" alt="world image" width="auto" height="250"> |  <img src="/media/screenshot3.png" alt="world image" width="auto" height="250">