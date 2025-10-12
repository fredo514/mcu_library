#ifndef TRACE_H
#define TRACE_H

#include <stdint.h>

#define TRACE_DICT_ENTRY(type, obj)          \
   do {                                      \
      static char_t const obj_name[] = #obj; \
      Trace_Begin(TRACE_TYPE_DICT_ENTRY);    \
      Trace_Log_Uint8(type);                 \
      Trace_Log_Uintptr(&obj);               \
      Trace_Log_Str(&obj_name[0]);           \
      Trace_End();                           \
   } while (0)

typedef enum {
   TRACE_TYPE_DICT_ENTRY,
   TRACE_TYPE_MAX,
} trace_type_t;

typedef enum {
   TRACE_DICT_ENTRY_FUNC,
   TRACE_DICT_ENTRY_VAR,
   TRACE_DICT_ENTRY_TYPE_MAX,
} trace_dict_entry_type_t;

typedef uint32_t (*trace_time_now_cb_t)(void);

typedef error_t (*trace_encode_cb_t)(uint8_t* const dest, size_t dest_len, uint8_t const* const src, size_t src_len);

typedef error_t (*trace_send_cb_t)(uint8_t const* const buffer, size_t len);

typedef struct trace trace_t;

typedef struct {
   trace_time_now_cb_t time_now_cb;
   trace_encode_cb_t encoder_cb;  // leave NULL to send raw input
   trace_send_cb_t sender_cb;     // leave NULL to use Trace_Byte_Get manually
} trace_config_t;

void Trace_Init(trace_t* const inst, trace_config_t const* const config);

error_t Trace_Byte_Get(trace_t const* const inst, uint8_t* const b);

#endif