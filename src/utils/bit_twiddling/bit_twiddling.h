#ifndef BIT_TWIDDLING_H
#define BIT_TWIDDLING_H

#include <stdint.h>

/**
 * @brief Macro for testing if a value is a power of 2.
 *
 * @param[in] type   Structure type.
 * @param[in] member Structure member whose offset is searched for.
 *
 * @return Member offset in bytes.
 */
#define IS_POWER_OF_2(x) (x && !(x & (x - 1)))

bool parity32(uint32_t word);
bool parity64(uint32_t word);

static inline uint32_t log2_u32(uint32_t x) {
   if (x == 0) {
      return 0xFFFFFFFFu;  // undefined
   }
#if (__ARM_ARCH >= 6)  // Cortex-M3 and up support CLZ
   return 31u - __builtin_clz(x);
#else
   // allback using lookup table on bytes
   static const uint8_t
       log2_lut[256] =
           {
               [0] = 0xFF,                                               // 0 (undefined)
               0,                                                        // 1
               1,          1,                                            // 2-3
               2,          2, 2, 2,                                      // 4-7
               3,          3, 3, 3, 3, 3, 3, 3,                          // 8-15
               4,          4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,  // 16-31
               5,          5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
               5,          5, 5, 5,  // 32-63
               6,          6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
               6,          6, 6, 6,  // 64-127
               7,          7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
               7,          7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
               7,          7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
               7,          7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7  // 128-255
           };

   uint32_t r = 0;
   if (x >> 16) {
      x >>= 16;
      r += 16;
   }
   if (x >> 8) {
      x >>= 8;
      r += 8;
   }
   return r + log2_lut[x];
#endif
}

#endif