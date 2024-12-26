#include <cstdio>
#include <rclcpp/rclcpp.hpp>
#include <ab_udp_msg/msg/ab_signals_v1.hpp>
#include <cmath>

#include "socket_can.h"
#include "can_convert.h"


class ABCanReplay: public rclcpp::Node
{
public:
  ABCanReplay(std::string can_interface): Node("ab_can_replay")
  {
    sub_can_frame_ = this->create_subscription<ab_udp_msg::msg::ABSignalsV1>(
      "hardware_interface/udp_signals", 10, std::bind(&ABCanReplay::can_frame_callback, this, std::placeholders::_1)
    );

    if (can_.init(can_interface) == false) {
      exit(1);
    }
  }

private:
  rclcpp::Subscription<ab_udp_msg::msg::ABSignalsV1>::SharedPtr sub_can_frame_;
  SocketCan can_;

  void can_frame_callback(const ab_udp_msg::msg::ABSignalsV1::SharedPtr msg)
  {
    printf("recv vehicle_data\n");
    printf("\ttime stamp: %d %d\n", msg->header.stamp.sec, msg->header.stamp.nanosec);
    printf("\tlongitudinal_velocity: %f\n", msg->vehicle_signals.longitudinal_velocity);  // 车速-speed
    printf("\tlongitudinal_acceleration: %f\n", msg->vehicle_signals.longitudinal_acceleration);  // 纵向加速
    printf("\tyaw_rate: %f\n", msg->vehicle_signals.yaw_rate);   
    printf("\tspeed_o_speed: %f\n", msg->vehicle_signals.speed_o_speed);  // 没有使用


    // rewrite the can frame  esp_0x109
    {
      struct esp_0x109_t esp_109;
      // PS 方向相反，msg中左正右负，单位rad/sec,  CAN中右正左负，单位deg/sec
      esp_109.longitudinal_acceleration =  - msg->vehicle_signals.longitudinal_acceleration * 180.0 / M_PI; 
      esp_109.lateral_acceleration = 0; // todo. n
      esp_109.yaw_rate = msg->vehicle_signals.yaw_rate;

      // 创建can frame 并写到总线
      struct can_frame data1;
      memset(&data1, 0, sizeof(data1));
      data1.can_id = 0x109;
      data1.can_dlc = 8;
      esp_0x109_pack(data1.data, &esp_109);
      can_.writeFrame(data1);
      dump_can_frame(data1);
    }


    // rewrite the can frame esp_0x104
    {
      struct esp_0x104_t esp_104;
      // ps. msg中单位为 m/s, CAN中单位为 km/h
      esp_104.speed = msg->vehicle_signals.longitudinal_velocity * 3.6; 

      // 创建can frame 并写到总线
      struct can_frame data1;
      memset(&data1, 0, sizeof(data1));
      data1.can_id = 0x104;
      data1.can_dlc = 8;
      esp_0x104_pack(data1.data, &esp_104);
      can_.writeFrame(data1);
      dump_can_frame(data1);
    }

  }


  void dump_can_frame(const can_frame& frame)
  {
    printf("rewrite can frame ok. [0x%x] ", frame.can_id);
    for (int i = 0; i < frame.can_dlc; i++) {
      printf("%02x ", frame.data[i]);
    }
    printf("\n");
  }
  
};


int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  std::string can_interface = "can0";
  if (argc > 1) {
    can_interface = argv[1];
  }
  printf("AB can replay start. use %s\n", can_interface.c_str());

  auto node = std::make_shared<ABCanReplay>(can_interface);
  rclcpp::spin(node);
  rclcpp::shutdown();

  return 0;
}
