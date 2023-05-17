#ifndef GPIO_H
#define GPIO_H

#include "stdint.h"
#include <stdbool.h>
#include "error.h"
#include "gpio_def.h"	// contains platform-specific defines

typedef enum {
	LOW = 0,
	HIGH = 1,
	GPIO_STATE_MAX
} GPIO_STATE_t;

typedef enum {
	PIN_0 = 0,
	PIN_1 = 1,
	PIN_2 = 2,
	PIN_3 = 3,
	PIN_4 = 4,
	PIN_5 = 5,
	PIN_6 = 6,
	PIN_7 = 7,
	PIN_8 = 8,
	PIN_9 = 9,
	PIN_10 = 10,
	PIN_11 = 11,
	PIN_12 = 12,
	PIN_13 = 13,
	PIN_14 = 14,
	PIN_15 = 15,
	GPIO_PIN_MAX
} GPIO_PIN_t;

typedef GPIO_CTX_t const * const GPIO_h;

typedef struct {
    GPIO_MODE_t mode;
	GPIO_STATE_t init_state;
	GPIO_ALT_FUNCTION_t alt_function;
    GPIO_PULL_t pull;
	GPIO_SPEED_t speed;
} GPIO_CONFIG_t;

GPIO_h Gpio_Create(GPIO_PORT_t const * const port, GPIO_PIN_t const pin);
ERROR_CODE_t Gpio_Init(GPIO_h gpio, GPIO_CONFIG_t const * const config);
ERROR_CODE_t Gpio_Callback_Register(GPIO_h gpio, GPIO_CALLBACK_t const function, void (*cb)(void));  // really need this?
ERROR_CODE_t Gpio_Mode_Set(GPIO_h gpio, GPIO_MODE_t const mode);

GPIO_STATE_t Gpio_Pin_Read(GPIO_h gpio);
ERROR_CODE_t Gpio_Pin_Set(GPIO_h gpio, GPIO_STATE_t const state);
// ERROR_CODE Gpio_Pin_Set_Low(GPIO_h gpio);
// ERROR_CODE Gpio_Pin_Set_High(GPIO_h gpio);
ERROR_CODE_t Gpio_Pin_Toggle(GPIO_h gpio);

// bool Gpio_Is_Set_High(GPIO_h gpio);  // for output
// bool Gpio_Is_Set_Low(GPIO_h gpio);  // for output
bool Gpio_Is_High(GPIO_h gpio);  // for input
bool Gpio_Is_Low(GPIO_h gpio);  // for input

// Use these SPARINGLY to extend the API
ERROR_CODE_t Gpio_Reg_Write (REG_SIZE_t const address, uint32_t const val);
REG_SIZE_t Gpio_Reg_Read (REG_SIZE_t const address);

#endif // GPIO_H