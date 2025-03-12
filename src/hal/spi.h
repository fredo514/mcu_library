#ifndef SPI_H
#define SPI_H

// TODO: Master blocking, master non-blocking and slave?

#include "stdint.h"
#include <stdbool.h>
#include "error.h"
#include "spi_def.h"	// contains platform-specific defines
#include "gpio.h"

typedef enum {
    SPI_CALLBACK_XFER_DONE,
    SPI_CALLBACK_ID_MAX
} SPI_CALLBACK_ID_t;

typedef enum {
    SPI_STATUS_IDLE,
    SPI_STATUS_BUSY,
    SPI_STATUS_ERROR,
    SPI_STATUS_MAX
} SPI_STATUS_t

typedef SPI_CTX_t * const SPI_h;

typedef struct {
    SPI_MASTER_MODE_t master_mode;
    SPI_MODE_t mode;
    SPI_ENDIAN_t endian;
    uint32_t speed_hz;
    bool custom_cs;
    GPIO_h * cs_pin;
    GPIO_STATE cs_idle;
} SPI_CONFIG_t;

SPI_h Spi_Create(SPI_TypeDef const * const instance);
ERROR_CODE_t Spi_Init(SPI_h spi, SPI_CONFIG_t const * const config);
ERROR_CODE_t Spi_Callback_Register(SPI_h spi, SPI_CALLBACK_ID_t const function, void (*cb)(void));
ERROR_CODE_t Spi_Buffer_Attach(SPI_h spi, uint8_t const * const buf);
ERROR_CODE_t Spi_Mode_Set(SPI_h spi, SPI_MODE_t const mode);
SPI_STATUS_t Spi_Status_Get(SPI_h spi);

uint8_t Spi_ReadWrite(SPI_h spi, uint8_t const tx_data);
ERROR_CODE_t Spi_Buffer_ReadWrite(SPI_h spi, uint8_t const * const tx_buf, uint8_t const * const rx_buf, uint8_t len);

// Use these SPARINGLY to extend the API
ERROR_CODE_t Spi_Reg_Write(REG_SIZE_t * const address, uint32_t const val);
REG_SIZE_t Spi_Reg_Read(REG_SIZE_t * const address);

#endif // SPI_H