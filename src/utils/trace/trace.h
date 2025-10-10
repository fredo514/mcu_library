#ifndef TRACE_H
#define TRACE_H

#include <stdint.h>

typedef error_t (*encoder_cb_t)(uint8_t* const dest, size_t dest_len, uint8_t const* const src, size_t src_len);

typedef error_t (*sender_cb_t)(uint8_t const* const buffer, size_t len);

typedef struct trace trace_t;

typedef struct {
   encoder_cb_t encoder_cb;
   sender_cb_t sender_cb;
} trace_config_t;

void Trace_Init(trace_t* const inst, trace_config_t const* const config);

#endif