#pragma once

#include <lookup.h>

static const uint16_t max = 0xfff;
static const uint16_t mid = max/2;

// rad = 2pi/2048
// rad = 0~0xfff
// 12bit sine
// 12 bit in 12 bit out
uint16_t sin12(uint16_t rad)
{
    if (rad > max) {
         rad = max;
    }

    static uint16_t q1 = 0xfff/4;


    if (rad <= q1) {
        return lookup[rad];
    } else if (rad <= q1*2) {
        return max - lookup[rad];
    } else if (rad <= q1*3) {
        return mid - lookup[rad];
    } else {
        return mid 
    }
    return 
}
