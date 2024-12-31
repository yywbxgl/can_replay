# Description
This package is used to rewrite can messages from AutoBrain's rosbag file.  
TopicName:  hardware_interface/udp_signals     
DataType: ab_udp_msg::msg::ABSignalsV1

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

# can rewrite.  can0 is used by default
ros2 run ab_can_replay ab_can_replay_ev6

# can rewrite with specified can interface
ros2 run ab_can_replay ab_can_replay_ev6  can2
```



# CAN Tools
```bash
python3 -m pip install cantools
python3 -m cantools generate_c_source ./mcu_233.dbc
python3 -m cantools generate_c_source ./ev6.dbc
```