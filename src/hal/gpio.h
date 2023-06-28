#ifndef GPIO_H
#define GPIO_H

#include "stdint.h"
#include <stdbool.h>
#include "error.h"
#include "gpio_def.h"	// contains platform-specific defines

typedef enum {
	GPIO_LOW = 0,
	GPIO_HIGH = 1,
	GPIO_STATE_MAX
} GPIO_STATE_t;

typedef enum {
	GPIO_PIN_0 = 0,
	GPIO_PIN_1 = 1,
	GPIO_PIN_2 = 2,
	GPIO_PIN_3 = 3,
	GPIO_PIN_4 = 4,
	GPIO_PIN_5 = 5,
	GPIO_PIN_6 = 6,
	GPIO_PIN_7 = 7,
	GPIO_PIN_8 = 8,
	GPIO_PIN_9 = 9,
	GPIO_PIN_10 = 10,
	GPIO_PIN_11 = 11,
	GPIO_PIN_12 = 12,
	GPIO_PIN_13 = 13,
	GPIO_PIN_14 = 14,
	GPIO_PIN_15 = 15,
	GPIO_PIN_MAX
} GPIO_PIN_t;

typedef GPIO_CTX_t const * const GPIO_h;

typedef struct {
    GPIO_h gpio;
	GPIO_MODE_t mode;
	GPIO_STATE_t init_state;
	GPIO_ALT_FUNCTION_t alt_function;
    GPIO_PULL_t pull;
	GPIO_SPEED_t speed;
} GPIO_CONFIG_t;

GPIO_h Gpio_Create(GPIO_PORT_t const * const port, GPIO_PIN_t const pin);
ERROR_CODE_t Gpio_Init(GPIO_CONFIG_t const * const config);
ERROR_CODE_t Gpio_Callback_Register(GPIO_h gpio, GPIO_CALLBACK_t const function, void (*cb)(void));  // really need this?
ERROR_CODE_t Gpio_Mode_Set(GPIO_h gpio, GPIO_MODE_t const mode);

GPIO_STATE_t Gpio_Read(GPIO_h gpio);
ERROR_CODE_t Gpio_Set(GPIO_h gpio, GPIO_STATE_t const state);
// ERROR_CODE Gpio_Set_Low(GPIO_h gpio);
// ERROR_CODE Gpio_Set_High(GPIO_h gpio);
ERROR_CODE_t Gpio_Toggle(GPIO_h gpio);

// bool Gpio_Is_Set_High(GPIO_h gpio);  // for output
// bool Gpio_Is_Set_Low(GPIO_h gpio);  // for output
bool Gpio_Is_High(GPIO_h gpio);  // for input
bool Gpio_Is_Low(GPIO_h gpio);  // for input

// Use these SPARINGLY to extend the API
ERROR_CODE_t Gpio_Reg_Write (REG_SIZE_t const address, uint32_t const val);
REG_SIZE_t Gpio_Reg_Read (REG_SIZE_t const address);

#endif // GPIO_H