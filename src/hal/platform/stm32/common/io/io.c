#include "io.h"

inline void Io_Write (REG_SIZE_t * const address, REG_SIZE_t const val) {
    REG_SIZE_t * p = 0;
    *(p + address) = val;
}

inline REG_SIZE_t Io_Read (REG_SIZE_t * const address) {
    REG_SIZE_t * p = 0;
    return *(p + address);
}