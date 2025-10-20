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

   // lock interrupts
   CRITICAL_SECTION(CORE_IRQ_ALL) {
      // make sure there is enough space left
      start = queue->head;
      // TODO: exit if not enough space left

      // reserve block
      // TODO: handle wrap
      queue->head += sizeof(msg_t) + msg_len;

      // mark not ready
      msg_slot = (msg_t*)&(queue->buffer[start]);
      msg_slot->is_ready = false;
   }

   // copy data with wrap
   // TODO: handle wrap
   msg_slot->len = msg_len;
   memcpy(msg_slot->data, src, msg_len);

   // mark ready
   msg_slot->is_ready = true;

   return true;
}

bool Msgq_Pop(msgq_t* const queue, void* dest, uint8_t const dest_len, uint8_t* msg_len) {
   // make sure queue not empty
   if (!Msgq_Is_Msg_Available(queue)) {
      return false;
   }

   msg_t* msg_slot = (msg_t*)&(queue->buffer[queue->tail]);

   // make sure msg at tail is ready
   assert(msg_slot->is_ready);

   // TODO: handle wrap
   uint8_t len = msg_slot->len;
   // make sure dest buffer is long enough
   assert(dest_len >= len);

   // copy data with wrap
   // TODO: handle wrap
   memcpy(dest, msg_slot->data, len);
   *msg_len = len;

   // advance tail
   // TODO: handle wrap
   queue->tail += sizeof(msg_t) + len;

   return true;
}

bool Msgq_Is_Msg_Available(msgq_t* const queue) {
   return ((queue->head != queue->tail) && (((msg_t*)&(queue->buffer[queue->tail]))->is_ready));
}