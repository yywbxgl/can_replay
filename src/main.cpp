#include <string>
#include <unistd.h>
#include <cstring>

#include "dds_helper.h"
#include "CanFramePubSubTypes.h"
#include "log.h"
#include "socket_can.h"


int g_log_level = L_DEBUG;
SocketCan can;



void data_callback(void* data)
{
    CanFrame* msg = (CanFrame*)data;
    LOG_INFO("recv can frame: id=%d, timestamp=%lu\n", msg->id(), msg->timestamp());

    can_frame frame;
    frame.can_id  = msg->id();
    frame.can_dlc = msg->dlc();
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


int main(int argc, char* argv[])
{

    std::string can_interface = "can0";
    if (argc > 1) {
        can_interface = argv[1];
    }

    if (can.init(can_interface) == false) {
        return EXIT_FAILURE;
    }
  
    DDSHelper dds_helper;
    dds_helper.registe_reader("rt/dds_replay/can" , new CanFramePubSubType(), data_callback);
  
    while(1) {
        sleep(1);
    }


    return EXIT_SUCCESS;
}