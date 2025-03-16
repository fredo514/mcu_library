#ifndef ISOTP_H
#define ISOTP_H

#include "error.h"
#include <stddef.h>
#include "can.h"

typedef enum {
    ISOTP_CB_TX_DONE,
    ISOTP_CB_RX_DONE,
    ISOTP_CB_TX_ABORT,
    ISOTP_CB_RX_ABORT,
    ISOTP_CB_ID_MAX
} isotp_cb_id_t;

typedef isotp_ctx_t * const isotp_h;

typedef struct {
    can_h interface;
    uint8_t * rx_buffer;
    size_t rx_buf_max_len;
    ERROR_CODE_t (*send)(can_frame_t frame);
} isotp_config_t;

isotp_h isotp_create(void);
ERROR_CODE_t isotp_init(isotp_h isotp, isotp_config_t const * const config);
void isotp_callback_attach(isotp_h isotp, isotp_cb_id_t const cb_id, void * cb);

ERROR_CODE_t isotp_buffer_send(isotp_h isotp, uint8_t const * const buffer, size_t const len);
ERROR_CODE_t isotp_frame_receive(isotp_h isotp, can_frame_t frame);

#endif // ISOTP_H