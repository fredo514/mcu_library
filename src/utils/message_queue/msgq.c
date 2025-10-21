#include "msgq.h"

#include <stdlib.h>

#include "assert.h"
#include "core.h"
#include "stdio.h"
#include "string.h"

#define IS_POWER_OF_2(x) (x && !(x & (x - 1)))

struct _msgq_ctx {
   uint8_t* buffer;
   uint16_t capacity;
   volatile uint16_t head;
   volatile uint16_t tail;
};

typedef struct {
   bool is_ready;
   uint8_t len;
   uint8_t data[];
} msg_t;

msgq_t* Msgq_Create(uint16_t const size) {
   assert(IS_POWER_OF_2(size));

   msgq_t* inst = calloc(1, sizeof(msgq_t));
   assert(inst);

   uint8_t* buf = calloc(size, sizeof(uint8_t));
   assert(buf);
   inst->buffer = buf;
   inst->capacity = size;

   return inst;
}

void Msgq_Init(msgq_t* const queue) {
   assert(queue);

   queue->head = 0;
   queue->tail = 0;
}

bool Msgq_Push(msgq_t* const queue, void* const src, uint8_t const msg_len) {
   assert(queue);
   assert(src);
   assert(msg_len <= MSGQ_MAX_MSG_SIZE);

   uint16_t start = 0;
   msg_t* msg_slot = NULL;
   bool part = false;

   // lock interrupts
   CRITICAL_SECTION(CORE_IRQ_ALL) {
      // make sure there is enough space left
      uint16_t space_left;
      if (queue->tail > queue->head) {
         space_left = queue->tail - queue->head;
      } else {
         space_left = queue->capacity - (queue->head - queue->tail);
      }
      if (msg_len > space_left) {
         // todo restore interrupts
         return false;
      }

      // reserve block
      start = queue->head;
      queue->head += sizeof(msg_t) + msg_len;
      // wrap
      queue->head &= queue->capacity - 1;
      if (queue->head < start) {
         part = true;
      }

      // mark not ready
      msg_slot = (msg_t*)&(queue->buffer[start]);
      msg_slot->is_ready = false;
   }

   // copy data with wrap
   for (size_t i = 0; i < sizeof(msg_slot->len); ++i) {
      queue->buffer[(start + sizeof(msg_slot->is_ready) + i) & (queue->capacity - 1)] = ((uint8_t*)msg_len)[i];
   }
   start += sizeof(msg_slot->len);
   start &= queue->capacity - 1;

   for (uint8_t i = 0; i < msg_len; ++i) {
      queue->buffer[(start + i) & (queue->capacity - 1)] = ((uint8_t*)src)[i];
   }

   // mark ready
   msg_slot->is_ready = true;

   return true;
}

bool Msgq_Pop(msgq_t* const queue, void* dest, uint8_t const dest_len, uint8_t* msg_len) {
   // make sure queue not empty
   if (!Msgq_Is_Msg_Available(queue)) {
      return false;
   }

   uint8_t start = queue->tail;
   msg_t* msg_slot = (msg_t*)&(queue->buffer[start]);

   // make sure msg at tail is ready
   assert(msg_slot->is_ready);

   // TODO: handle wrap
   for (size_t i = 0; i < len; ++i) {
      ((uint8_t*)&dest)[i] = queue->buffer[(start + sizeof(msg_slot->is_ready) + i) & (queue->capacity - 1)];
   }

   uint8_t len = msg_slot->len;
   // make sure dest buffer is long enough
   assert(dest_len >= len);

   // copy data with wrap
   // TODO: handle wrap
   memcpy(dest, msg_slot->data, len);
   *msg_len = len;

   // advance tail
   queue->tail += sizeof(msg_t) + len;
   // wrap
   queue->tail &= queue->capacity - 1;

   return true;
}

bool Msgq_Is_Msg_Available(msgq_t* const queue) {
   return ((queue->head != queue->tail) && (((msg_t*)&(queue->buffer[queue->tail]))->is_ready));
}