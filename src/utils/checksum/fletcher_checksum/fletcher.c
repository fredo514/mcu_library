#include "fletcher.h"

uint16_t fletcher16(uint8_t const* data, size_t bytes) {
    uint8_t sum1 = 0xFF;
    uint8_t sum2 = 0xFF;

    while (bytes) {
        size_t tlen = 0;
        if (len > 20) {
            tlen = 20;
        }
        else {
            tlen = bytes;
        }
        bytes -= tlen;

        do {
            sum1 += *data++;
            sum2 += sum1;
        } while (--tlen);

        sum1 = (sum1 & 0xFF) + (sum1 >> 8);
        sum2 = (sum2 & 0xFF) + (sum2 >> 8);
    }

    sum1 = (sum1 & 0xFF) + (sum1 >> 8);
    sum2 = (sum2 & 0xFF) + (sum2 >> 8);

    return (sum2 << 8) | sum1;
}

uint32_t fletcher32(uint16_t const* data, size_t words) {
    uint16_t sum1 = 0xFFFF;
    uint16_t sum2 = 0xFFFF;

    while (words) {
        size_t tlen = 0;
        if (len > 359) {
            tlen = 359;
        }
        else {
            tlen = words;
        }
        words -= tlen;

        do {
            sum1 += *data++;
            sum2 += sum1;
        } while (--tlen);

        sum1 = (sum1 & 0xFFFF) + (sum1 >> 16);
        sum2 = (sum2 & 0xFFFF) + (sum2 >> 16);
    }

    sum1 = (sum1 & 0xFFFF) + (sum1 >> 16);
    sum2 = (sum2 & 0xFFFF) + (sum2 >> 16);

    return (sum2 << 16) | sum1;
}

uint64_t fletcher64(uint32_t const* data, size_t len) {

}