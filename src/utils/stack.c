#include "stack.h"

// TODO: make it atomic (thread-safe)

#define MAX_STACK_SIZE 128

typedef struct {
    size_t index;
    size_t size;
    STACK_ITEM_t array[];
} STACK_CTX_t;

ERROR_CODE_t Stack_Init(STACK_h stack, size_t size) {
    ERROR_CODE_t ret = ERROR;

    if (size <= MAX_STACK_SIZE) {
        stack = malloc(sizeof(STACK_CTX_t) + (size-1)*sizeof(STACK_ITEM_t));

        if (stack != 0) {
            stack->index = 0;
            stack->size = size;
            ret = SUCCESS;
        }
    }

    return ret;
}

ERROR_CODE_t Stack_Push(STACK_h stack, STACK_ITEM_t val) {
    ERROR_CODE_t ret = ERROR;

    if (stack->index < stack->size) {
        stack->array[stack->index] = val;
        stack->index++;
        ret = SUCCESS;
    }

    return ret;
}

ERROR_CODE_t Stack_Pop(STACK_h stack, STACK_ITEM_t* val) {
    ERROR_CODE_t ret = ERROR;

    if (stack->index > 0) {
        stack->index--;
        *val = stack->array[stack->index];
        ret = SUCCESS;
    }

    return ret;
}