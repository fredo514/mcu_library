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

static uint16_t Copy_To_Queue(msgq_t* const queue, uint8_t const* const pSrc, uint16_t const start, uint8_t const len);
static uint16_t Copy_From_Queue(msgq_t* const queue, uint8_t* const pDest, uint16_t const start, uint8_t const len);

/**
 * @brief Allocate a message queue instance and its buffer on the heap.
 * @note Msgq_Init must be called on the instance before use
 *
 * @return Pointer to the created instance.
 */
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

/**
 * @brief Initialize a message queue.
 *
 * @param[inout] queue   queue object.
 *
 * @return None
 */
void Msgq_Init(msgq_t* const queue) {
   assert(queue);

   queue->head = 0;
   queue->tail = 0;

   memset(queue->buffer, 0, queue->capacity);
}

/**
 * @brief Pushes the provided message to the queue if there is enough space left.
 *
 * @param[inout] queue   Message queue object.
 * @param[in] pSrc Message to store source buffer.
 * @param msg_len length of message to store.
 *
 * @return True if stored, false otherwise
 */
bool Msgq_Push(msgq_t* const queue, void* const pSrc, uint8_t const msg_len) {
   assert(queue);
   assert(pSrc);
   assert(msg_len <= MSGQ_MAX_MSG_SIZE);

   uint16_t is_ready_pos = 0;
   uint16_t curr_idx = 0;
   bool part = false;

   // lock interrupts to reserve a memory block atomically
   bool int_state = Core_Interrupts_SaveAndDisable(CORE_IRQ_ALL);

   // make sure there is enough space left
   uint16_t space_left;
   // empty?
   if (queue->tail == queue->head) {
      space_left = queue->capacity;
   }
   // are we crossing wrap boundary?
   else if (queue->tail >= queue->head) {
      space_left = queue->tail - queue->head;
   } else {
      space_left = queue->capacity - (queue->head - queue->tail);
   }

   if ((msg_len + sizeof(msg_t)) > space_left) {
      // not enough space left
      Core_Interrupts_Restore(CORE_IRQ_ALL, int_state);
      return false;
   }

   // reserve block
   is_ready_pos = queue->head;
   queue->head += sizeof(msg_t) + msg_len;
   // wrap
   queue->head &= queue->capacity - 1;

   // mark not ready
   const bool not_ready = false;
   curr_idx = Copy_To_Queue(queue, (uint8_t*)&not_ready, is_ready_pos, sizeof(not_ready));

   Core_Interrupts_Restore(CORE_IRQ_ALL, int_state);

   // copy message len
   curr_idx = Copy_To_Queue(queue, (uint8_t*)&msg_len, curr_idx, sizeof(msg_len));

   // copy data
   (void)Copy_To_Queue(queue, pSrc, curr_idx, msg_len);

   // mark ready
   queue->buffer[is_ready_pos] = true;

   return true;
}

/**
 * @brief Pop the oldest message from the queue to provided buffer.
 *
 * @param[inout] queue   Message queue object.
 * @param[out] pDest Destination buffer.
 * @param dest_len length of destination buffer (for overflow checks).
 * @param[out] pMsg_len popped message length.
 *
 * @return True if message popped, false otherwise
 */
bool Msgq_Pop(msgq_t* const queue, void* pDest, uint8_t const dest_len, uint8_t* pMsg_len) {
   assert(queue);
   assert(pDest);
   assert(pMsg_len);

   // make sure queue not empty
   if (!Msgq_Is_Msg_Available(queue)) {
      return false;
   }

   uint16_t curr_idx = queue->tail;

   // get message metadata
   bool is_ready;
   curr_idx = Copy_From_Queue(queue, (uint8_t*)&is_ready, curr_idx, sizeof(is_ready));

   uint8_t msg_len;
   curr_idx = Copy_From_Queue(queue, (uint8_t*)&msg_len, curr_idx, sizeof(msg_len));

   // make sure dest buffer is long enough
   assert(dest_len >= msg_len);

   // copy data
   queue->tail = Copy_From_Queue(queue, pDest, curr_idx, msg_len);
   *pMsg_len = msg_len;

   return true;
}

/**
 * @brief Checks whether a message is availbale in the queue.
 *
 * @param[in] queue   Message queue object.
 *
 * @return True if message available, false otherwise
 */
bool Msgq_Is_Msg_Available(msgq_t* const queue) {
   assert(queue);

   bool is_not_empty = queue->head != queue->tail;
   bool msg_is_ready = queue->buffer[queue->tail] == true;

   return (is_not_empty && msg_is_ready);
}

/**
 * @brief Copy the provided buffer to the message queue circular buffer while handling wrap.
 *
 * @param[inout] queue   Message queue object.
 * @param[in] pSrc Source buffer.
 * @param start Initial index to start the copy from
 * @param len number of bytes to copy.
 *
 * @return Advanced circular buffer index
 */
static uint16_t Copy_To_Queue(msgq_t* const queue, uint8_t const* const pSrc, uint16_t const start, uint8_t const len) {
   // find how many butes left until end of buffer
   uint16_t remaining = queue->capacity - start;

   // check if crossing wrap boundary
   if (len <= remaining) {
      // no, no need to split
      memcpy(&queue->buffer[start], pSrc, len);
   } else {
      // yes, split copy in 2 parts
      memcpy(&queue->buffer[start], pSrc, remaining);
      memcpy(&queue->buffer[0], pSrc + remaining, len - remaining);
   }

   // advance index with wrap
   return (start + len) & (queue->capacity - 1);
}

/**
 * @brief Copy the a number of bytes from the queue circular buffer to the provided buffer while handling wrap.
 *
 * @param[inout] queue   Message queue object.
 * @param[in] pDest Destination buffer.
 * @param start Initial index to start the copy from
 * @param len number of bytes to copy.
 *
 * @return Advanced circular buffer index
 */
static uint16_t Copy_From_Queue(msgq_t* const queue, uint8_t* const pDest, uint16_t const start, uint8_t const len) {
   // find how many butes left until end of buffer
   uint16_t remaining = queue->capacity - start;

   // check if crossing wrap boundary
   if (len <= remaining) {
      // no, no need to split
      memcpy(pDest, &queue->buffer[start], len);
   } else {
      // yes, split copy in 2 parts
      memcpy(pDest, &queue->buffer[start], remaining);
      memcpy(pDest + remaining, queue->buffer[0], len - remaining);
   }

   // advance index with wrap
   return (start + len) & (queue->capacity - 1);
}
