#ifndef PT_BASE_H
#define PT_BASE_H

#include <stdbool.h>
#include <stdint.h>
#include "dllist.h"

typedef enum
{
    PT_NO_REQUEST,
    PT_LOCK,
    PT_UNLOCK,
    PT_LOCK_MAX
} pt_lock_request_t;

typedef struct
{
    char *   name;
    bool     is_valid;
    uint16_t seq_num;
    bool     is_locked;
    dllist_t suscribers;
} pt_base_t

#endif  // PT_BASE_H
