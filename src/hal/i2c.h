// TODO: Master blocking, master non-blocking and slave?

/* USAGE
// MASTER
//
//
// SLAVE
// Implement and register an address match callback that attach a buffer with the
// expectect transaction length when called

*/

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
    I2C_ADDRESS_LEN_7BIT,
    I2C_ADDRESS_LEN_10BIT,
    I2C_ADDRESS_LEN_MAX
} I2C_ADDRESS_LEN_t;

typedef enum {
    I2C_ADDRESS1_ONLY,
    I2C_ADDRESS1_AND_2,
    I2C_ADDRESS_MODE_MAX
} I2C_ADDRESS_MODE_t;

typedef enum {
    I2C_MASTER_TX_DONE_CALLBACK,
    I2C_MASTER_RX_DONE_CALLBACK,
    I2C_ABORT_DONE_CALLBACK,

    I2C_LISTEN_DONE_CALLBACK,      // what's this for? 
    I2C_ADDRESS_MATCH_CALLBACK,
    I2C_SLAVE_TX_DONE_CALLBACK,
    I2C_SLAVE_RX_DONE_CALLBACK,
    I2C_SLAVE_RX_ACK_CALLBACK,

    I2C_ERROR_CALLBACK,
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
    I2C_STATUS_INACTIVE,
    I2C_STATUS_READY,
    I2C_STATUS_BUSY_TX,
    I2C_STATUS_BUSY_RX,
    I2C_STATUS_DONE_RX,
    I2C_STATUS_DONE_TX,
    I2C_STATUS_ABORT,
    I2C_STATUS_TIMEOUT,
    I2C_STATUS_ERROR,
    I2C_STATUS_MAX
} I2C_STATE_t;

typedef enum {
    I2C_XFER_DIR_READ,
    I2C_XFER_DIR_WRITE,
    I2C_XFER_DIR_MAX
} I2C_XFER_DIR_t;

typedef struct I2C_CTX const * const I2C_h;

typedef struct {
    I2C_MODE_t mode;
    I2C_SPEED_t speed;
    I2C_ADDRESS_LEN_t address_len;
    I2C_ADDRESS_MODE_t address_mode;
    uint16_t slave_address;
#ifdef I2C_DUAL_ADDRESS
    uint16_t slave_address2;
#endif
} I2C_CONFIG_t;

I2C_h I2c_Create(I2C_TypeDef const * const instance);
ERROR_CODE_t I2c_Init(I2C_h i2c, I2C_CONFIG_t const * const config);
ERROR_CODE_t I2c_Callback_Register(I2C_h i2c, I2C_CALLBACK_ID_t const callback_id, void * cb);
I2C_STATE_t I2c_State_Get(I2C_h i2c);
uint8_t I2c_Receive_Count_Get(I2C_h i2c);
I2C_STATUS_t I2c_Buffer_Attach(I2C_h i2c, uint8_t const * const buffer_ptr, uint16_t len);

ERROR_CODE_t I2c_Master_Write(I2C_h i2c, uint16_t address, uint8_t const * const buffer_ptr, uint16_t len, I2C_XFER_OPTIONS_t options);
ERROR_CODE_t I2c_Master_Read(I2C_h i2c, uint16_t address, uint8_t const * const buffer_ptr, uint16_t len, I2C_XFER_OPTIONS_t options);
ERROR_CODE_t I2c_Master_Abort(I2C_h i2c);
ERROR_CODE_t I2c_Slave_Enable(I2C_h i2c);
ERROR_CODE_t I2c_Slave_Disable(I2C_h i2c);

// Use these SPARINGLY to extend the API
ERROR_CODE_t I2c_Reg_Write (REG_SIZE_t * const address, uint32_t const val);
REG_SIZE_t I2c_Reg_Read (REG_SIZE_t * const address);

#endif // I2C_H