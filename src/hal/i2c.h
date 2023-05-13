// TODO: Master blocking, master non-blocking and slave?

#ifndef I2C_H
#define I2C_H

#include "stdint.h"
#include "error.h"
#include "platform.h"

typedef enum {
    I2C_MODE_MASTER,
    I2C_MODE_SLAVE
} I2C_MODE_t;

typedef enum {
    I2C_ADDRESS_MODE_7BIT,
    I2C_ADDRESS_MODE_10BIT
} I2C_ADDRESS_MODE_t;

typedef enum {
    I2C_MASTER_TX_DONE_CALLBACK,
    I2C_MASTER_RX_DONE_CALLBACK,
    I2C_SLAVE_TX_DONE_CALLBACK,
    I2C_SLAVE_RX_DONE_CALLBACK,
    I2C_LISTEN_DONE_CALLBACK,
    I2C_ADDRESS_MATCH_CALLBACK,
    I2C_ERROR_CALLBACK,
    I2C_ABORT_DONE_CALLBACK,
    I2C_CB_ID_MAX
} I2C_CALLBACK_ID_t;

typedef enum {
    I2C_XFER_OPTION_NONE = 0,
    I2C_FIRST_FRAME = 1<<0,
    I2C_FIRST_AND_NEXT_FRAME = 1<<1,
    I2C_NEXT_FRAME = 1<<2,
    I2C_FIRST_AND_LAST_FRAME = 1<<3,
    I2C_LAST_FRAME = 1<<4,
    I2C_LAST_FRAME_NO_STOP = 1<<5,
    I2C_OTHER_FRAME = 1<<6,
    I2C_OTHER_AND_LAST_FRAME = 1<<7,
    I2C_XFER_OPTIONS_MAX
} I2C_XFER_OPTIONS_t;

typedef enum {
    I2C_STATUS_RESET,
    I2C_STATUS_READY,
    I2C_STATUS_BUSY,
    I2C_STATUS_BUSY_TX,
    I2C_STATUS_BUSY_RX,
    I2C_STATUS_LISTEN,
    I2C_STATUS_BUSY_TX_LISTEN,
    I2C_STATUS_BUSY_RX_LISTEN,
    I2C_STATUS_ABORT,
    I2C_STATUS_TIMEOUT,
    I2C_STATUS_ERROR,
    I2C_STATUS_MAX
} I2C_STATUS_t;

typedef struct I2C_CTX const * const I2C_h;

typedef struct {
    I2C_MODE_t mode;
    I2C_SPEED_t speed;
    uint16_t address;
#ifdef I2C_DUAL_ADDRESS
    uint16_t address2;
#endif
    I2C_ADDRESS_MODE_t address_mode;
} I2C_CONFIG_t;

I2C_h I2c_Create(I2C_TypeDef const * const instance);
ERROR_CODE_t I2c_Init(I2C_h i2c, I2C_CONFIG_t const * const config);
ERROR_CODE_t I2c_Callback_Register(I2C_h i2c, I2C_CALLBACK_ID_t const callback_id, void (*cb)(I2C_h i2c));
I2C_STATUS_t I2c_Status_Get(I2C_h i2c);

ERROR_CODE_t I2c_Master_Write(I2C_h i2c, uint16_t address, uint8_t const * const buffer_ptr, uint16_t len, I2C_XFER_OPTIONS_t options);
ERROR_CODE_t I2c_Master_Read(I2C_h i2c, uint16_t address, uint8_t const * const buffer_ptr, uint16_t len, I2C_XFER_OPTIONS_t options);
ERROR_CODE_t I2c_Master_Abort(I2C_h i2c);
ERROR_CODE_t I2c_Slave_Enable(I2C_h i2c);
ERROR_CODE_t I2c_Slave_Disable(I2C_h i2c);

// Use these SPARINGLY to extend the API
ERROR_CODE_t I2c_Reg_Write (REG_SIZE_t * const address, uint32_t const val);
REG_SIZE_t I2c_Reg_Read (REG_SIZE_t * const address);

#endif // I2C_H