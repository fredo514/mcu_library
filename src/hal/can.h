#ifndef CAN_H
#define CAN_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "error.h"
#include "platform.h"

typedef enum {
    CAN_STATE_UNINIT,
    CAN_STATE_IDLE,
    CAN_STATE_BUSY,
    CAN_STATE_ERROR,
    CAN_STATE_MAX
} can_state_t;

typedef enum {
    CAN_ID_STANDARD,
    CAN_ID_EXTENDED,
    CAN_ID_TYPE_MAX
} can_id_type_t;

typedef enum {
    CAN_100KBAUD = 100000,
    CAN_125KBAUD = 125000,
    CAN_250KBAUD = 250000,
    CAN_500KBAUD = 500000,
    CAN_1MBAUD   = 1000000,
    CAN_BAUDRATE_MAX
} can_baudrate_t;

typedef enum {
    CAN_FILTER_ACCEPT,
    CAN_FILTER_REJECT,
    CAN_FILTER_DIR_MAX
} can_filter_dir_t;

typedef enum {
    CAN_FILTER_MASK,
    CAN_FILTER_DUAL_ID,
    CAN_FILTER_RANGE,
    CAN_FILTER_MODE_MAX
} can_filter_mode_t;

typedef struct can_ctx * const can_h;

typedef struct {
    can_baudrate_t baudrate;
} can_config_t;

typedef struct {
    uint32_t id;
    can_id_type_t id_type;
    size_t payload_len;
    uint8_t * payload_buffer;
    // hit_filter
    // overflow
    // rtr
} can_frame_t;

typedef struct {
    uint32_t filter_id1_rangeStart;
    uint32_t mask_id2_rangeEnd;
    can_id_type_t id_type;
    can_filter_dir_t dir;
    can_filter_mode_t mode;
} can_filter_config_t;

can_h Can_Create(can_regs_t const * const regs);
ERROR_CODE_t Can_Init(can_h can, can_config_t const * const config);
ERROR_CODE_t Can_Callback_Register(can_h can, can_callback_id_t const callback_id, void * cb);
can_state_t Can_State_Get(can_h can);

void Can_Filter_Config(can_h can, can_filter_config_t const * const config);
ERROR_CODE_t Can_Filter_Enable(can_h can, can_filter_id_t const filter_id);
ERROR_CODE_t Can_Filter_Disable(can_h can, can_filter_id_t const filter_id);

void Can_Start(can_h can);
ERROR_CODE_t Can_Stop(can_h can);

bool Can_Is_Frame_Available(can_h can);
bool Can_Is_Tx_Full(can_h can);

ERROR_CODE_t Can_Frame_Write(can_h can, can_frame_t const * const frame);
ERROR_CODE_t Can_Frame_Get(can_h can, can_frame_t * const frame);

#endif // CAN_H