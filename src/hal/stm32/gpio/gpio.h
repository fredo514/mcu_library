#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include "stdint.h"
#include <stdbool.h>
#include "error.h"

typedef enum {
    GPIO_LOW = 0;
    GPIO_HIGH = 1;
} GPIO_STATE;

typedef GPIO_CTX* GPIO_h;

typedef struct {
    GPIO_h pin;
    GPIO_MODE mode;
} GPIO_CONFIG;

ERROR_CODE Gpio_Init(const GPIO_CONFIG* const config);
ERROR_CODE Gpio_Mode_Set(GPIO_h pin, GPIO_MODE mode);
ERROR_CODE Gpio_Dir_Set(GPIO_h pin, GPIO_DIR dir);
ERROR_CODE Gpio_Callback_Register(GPIO_h pin, GPIO_CALLBACK function, void (*cb)(void));  // really need this?

GPIO_STATE Gpio_Read(const GPIO_h pin);

ERROR_CODE Gpio_Set(GPIO_h pin, GPIO_STATE state);
ERROR_CODE Gpio_Set_Low(GPIO_h pin);
ERROR_CODE Gpio_Set_High(GPIO_h pin);
ERROR_CODE Gpio_Toggle(GPIO_h pin);

bool Gpio_Is_Set_High(GPIO_h pin);  // for output
bool Gpio_Is_Set_Low(GPIO_h pin);  // for output
bool Gpio_Is_High(GPIO_h pin);  // for input
bool Gpio_Is_Low(GPIO_h pin);  // for input

// Use these SPARINGLY to extend the API
ERROR_CODE Gpio_Reg_Write (uintptr address, uint32_t val);
uint32_t Gpio_Reg_Read (uintptr address);

#endif // HAL_GPIO_H