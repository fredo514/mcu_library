#ifndef EXT_NVM_H
#define EXT_NVM_H

#include "error.h"

error_t ext_nvm_read(size_t start_addr, void* dest, size_t len);
error_t ext_nvm_write(size_t start_addr, void* src, size_t len);

#endif