// TODO: make it atomic (thread-safe)
// TODO: make it type-generic template

#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>
#include <stdbool.h>

#include "error.h"

typedef enum {
    QUEUE_OVERRUN_DROP_OLD,
    QUEUE_OVERRUN_DROP_NEW, 
    QUEUE_OVERRUN_MODE_MAX
} queue_overrun_mode_t;

typedef struct queue_ctx * const queue_h;

typedef struct {
    queue_overrun_mode_t overrun_mode;
} queue_config_t;

queue_h Queue_Create(size_t len, size_t item_size);
// error_t Queue_Free(queue_h queue);
error_t Queue_Init(queue_h queue, queue_config_t const * const config);
void Queue_Flush(queue_h queue);
// size_t queue_capacity_get(queue_h queue);
// size_t queue_used_qty_get(queue_h queue);
// size_t queue_free_qty_get(queue_h queue);

bool Queue_Is_Full(queue_h queue);
bool Queue_Is_Empty(queue_h queue);

error_t Queue_Push(queue_h queue, void const * const src_item);
// error_t Queue_Push_Front(queue_h queue, void const * const src_item);
error_t Queue_Pop(queue_h queue, void * const dest_item);
// error_t queue_get(queue_h queue, queue_elem_t* val);
// size_t queue_process_all(queue_h queue, void (*cb)(void *));

#ifdef __DEBUG
size_t Queue_Watermark_Get(queue_h queue);
#endif // __DEBUG

#endif // QUEUE_H
