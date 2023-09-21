// TODO: make it atomic (thread-safe)
// TODO: make it type-generic template

#ifndef RINGFIFO_H
#define	RINGFIFO_H

#include <stdbool.h>
#include <stdint.h>
#include "error.h"

// TODO: make this a template?
typedef RINGFIFO_ITEM_t uint8_t;

typedef enum {
    RINGFIFO_OVERRUN_DROP_OLD,
    RINGFIFO_OVERRUN_DROP_NEW,
    RINGFIFO_OVERRUN_MODE_MAX
} RINGFIFO_OVERRUN_MODE_t

typedef RINGFIFO_CTX const * const RINGFIFO_h;

RINGFIFO_h RingFifo_Create(uint8_t len);
ERROR_CODE_t RingFifo_Init(RINGFIFO_h fifo, RINGFIFO_OVERRUN_MODE_t mode);

ERROR_CODE_t RingFifo_Push(RINGFIFO_h fifo, RINGFIFO_ITEM_t val);
ERROR_CODE_t RingFifo_Pop(RINGFIFO_h fifo, RINGFIFO_ITEM_t* val);
ERROR_CODE_t RingFifo_Flush(RINGFIFO_h fifo);

bool Ringfifo_Is_Empty(RINGFIFO_h fifo);
bool Ringfifo_Is_Full(RINGFIFO_h fifo);

#endif // RINGFIFO_H