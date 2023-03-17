#ifndef SPI_H
#define SPI_H

#include "stdint.h"
#include <stdbool.h>
#include "error.h"
#include "spi_def.h"	// contains platform-specific defines
#include "gpio.h"

typedef SPI_CTX * const SPI_h;

typedef struct {
    SPI_MASTER_MODE master_mode;
    SPI_MODE mode;
    SPI_ENDIAN endian;
    uint32_t speed_hz;
    bool custom_cs;
    GPIO_h * cs_pin;
    GPIO_STATE cs_idle;
} SPI_CONFIG;

ERROR_CODE Spi_Init(SPI_h spi, SPI_CONFIG const * const config);
ERROR_CODE Spi_Callback_Register(SPI_h spi, SPI_CALLBACK const function, void (*cb)(void));  // really need this?

ERROR_CODE Spi_Mode_Set(SPI_h spi, SPI_MODE const mode);

uint8_t Spi_Exchange(SPI_h spi, uint8_t const ch);

bool Spi_Is_Busy(SPI_h spi);

// Use these SPARINGLY to extend the API
ERROR_CODE Spi_Reg_Write (REG_SIZE * const address, uint32_t const val);
REG_SIZE Spi_Reg_Read (REG_SIZE * const address);

#endif // SPI_H