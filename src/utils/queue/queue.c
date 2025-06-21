#include "queue.h"

#include "assert.h"

struct queue_ctx {
    void * buffer;
    size_t capacity;
    size_t elem_size;
    size_t index;
    size_t outdex;
    bool keep_new;
};

static size_t Next_Index(queue_h queue, size_t dex);

queue_h Queue_Create(size_t len, size_t item_size) {
    struct queue_ctx * queue_h inst = calloc(1, sizeof(queue_h));
    inst->buffer = calloc(len, item_size);
    inst->capacity = len;
    inst->elem_size = item_size;

    return inst;
}

error_t Queue_Init(queue_h queue, queue_init_t const * const config) {
    ASSERT(queue);
    ASSERT(config);
    ASSERT(config->overrun_mode < QUEUE_OVERRUN_MODE_MAX);

    Queue_Flush(queue);

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

    return SUCCESS;
}

bool Queue_Is_Full(queue_h queue) {
    ASSERT(queue);

    // keeping a free slot for this check allows concurency of 1 producer and many consumers
    // TODO: use full flag and mutex instead?
    size_t next_in = Next_Index(queue, queue->index);

    return (next_in == queue->outdex);
}

bool Queue_Is_Empty(queue_h queue) {
    ASSERT(queue);

    return (queue->index == queue->outdex);
}

error_t Queue_Push(queue_h queue, void const * const src_item) {
    ASSERT(queue);
    ASSERT(src_item);

    if (Queue_Is_Full(queue)) {
        if (queue->keep_new) {
            // drop oldest
            queue->outdex = Next_Index(queue, queue->outdex);
        }
        else {
            // full and set to not drop oldest
            return ERROR_OVERRUN;
        }
    }

    memcpy((uint8_t *)queue->buffer + (queue->index * queue->elem_size), item, queue->elem_size);
    queue->index = Next_Index(queue, queue->index);
    
    return SUCCESS;
}

error_t queue_pop(queue_h queue, void * const dest_item) {
    ASSERT(queue);
    ASSERT(dest_item);
    
    if (queue_is_empty(queue)) {
        memset((uint8_t *)dest_item, 0, queue->elem_size);
        
        return ERROR_NO_DATA;
    }

    memcpy(dest_item, (uint8_t *)queue->buffer + (queue->outdex * queue->elem_size), queue->elem_size);
    queue->outdex = next_index(queue, queue->outdex);
    
    return SUCCESS;
}

void queue_flush(queue_h queue) {
    queue->index = 0;
    queue->outdex = 0;
}

static size_t next_index(queue_h queue, size_t dex) {
    dex++;

    if (dex >= queue->capacity) {
        dex = 0;
    }
    
    return dex;
}
