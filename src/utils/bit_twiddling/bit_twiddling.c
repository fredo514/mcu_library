#include "utils/bit_twiddling/bit_twiddling.h"

// Hacker's Delight p98
bool parity32(uint32_t word) {
    uint32_t y = word ^ (word >> 1);
    y = (y ^ (y >> 2)) & 0x11111111;
    y = y * 0x11111111;

    return (y >> 28) & 1;
}

bool parity64(uint64_t word) {
    uint64_t y = word ^ (word >> 1);
    y = (y ^ (y >> 2)) & 0x1111111111111111;
    y = y * 0x1111111111111111;

    return (y >> 60) & 1;
}