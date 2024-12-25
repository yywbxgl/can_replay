# Description
This package is used to replay can messages from AutoBrain's rosbag file. 
The can interface can be specified as an argument to the node. If no can interface is specified, the node will default to can0.


# Build
```bash
source /opt/ros/humble/setup.bash
colcon build
```

# Run
```bash
# source environment
source ./install/setup.bash

# enable can interface
sudo ip link set can0 up type can bitrate 500000 loopback off

# run rosbag replay
ros2 bag play ./test/AB_rosbag_2024_10_09_16_28_36_1627/

# run can replay, use can0
ros2 run ab_can_replay ab_can_replay

# run can replay, use can2
ros2 run ab_can_replay ab_can_replay  can2
```