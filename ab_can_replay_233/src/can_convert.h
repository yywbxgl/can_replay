#pragma once

#include <stdint.h>
#include <unistd.h>

struct esp_0x109_t {
    double longitudinal_acceleration;
    double lateral_acceleration;
    double yaw_rate;
};


struct esp_0x104_t {
    double speed;
};



bool esp_0x109_pack(uint8_t *dst_p, const struct esp_0x109_t *src_p)
{
    uint16_t lat_acc_uint16 = (src_p->lateral_acceleration - (-26.752)) / 0.001 ;

    dst_p[0] |= (lat_acc_uint16 >> 8) & 0xff;
    dst_p[1] |= lat_acc_uint16 & 0xff;

    uint16_t long_acc_uint16 = (src_p->longitudinal_acceleration - (-26.752)) / 0.001 ;
    dst_p[2] |= (long_acc_uint16 >> 8) & 0xff;
    dst_p[3] |= long_acc_uint16 & 0xff;

    uint16_t yaw_rate_uint16  = (src_p->yaw_rate - (-128)) / 0.0625 ;
    dst_p[4] |= (yaw_rate_uint16 >> 4) & 0xff;
    dst_p[5] |= (yaw_rate_uint16 << 4) & 0xf0;
    
    //PS: 0x0f表示以上数据都有效，0x00表示数据无效
    dst_p[5] |= 0x0f;

    return true;
}


bool esp_0x104_pack(uint8_t *dst_p, const struct esp_0x104_t *src_p)
{
    uint16_t speed_uint16 = (src_p->speed - 0) / 0.05625 ;
    dst_p[4] |= (speed_uint16 >> 8) & 0x1f;
    dst_p[5] |= speed_uint16 & 0xff;

    return true;
}



// bool message_1_0x6b_unpack(struct message_1_0x6b_t *dst_p, const uint8_t *src_p, size_t len)
// {
//     return true;
// }