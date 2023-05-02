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

typedef I2C_CTX_t const * const I2C_h;

typedef struct {
    I2C_MODE_t mode;
    I2C_SPEED_t speed;
    uint16_t address;
} I2C_CONFIG_t;

ERROR_CODE_t I2c_Init(I2C_h i2c, I2C_CONFIG_t const * const config);
ERROR_CODE_t I2c_Callback_Register(I2C_h i2c, I2C_CALLBACK_t const function, void (*cb)(I2C_h i2c));  // really need this?

// Use these SPARINGLY to extend the API
ERROR_CODE_t I2c_Reg_Write (REG_SIZE_t * const address, uint32_t const val);
REG_SIZE_t I2c_Reg_Read (REG_SIZE_t * const address);

#endif // I2C_H