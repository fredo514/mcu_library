#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include "stdint.h"

typedef enum {
    GPIO_LOW = 0;
    GPIO_HIGH = 1;
} GPIO_STATE;

typedef GPIO_CTX* GPIO_h;

typedef struct {
    GPIO_h pin;
    GPIO_MODE mode;
} GPIO_CONFIG;

void Gpio_Init(const GPIO_CONFIG* const config);
GPIO_STATE Gpio_Read(const GPIO_h pin);
void Gpio_Set(GPIO_h pin, GPIO_STATE state);
void Gpio_Toggle(GPIO_h pin);
void Gpio_Mode_Set(GPIO_h pin, GPIO_MODE mode);
void Gpio_Dir_Set(GPIO_h pin, GPIO_DIR dir);
void Gpio_Callback_Register(GPIO_h pin, GPIO_CALLBACK function, void (*cb)(void));

// Use these to extend the API
void Gpio_Reg_Write (uintptr address, uint32_t val);
uint32_t Gpio_Reg_Read (uintptr address);

#endif // HAL_GPIO_H