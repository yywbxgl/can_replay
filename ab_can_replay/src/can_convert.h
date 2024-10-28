#pragma once

#include <stdint.h>
#include <unistd.h>

// static inline uint8_t pack_left_shift_u8(
//     uint8_t value,
//     uint8_t shift,
//     uint8_t mask)
// {
//     return (uint8_t)((uint8_t)(value << shift) & mask);
// }

// static inline uint8_t pack_left_shift_u16(
//     uint16_t value,
//     uint8_t shift,
//     uint8_t mask)
// {
//     return (uint8_t)((uint8_t)(value << shift) & mask);
// }

// static inline uint8_t pack_left_shift_u32(
//     uint32_t value,
//     uint8_t shift,
//     uint8_t mask)
// {
//     return (uint8_t)((uint8_t)(value << shift) & mask);
// }

// static inline uint8_t pack_right_shift_u8(
//     uint8_t value,
//     uint8_t shift,
//     uint8_t mask)
// {
//     return (uint8_t)((uint8_t)(value >> shift) & mask);
// }

// static inline uint8_t pack_right_shift_u16(
//     uint16_t value,
//     uint8_t shift,
//     uint8_t mask)
// {
//     return (uint8_t)((uint8_t)(value >> shift) & mask);
// }

// static inline uint8_t pack_right_shift_u32(
//     uint32_t value,
//     uint8_t shift,
//     uint8_t mask)
// {
//     return (uint8_t)((uint8_t)(value >> shift) & mask);
// }

// static inline uint8_t unpack_left_shift_u8(
//     uint8_t value,
//     uint8_t shift,
//     uint8_t mask)
// {
//     return (uint8_t)((uint8_t)(value & mask) << shift);
// }

// static inline uint16_t unpack_left_shift_u16(
//     uint8_t value,
//     uint8_t shift,
//     uint8_t mask)
// {
//     return (uint16_t)((uint16_t)(value & mask) << shift);
// }

// static inline uint32_t unpack_left_shift_u32(
//     uint8_t value,
//     uint8_t shift,
//     uint8_t mask)
// {
//     return (uint32_t)((uint32_t)(value & mask) << shift);
// }

// static inline uint8_t unpack_right_shift_u8(
//     uint8_t value,
//     uint8_t shift,
//     uint8_t mask)
// {
//     return (uint8_t)((uint8_t)(value & mask) >> shift);
// }

// static inline uint16_t unpack_right_shift_u16(
//     uint8_t value,
//     uint8_t shift,
//     uint8_t mask)
// {
//     return (uint16_t)((uint16_t)(value & mask) >> shift);
// }

// static inline uint32_t unpack_right_shift_u32(
//     uint8_t value,
//     uint8_t shift,
//     uint8_t mask)
// {
//     return (uint32_t)((uint32_t)(value & mask) >> shift);
// }


struct message_1_0x6b_t {
    double longitudinal_acceleration;
    double speed;
    double yaw_rate;
};


// todo.
bool message_1_0x6b_pack(uint8_t *dst_p, const struct message_1_0x6b_t *src_p)
{
    dst_p[0] = (uint8_t)(src_p->longitudinal_acceleration / 0.001);
    dst_p[1] = (uint8_t)(src_p->speed / 0.01);
    dst_p[2] = (uint8_t)(src_p->yaw_rate / 0.01);
    return true;
}



// bool message_1_0x6b_unpack(struct message_1_0x6b_t *dst_p, const uint8_t *src_p, size_t len)
// {
//     return true;
// }