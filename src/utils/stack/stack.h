#ifndef STACK_H
#define	STACK_H

#include <stdbool.h>
#include <stdint.h>
#include "error.h"

typedef STACK_ITEM_t uint32_t;
typedef STACK_CTX_t* STACK_h;

ERROR_CODE_t Stack_Init(STACK_h stack);
ERROR_CODE_t Stack_Push(STACK_h stack, STACK_ITEM_t val);
ERROR_CODE_t Stack_Pop(STACK_h stack, STACK_ITEM_t* val);

#endif // STACK_H