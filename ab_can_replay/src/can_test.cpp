#include <string>
#include <unistd.h>
#include <cstring>

#include "socket_can.h"


int g_log_level = L_DEBUG;


int main(int argc, char* argv[])
{
    SocketCan can;

    std::string can_interface = "can0";
    if (argc > 1) {
        can_interface = argv[1];
    }


    if (can.init(can_interface) == false) {
        return EXIT_FAILURE;
    }
  
    while(1) {
        can_frame frame;
        if (can.readFrame(frame)) {
            printf("rewrite can [%03x] ", frame.can_id);
            for(int i = 0; i < 8; i++) {
                printf("%02x ", frame.data[i]);
            }
            printf("\n");
        } else {
            LOG_ERROR("Failed to read frame");
        }

        // rewrited can frame
        if (can.writeFrame(frame) == false) {
            LOG_ERROR("Failed to send frame");
        }
    } 

    return EXIT_SUCCESS;
}