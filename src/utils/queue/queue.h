// TODO: make it atomic (thread-safe)
// TODO: make it type-generic template

#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include <stdbool.h>

#include "error.h"

typedef uint8_t queue_elem_t;

typedef enum {
    QUEUE_OVERRUN_DROP_OLD,
    QUEUE_OVERRUN_DROP_NEW, 
    QUEUE_OVERRUN_MODE_MAX
} queue_overrun_mode_t;

typedef struct {
    queue_elem_t* buffer;
    size_t size;
    queue_elem_t default_value;
    queue_overrun_mode_t overrun_mode;
} queue_init_t;

typedef struct queue_ctx const * const queue_t;

queue_t Queue_Create(size_t len);
error_t Queue_Init(queue_t queue, queue_init_t const * const config);
void Queue_Flush(queue_t queue);
// size_t queue_capacity_get(queue_t queue);
// size_t queue_used_qty_get(queue_t queue);
// size_t queue_free_qty_get(queue_t queue);

bool Queue_Is_Full(queue_t queue);
bool Queue_Is_Empty(queue_t queue);

error_t Queue_Push(queue_t queue, queue_elem_t val);
queue_elem_t Queue_Pop(queue_t queue);
// error_t queue_pop2(queue_t queue, queue_elem_t* val);
// queue_elem_t queue_get(queue_t queue);
// error_t queue_get2(queue_t queue, queue_elem_t* val);
// error_t queue_free(queue_t queue);
// size_t queue_process_all(queue_t queue, void (*cb)(queue_elem_t));

#endif // QUEUE_H
