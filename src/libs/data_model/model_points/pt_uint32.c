#include "pt_uint32.h"

#include <assert.h>

static bool test_and_update_lock (pt_uint32_t * const mp, pt_lock_request_t const lock);

error_t pt_uint32_read (pt_uint32_t * const mp, uint32_t * const dest, uint16_t * const seq_num)
{
    assert(mp);
    assert(dest);
    assert(seq_num);

    if (mp->base.is_valid)
    {
        *dest = mp->value;
    }

    if (seq_num)
    {
        *seq_num = mp->base.seq_num;
    }

    return 0;
}

uint16_t pt_uint32_write (pt_uint32_t * const mp, uint32_t const new_value, pt_lock_request_t const lock)
{
    assert(mp);
    assert(lock < PT_LOCK_MAX);

    if (test_and_update_lock(mp, lock))
    {
        if ((!mp->base.is_valid) || (mp->value != new_value))
        {
            mp->value         = new_value;
            mp->base.is_valid = true;
            mp->base.seq_num++;

            // TODO: walk suscriber list and notify
        }
    }

    return mp->base.seq_num;
}

bool pt_uint32_is_valid (pt_uint32_t const * const mp)
{
    return mp->base.is_valid;
}

static bool test_and_update_lock (pt_uint32_t * const mp, pt_lock_request_t const lock)
{
    bool result = false;

    switch (lock)
    {
        case PT_UNLOCK:
            mp->base.is_locked = false;
            result             = true;
            break;

        case PT_LOCK:
            mp->base.is_locked = true;
            result             = true;
            break;

        default:
            if (!mp->base.is_locked)
            {
                result = true;
            }
    }

    return result;
}
