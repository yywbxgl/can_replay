#include <string>
#include <unistd.h>
#include <cstring>

#include "dds_helper.h"
#include "CanFramePubSubTypes.h"
#include "CanFrameListPubSubTypes.h"
#include "log.h"
#include "socket_can.h"


int g_log_level = L_DEBUG;
SocketCan can;

using namespace proj233_msgs::msg;


void rewrite_can_frame(CanFrame* msg)
{
    can_frame frame;
    frame.can_id  = msg->id();
    frame.can_dlc = msg->len();
    memcpy(frame.data, msg->data().data(), 8);

    if (can.writeFrame(frame) == false) {
        LOG_ERROR("Failed to send frame");
    } else {
        LOG_INFO("rewrite can [%03X] ", frame.can_id);
        for(int i = 0; i < 8; i++) {
            printf("%02x ", frame.data[i]);
        }
        printf("\n");
    }
}


void signal_can_data_callback(void* data)
{
    LOG_DEBUG("Received can frame");
    rewrite_can_frame((CanFrame*)data);
}


void squence_can_data_callback(void* data)
{
    LOG_DEBUG("Received can frame list");
    CanFrameList* msg = (CanFrameList*)data;
    for(int i = 0; i < msg->can_frames().size(); i++) {
        rewrite_can_frame(&msg->can_frames()[i]);
    }
}


int main(int argc, char* argv[])
{

    std::string can_interface = "can0";
    if (argc > 1) {
        can_interface = argv[1];
    }

    printf("start can replay. version: 20250305. use %s\n", can_interface.c_str());


    if (can.init(can_interface) == false) {
        return EXIT_FAILURE;
    }
  
    DDSHelper dds_helper;
    dds_helper.registe_reader("rt/dds_replay/can" , new CanFramePubSubType(), signal_can_data_callback);
    dds_helper.registe_reader("rt/mcu/raw_can", new CanFrameListPubSubType(), squence_can_data_callback);

    while(1) {
        sleep(1);
    }

    return EXIT_SUCCESS;
}