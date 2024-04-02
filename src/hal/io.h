#ifndef IO_H
#define IO_H

#include "platform.h"

void Io_Write (REG_SIZE_t * const address, REG_SIZE_t const val);
REG_SIZE_t Io_Read (REG_SIZE_t * const address);

#endif // IO_H