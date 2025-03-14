#include <string>
#include <unistd.h>
#include <cstring>

#include "dds_helper.h"
#include "CanFramePubSubTypes.h"
#include "CanFrameListPubSubTypes.h"
#include "log.h"


int g_log_level = L_DEBUG;

using namespace proj233_msgs::msg;


void test_can_cnt(void* data)
{
    static std::map<uint32_t, int> cnt_map;
    static int total_cnt = 0;
    CanFrameList* msg = (CanFrameList*)data;

    LOG_DEBUG("Received can frame list. size=%ld\n", msg->can_frames().size());

    for(int i = 0; i < msg->can_frames().size(); i++) {
        
        printf("recv can [0x%03X] ", msg->can_frames()[i].id());
        for(int j = 0; j < 8; j++) {
            printf("%02x ", msg->can_frames()[i].data()[j]);
        }
        printf("\n");

        int can_id = msg->can_frames()[i].id();
        if (cnt_map.find(can_id) == cnt_map.end()) {
            cnt_map[can_id] = 1;
        } else {
            cnt_map[can_id]++;
        }
        total_cnt++;
    }

    printf("cnt:  ");
    for(auto it = cnt_map.begin(); it != cnt_map.end(); it++) {
        printf("[0x%03X]=%d, ", it->first, it->second);
    }
    printf("total=%d\n", total_cnt);
}


int main(int argc, char* argv[])
{

    std::string can_interface = "can0";
    if (argc > 1) {
        can_interface = argv[1];
    }

    printf("start can test. version: 20250314. \n");

    DDSHelper dds_helper;
    dds_helper.registe_reader("rt/mcu/raw_can", new CanFrameListPubSubType(), test_can_cnt);

    while(1) {
        sleep(1);
    }

    return EXIT_SUCCESS;
}