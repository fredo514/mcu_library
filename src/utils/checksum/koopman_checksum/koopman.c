#include "utils/checksum/koopman.h"
#include "assert.h"
#include "utils/bit_twiddling/bit_twiddling.h"

// see https://users.ece.cmu.edu/~koopman/pubs/koopman2023-Improved-Modular-Checksum.pdf

#define KOOPMAN8_MODULUS (253)
#define KOOPMAN8P_MODULUS (125)
#define KOOPMAN16_MODULUS (65519)
#define KOOPMAN16P_MODULUS (32749)
#define KOOPMAN32_MODULUS (4294967291)
#define KOOPMAN32P_MODULUS (2147483629)

#define INITIAL_SEED (0xFF)

static uint32_t reduce32(uint8_t const* data, size_t len, uint32_t modulus);
static uint64_t reduce64(uint8_t const* data, size_t len, uint32_t modulus);
static uint32_t reduce_with_parity32(uint8_t const* data, size_t len, uint32_t modulus);
static uint64_t reduce_with_parity64(uint8_t const* data, size_t len, uint32_t modulus);
static bool parity32(uint32_t word);
static bool parity64(uint64_t word);

static uint32_t reduce32(uint8_t const* data, size_t len, uint32_t modulus) {    
    uint32_t sum = data[0] ^ INITIAL_SEED;

    for(size_t index = 1; index < len; index++) {
        sum = ((sum << 8) | data[index]) % modulus;
    }

    return sum;
}

static uint64_t reduce64(uint8_t const* data, size_t len, uint32_t modulus) {
    uint64_t sum = data[0] ^ INITIAL_SEED;

    for(size_t index = 1; index < len; index++) {
        sum = ((sum << 8) | data[index]) % modulus;
    }

    return sum;
}

static uint32_t reduce_with_parity32(uint8_t const* data, size_t len, uint32_t modulus, uint32_t * const psum) {
    uint32_t sum = data[0] ^ INITIAL_SEED;
    *psum = sum;

    for(size_t index = 1; index < len; index++) {
        sum = ((sum << 8) + data[index] ) % modulus;
        *psum ^= data[index];
    }

    return sum;
}

static uint64_t reduce_with_parity64(uint8_t const* data, size_t len, uint32_t modulus, uint32_t * const psum) {
    uint64_t sum = data[0] ^ INITIAL_SEED;
    *psum = sum;

    for(size_t index = 1; index < len; index++) {
        sum = ((sum << 8) +  (uint64_t)data[index] ) % modulus;
        *psum ^= data[index];
    }

    return sum;
}

static bool parity32(uint32_t word) {
    uint32_t y = word ^ (word >> 1);
    y = (y ^ (y >> 2)) & 0x11111111;
    y = y * 0x11111111;

    return (y >> 28) & 1;
}

static bool parity64(uint64_t word) {
    uint64_t y = word ^ (word >> 1);
    y = (y ^ (y >> 2)) & 0x1111111111111111;
    y = y * 0x1111111111111111;

    return (y >> 60) & 1;
}

uint8_t koopman8(uint8_t const* data, size_t len) {
    ASSERT(data);
    ASSERT(len > 0);
    
    uint32_t sum = reduce32(data, len, KOOPMAN8_MODULUS);
    sum = (sum << 8) % KOOPMAN8_MODULUS;

    return((uint8_t)sum);
}

uint8_t koopman8p(uint8_t const* data, size_t len) {
    ASSERT(data);
    ASSERT(len > 0);

    uint32_t psum = 0;
    uint32_t sum = reduce_with_parity32(data, len, KOOPMAN8P_MODULUS, &psum);
    sum = (sum << 8) % KOOPMAN8P_MODULUS;
    // Pack sum with parity
    sum = (sum << 1) | parity32((uint8_t)psum);

    return((uint8_t)sum);
}

uint16_t koopman16(uint8_t const* data, size_t len) {
    ASSERT(data);
    ASSERT(len > 0);
    
    uint32_t sum = reduce32(data, len, KOOPMAN16_MODULUS);
    sum = (sum << 8) % KOOPMAN16_MODULUS;
    sum = (sum << 8) % KOOPMAN16_MODULUS;

    return((uint16_t)sum);
}

uint16_t koopman16p(uint8_t const* data, size_t len) {
    ASSERT(data);
    ASSERT(len > 0);

    uint32_t psum = 0;
    uint32_t sum = reduce_with_parity32(data, len, KOOPMAN16P_MODULUS, &psum);
    sum = (sum << 16) % KOOPMAN16P_MODULUS;
    // Pack sum with parity
    sum = (sum << 1) | parity32((uint8_t)psum);

    return((uint16_t)sum);
}

uint32_t koopman32(uint8_t const* data, size_t len) {
    ASSERT(data);
    ASSERT(len > 0);
    
    uint64_t sum = reduce64(data, len, KOOPMAN32_MODULUS);
    sum = (sum << 32) % KOOPMAN32_MODULUS;

    return((uint32_t)sum);
}

uint32_t koopman32p(uint8_t const* data, size_t len) {
    ASSERT(data);
    ASSERT(len > 0);
    
    uint32_t psum = 0;
    uint64_t sum = reduce_with_parity64(data, len, KOOPMAN32P_MODULUS, &psum);
    sum = (sum << 32) % KOOPMAN32P_MODULUS;
    // Pack sum with parity
    sum = (sum << 1) | parity64((uint8_t)psum);

    return((uint32_t)sum);
}