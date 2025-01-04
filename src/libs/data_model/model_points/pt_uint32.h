#ifndef PT_UINT32_H
#define PT_UINT32_H

#include "error.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    PT_NO_REQUEST,
    PT_LOCK,
    PT_UNLOCK,
    PT_LOCK_MAX
} pt_lock_request_t;

typedef struct {
    char * name;
    bool is_valid;
    uint32_t value;
    uint16_t seq_num;
    mp_subsciber_uint32_t observer;
} pt_uint32_t;

typedef struct {
    char const * const name;
    uint32_t initial_value;
} pt_uint32_config_t;

error_t pt_uint32_init(pt_uint32_t * const mp, pt_uint32_config_t const* const config);

error_t pt_uint32_observer_attach(pt_uint32_t * const mp, mp_subsciber_uint32_t const * const observer, uint16_t initial_seq_num);
error_t pt_uint32_observer_detach(pt_uint32_t * const mp, mp_subsciber_uint32_t const * const observer);

// triggers change notification, returns the current sequence number
uint16_t pt_uint32_touch(pt_uint32_t const * const mp)

// returns success if read value is valid, returns the current sequence number through parameter if not null
error_t pt_uint32_read(pt_uint32_t * const mp, uint32_t* const dest, uint16_t* const seq_num);

// returns the new sequence number
uint16_t pt_uint32_write(pt_uint32_t * const mp, uint32_t const new_value, pt_lock_request_t const lock);
// rear-modify-write of value by count, returns the new sequence number
uint16_t pt_uint32_increment(pt_uint32_t * const mp, uint32_t const count, pt_lock_request_t const lock);
uint16_t pt_uint32_decrement(pt_uint32_t * const mp, uint32_t const count, pt_lock_request_t const lock);

// returns the current sequence number
uint16_t pt_uint32_set_invalid(pt_uint32_t * const mp, pt_lock_request_t const lock);
bool pt_uint32_is_valid(pt_uint32_t const * const mp);
uint16_t pt_uint32_lock_set(pt_uint32_t * const mp, pt_lock_request_t const lock);

char * pt_uint32_name_get(pt_uint32_t const * const mp);
error_t pt_uint32_to_json(pt_uint32_t const * const mp, char * dest, size_t dest_size, bool * const truncated);
error_t pt_uint32_from_json(pt_uint32_t const * const mp, char const * const src, uint16_t * const seq_num);

#endif