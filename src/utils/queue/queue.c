#include "queue.h"

#include "assert.h"

static size_t next_index(queue_t queue, size_t dex);

struct queue_ctx {
    queue_elem_t* buffer;
    size_t capacity;
    size_t index;
    size_t outdex;
    bool keep_new;
    queue_elem_t default_value;
};

error_t queue_init(queue_ctx_t * const queue, queue_init_t const * const config) {
    ASSERT(queue);
    ASSERT(config);
    ASSERT(config->overrun_mode < QUEUE_OVERRUN_MODE_MAX);

    queue->buffer = config->buffer;
    queue->capacity = config->size;

    queue_flush(queue);

    switch (config->overrun_mode) {
        case QUEUE_OVERRUN_DROP_OLD:
            queue->keep_new = true;
        break;

        case QUEUE_OVERRUN_DROP_NEW:
            queue->keep_new = false;
        break;

        default:
            // unknown overrun mode
            ASSERT(false);
    }

    queue->default_value = config->default_value;

    return SUCCESS;
}

bool queue_is_full(queue_ctx_t const * const queue) {
    ASSERT(queue);

    // keeping a free slot for this check allows concurency of 1 producer and many consumers
    // TODO: use full flag and mutex instead?
    size_t next_in = next_index(queue, queue->index);

    return (next_in == queue->outdex);
}

bool queue_is_empty(queue_ctx_t const * const queue) {
    ASSERT(queue);

    return (queue->index == queue->outdex);
}

error_t queue_push(queue_ctx_t * const queue, queue_elem_t val) {
    ASSERT(queue);

    if (queue_is_full(queue)) {
        if (queue->keep_new) {
            // drop oldest
            queue->outdex = next_index(queue, queue->outdex);
        }
        else {
            // full and set to not drop oldest
            return ERROR_OVERRUN;
        }
    }

    queue->buffer[queue->index] = val;
    queue->index = next_index(queue, queue->index);
    
    return SUCCESS;
}

queue_elem_t queue_pop(queue_ctx_t * const queue) {
    ASSERT(queue);
    
    if (queue_is_empty(queue)) {
        return queue->default_value;
    }
    
    queue_elem_t val = queue->buffer[queue->outdex];
    queue->outdex = next_index(queue, queue->outdex);
    
    return val;
    
}

void queue_flush(queue_ctx_t * const queue) {
    queue->index = 0;
    queue->outdex = 0;
}

static size_t next_index(queue_ctx_t const * const queue, size_t dex) {
    dex++;

    if (dex >= queue->capacity) {
        dex = 0;
    }
    
    return dex;
}
