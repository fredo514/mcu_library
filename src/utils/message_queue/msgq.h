/**
 * @file   msgq.h
 *
 * @brief   Multiple-producer, single-consumer circular message queue.
 * @details
 * This module implements a lockless message queue suitable for systems
 * with multiple producers and a single consumer. It stores variable-length
 * messages in a fixed-size circular buffer.
 *
 * Each message is stored alongside 2 bytes of metadata (whether the message is ready to be readm, and its length).
 * Producers atomically reserve space, copy their data, and commit the write. The consumer reads messages in FIFO order.

 *
 * This implementation is appropriate for interrupt-driven or multicore
 * embedded systems where producers cannot block and the consumer drains
 * messages in a single thread or task context.
 *
 * @note The buffer size must be a power of two to enable efficient masking
 *       of indices.
 * @note The maximum message size is limited to 256 bytes.
 */

#ifndef MSGQ_H
#define MSGQ_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "error.h"

// -----------------------------------------------------------------------------
// Public Constants
// -----------------------------------------------------------------------------

#define MSGQ_MAX_MSG_SIZE (UINT8_MAX + 1)

// -----------------------------------------------------------------------------
// Public Types
// -----------------------------------------------------------------------------

/**
 * @brief Opaque message queue Pointer.
 */
typedef struct _msgq_ctx msgq_t;

// -----------------------------------------------------------------------------
// Public Functions
// -----------------------------------------------------------------------------

msgq_t* Msgq_Create(uint16_t const len);
void Msgq_Init(msgq_t* const queue);

bool Msgq_Push(msgq_t* const queue, void* const pSrc, uint8_t const msg_len);
bool Msgq_Pop(msgq_t* const queue, void* pDest, uint8_t const dest_len, uint8_t* msg_len);

bool Msgq_Is_Msg_Available(msgq_t* const queue);

#ifdef __cplusplus
}
#endif

#endif  // MSGQ_H