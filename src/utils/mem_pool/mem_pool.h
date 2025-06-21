#ifndef MEM_POOL_H
#define MEM_POOL_H

#include <stdint.h>
#include "error.h"

typedef struct MEM_POOL_CTX * MEM_POOL_h;
typedef struct MEM_BLOCK_CTX * MEM_BLOCK_h;

MEM_POOL_h mempool_create(size_t block_size, size_t num_blocks); 
MEM_BLOCK_h mempool_alloc(MEM_POOL_h pool); 
ERROR_CODE_t mempool_free(MEM_POOL_h pool, MEM_BLOCK_h block); 

#endif // MEM_POOL_H