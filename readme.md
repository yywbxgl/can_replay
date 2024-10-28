


# build
```bash
source /opt/ros/galactic/setup.bash
colcon build
```


# Run
```bash
# source environment
source /opt/ros/galactic/setup.bash
source ./install/setup.bash

# rosbag replay
ros2 bag play ./test/AB_rosbag_2024_10_09_16_28_36_1627/

# can rewrite, default use can0
ros2 run ab_can_replay ab_can_replay
```