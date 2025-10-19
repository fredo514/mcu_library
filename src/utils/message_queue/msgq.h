#ifndef MSGQ_H
#define MSGQ_H

#include <stdbool.h>
#include <stdint.h>

#include "error.h"

typedef struct _msgq_ctx msgq_t;

msgq_t* Msgq_Create(uint16_t const len);
void Msgq_Init(msgq_t* const queue);

bool Msgq_Push(msgq_t* const queue, void* const src, uint16_t const src_len);
bool Msgq_Pop(msgq_t* const queue, void* dest, uint16_t const dest_len, uint16_t* out_len);

#endif