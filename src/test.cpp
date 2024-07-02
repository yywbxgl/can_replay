#include <string>
#include <unistd.h>
#include <cstring>

#include "dds_helper.h"
#include "CanFramePubSubTypes.h"
#include "log.h"
#include "socket_can.h"


int g_log_level = L_DEBUG;


int main()
{
    SocketCan can;

    if (can.init("can0") == false) {
        return EXIT_FAILURE;
    }
  

    while(1) {
        can_frame frame;
        if (can.readFrame(frame)) {
            printf("recv can. id=%03x, data=", frame.can_id);
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