#ifndef PT_UINT32_H
#define PT_UINT32_H

#include "error.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    mp_subsciber_uint32_t observer;
    bool is_valid;
    uint32_t value;
    uint16_t seq_num;
} pt_uint32_t;

typedef struct {
    uint32_t initial_value;
} pt_uint32_config_t;

error_t pt_uint32_init(pt_uint32_t * const mp, pt_uint32_config_t const* const config);
bool pt_uint32_is_valid(pt_uint32_t const * const mp);

// optionally returns the current sequence number if not null
error_t pt_uint32_read(pt_uint32_t * const mp, uint32_t* const dest, uint16_t* const seq_num);

// returns the new sequence number
uint16_t pt_uint32_write(pt_uint32_t * const mp, uint32_t new_value);

// returns the current sequence number
uint16_t pt_uint32_set_invalid(pt_uint32_t * const mp);
bool pt_uint32_is_valid(pt_uint32_t const * const mp);

#endif