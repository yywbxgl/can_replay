#include <cstdio>
#include <rclcpp/rclcpp.hpp>
#include <ab_udp_msg/msg/ab_signals_v1.hpp>

#include "socket_can.h"


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
    printf("-----------vehicle_data_recv------------\n");
    printf("time stamp: %d %d\n", msg->header.stamp.sec, msg->header.stamp.nanosec);
    printf("longitudinal_velocity: %f\n", msg->vehicle_signals.longitudinal_velocity);
    printf("longitudinal_acceleration: %f\n", msg->vehicle_signals.longitudinal_acceleration);
    printf("yaw_rate: %f\n", msg->vehicle_signals.yaw_rate);
    printf("speed_o_speed: %f\n", msg->vehicle_signals.speed_o_speed);


    // 车速
    // longitudinal_velocity
    

    // 纵向加速度
    // longitudinal_acceleration

    // 转向角速度
    // yaw_rate
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
