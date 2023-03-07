#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include "stdint.h"

typedef enum {
    GPIO_LOW = 0;
    GPIO_HIGH = 1;
} gpio_state;

typedef gpio_ctx* gpio_h;

typedef struct {
    gpio_h pin;
    gpio_mode mode;
} gpio_config;

void Gpio_Init(const gpio_config* const config);
gpio_state Gpio_Read(gpio_h pin);
void Gpio_Set(gpio_h pin, gpio_state state);
void Gpio_Toggle(gpio_h pin);
void Gpio_Mode_Set(gpio_h pin, gpio_mode mode);
void Gpio_Dir_Set(gpio_h pin, gpio_dir dir);
void Gpio_Callback_Register(gpio_callback function, void (*cb)(void));

void Gpio_Reg_Write (uintptr address, uint32_t val);
uint32_t Gpio_Reg_Read (uintptr address);

#endif // HAL_GPIO_H