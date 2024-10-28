#include <cstdio>
#include <rclcpp/rclcpp.hpp>
#include <ab_udp_msg/msg/ab_signals_v1.hpp>

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
    printf("\tlongitudinal_velocity: %f\n", msg->vehicle_signals.longitudinal_velocity);
    printf("\tlongitudinal_acceleration: %f\n", msg->vehicle_signals.longitudinal_acceleration);
    printf("\tyaw_rate: %f\n", msg->vehicle_signals.yaw_rate);
    printf("\tspeed_o_speed: %f\n", msg->vehicle_signals.speed_o_speed);


    // rewrite the can frame
    struct message_1_0x6b_t temp;
    temp.longitudinal_acceleration = msg->vehicle_signals.longitudinal_acceleration;
    temp.speed = msg->vehicle_signals.longitudinal_velocity;
    temp.yaw_rate = msg->vehicle_signals.yaw_rate;

    struct can_frame frame_to_write;
    frame_to_write.can_id = 0x6b;
    frame_to_write.can_dlc = 8;
    frame_to_write.data[0] = 0x00;
    message_1_0x6b_pack(frame_to_write.data, &temp);

    can_.writeFrame(frame_to_write);
    dump_can_frame(frame_to_write);
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
