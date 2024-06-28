#include <string>
#include <unistd.h>

#include "dds_helper.h"
#include "CanFramePubSubTypes.h"
#include "log.h"


int g_log_level = L_DEBUG;


void data_callback(void* data)
{
    CanFrame* frame = (CanFrame*)data;
    printf("recv can frame: id=%d, timestamp=%lu\n", frame->id(), frame->timestamp());
}


int main()
{
    DDSHelper dds_helper;
    dds_helper.registe_reader("rt/dds_replay/can" , new CanFramePubSubType(), data_callback);

    while(1) {
        sleep(1);
    }
}