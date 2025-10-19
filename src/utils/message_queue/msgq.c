#include "msgq.h"

#include "assert.h"

#define IS_POWER_OF_2(x) (x && !(x & (x - 1)))

struct _msgq_ctx {
   uint8_t* buffer;
   uint16_t capacity;
};

msgq_t* Msgq_Create(uint16_t const size) {
   assert(IS_POWER_OF_2(size));

   msgq_t* inst = calloc(1, sizeof(msgq_t));
   assert(inst);

   inst->buffer = calloc(size, sizeof(uint8_t));
   inst->capacity = size;

   return inst;
}