#ifndef STACK_H
#define	STACK_H

#include <stdbool.h>
#include <stdint.h>
#include "error.h"

// TODO: Make this a template?
typedef STACK_ITEM_t uint32_t;

typedef STACK_CTX const * const STACK_h;

ERROR_CODE_t Stack_Init(STACK_h stack);
ERROR_CODE_t Stack_Push(STACK_h stack, STACK_ITEM_t const val);
ERROR_CODE_t Stack_Pop(STACK_h stack, STACK_ITEM_t* const val);

#endif // STACK_H