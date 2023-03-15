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
} GPIO_STATE;

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
} GPIO_PIN;

typedef GPIO_CTX const * const GPIO_h;

typedef struct {
    GPIO_MODE mode;
    GPIO_PULL pull;
	GPIO_DRIVE drive;
    GPIO_STATE init_state;
    GPIO_ALT_FUNCTION alt_function;
	GPIO_INT_MODE int_mode;
} GPIO_CONFIG;

ERROR_CODE Gpio_Init(GPIO_h gpio, GPIO_CONFIG const * const config);
ERROR_CODE Gpio_Callback_Register(GPIO_h gpio, GPIO_CALLBACK const function, void (*cb)(void));  // really need this?
ERROR_CODE Gpio_Mode_Set(GPIO_h gpio, GPIO_MODE const mode);

GPIO_STATE Gpio_Pin_Read(GPIO_h gpio);
ERROR_CODE Gpio_Pin_Set(GPIO_h gpio, GPIO_STATE const state);
ERROR_CODE Gpio_Pin_Set_Low(GPIO_h gpio);
ERROR_CODE Gpio_Pin_Set_High(GPIO_h gpio);
ERROR_CODE Gpio_Pin_Toggle(GPIO_h gpio);

bool Gpio_Is_Set_High(GPIO_h gpio);  // for output
bool Gpio_Is_Set_Low(GPIO_h gpio);  // for output
bool Gpio_Is_High(GPIO_h gpio);  // for input
bool Gpio_Is_Low(GPIO_h gpio);  // for input

// Use these SPARINGLY to extend the API
ERROR_CODE Gpio_Reg_Write (REG_SIZE const address, uint32_t const val);
REG_SIZE Gpio_Reg_Read (REG_SIZE const address);

#endif // GPIO_H