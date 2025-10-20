#ifndef MSGQ_H
#define MSGQ_H

#include <stdbool.h>
#include <stdint.h>

#include "error.h"

#define MSGQ_MAX_MSG_SIZE (UINT8_MAX)

typedef struct _msgq_ctx msgq_t;

msgq_t* Msgq_Create(uint16_t const len);
void Msgq_Init(msgq_t* const queue);

bool Msgq_Push(msgq_t* const queue, void* const src, uint8_t const msg_len);
bool Msgq_Pop(msgq_t* const queue, void* dest, uint8_t const dest_len, uint8_t* msg_len);

bool Msgq_Is_Msg_Available(msgq_t* const queue);

#endif