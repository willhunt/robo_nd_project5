# Project 5, Home Service Robot
## Udacity Robotics Software Engineer Nanodegree
Home Service Robot project for the Robotics Software Engineer Nanodegree program from Udacity.

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

#### After building the package, source your environment
```sh
$ source devel/setup.bash
```

#### Once the `robo_nd_project5` package has been built, you can launch using:
```sh
$ roslaunch myrobot_main main.launch
```