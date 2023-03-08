#ifndef STACK_H
#define	STACK_H

#include <stdbool.h>
#include <stdint.h>
#include "error.h"

typedef STACK_ITEM uint32_t;
typedef STACK_CTX* STACK_h;

ERROR_CODE Stack_Init(STACK_h stack);
ERROR_CODE Stack_Init(STACK_h stack);
 Stack_Push(STACK_h stack, STACK_ITEM val);
ERROR_CODE Stack_Init(STACK_h stack);
 Stack_Pop(STACK_h stack, STACK_ITEM* val);

#endif // STACK_H