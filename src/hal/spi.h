#ifndef SPI_H
#define SPI_H

#include "stdint.h"
#include <stdbool.h>
#include "error.h"
#include "spi_def.h"	// contains platform-specific defines
#include "gpio.h"

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

ERROR_CODE_t Spi_Init(SPI_h spi, SPI_CONFIG_t const * const config);
ERROR_CODE_t Spi_Callback_Register(SPI_h spi, SPI_CALLBACK_t const function, void (*cb)(void));  // really need this?

ERROR_CODE_t Spi_Mode_Set(SPI_h spi, SPI_MODE_t const mode);

uint8_t Spi_Exchange(SPI_h spi, uint8_t const ch);

// TODO: non-blocking API

bool Spi_Is_Busy(SPI_h spi);

// Use these SPARINGLY to extend the API
ERROR_CODE_t Spi_Reg_Write (REG_SIZE_t * const address, uint32_t const val);
REG_SIZE_t Spi_Reg_Read (REG_SIZE_t * const address);

#endif // SPI_H