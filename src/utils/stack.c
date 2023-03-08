#include "stack.h"

#define MAX_STACK_SIZE 128

typedef struct {
    size_t index;
    size_t size;
    STACK_ITEM array[];
} STACK_CTX;

ERROR_CODE Stack_Init(STACK_h stack, size_t size) {
    ERROR_CODE ret = ERROR;

    if (size <= MAX_STACK_SIZE) {
        stack = malloc(sizeof(STACK_CTX) + (size-1)*sizeof(STACK_ITEM));

        if (stack != 0) {
            stack->index = 0;
            stack->size = size;
            ret = SUCCESS;
        }
    }

    return ret;
}

ERROR_CODE Stack_Push(STACK_h stack, STACK_ITEM val) {
    ERROR_CODE ret = ERROR;

    if (stack->index < stack->size) {
        stack->array[stack->index] = val;
        stack->index++;
        ret = SUCCESS;
    }

    return ret;
}

ERROR_CODE Stack_Pop(STACK_h stack, STACK_ITEM* val) {
    ERROR_CODE ret = ERROR;

    if (stack->index > 0) {
        stack->index--;
        *val = stack->array[stack->index];
        ret = SUCCESS;
    }

    return ret;
}