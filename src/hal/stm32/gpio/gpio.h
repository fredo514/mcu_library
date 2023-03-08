#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include "stdint.h"
#include <stdbool.h>
#include "error.h"

typedef enum {
    GPIO_LOW = 0;
    GPIO_HIGH = 1;
} GPIO_STATE;

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
	GPIO_PIN_15 = 15
} GPIO_PIN;

typedef enum {
	GPIO_MODE_INPUT              =  0x0UL << 0U,                                                  // Input Floating Mode
	GPIO_MODE_OUTPUT		     =  0x1UL,
	GPIO_MODE_OUTPUT_PP          = (0x1UL << 0U | (0x0UL << 4U)),                                 // Output Push Pull Mode
	GPIO_MODE_OUTPUT_OD          = (0x1UL << 0U | (0x1UL << 4U)),                                 // Output Open Drain Mode
	GPIO_MODE_AF_PP              = (0x2UL << 0U | (0x0UL << 4U)),                                 // Alternate Function Push Pull Mode
	GPIO_MODE_AF_OD              = (0x2UL << 0U | (0x1UL << 4U)),                                 // Alternate Function Open Drain Mode

	GPIO_MODE_ANALOG             = 0x3UL << 0U,                                                	 // Analog Mode
	
    // Interrupt both send signals and trigger interrupt
	GPIO_MODE_IT_RISING          = (0x0UL << 0U | (0x1UL << 16U) | (0x1UL << 20U)),               // Rising edge trigger External Interrupt      
	GPIO_MODE_IT_FALLING         = (0x0UL << 0U | (0x1UL << 16U) | (0x2UL << 20U)),               // Falling edge trigger External Interrupt    
	GPIO_MODE_IT_RISING_FALLING  = (0x0UL << 0U | (0x1UL << 16U) | (0x1UL << 20U) | (0x2UL << 20U)),   // Rising/Falling edge trigger External Interrupt
	
    // Events send signals but don't trigger interrupt
	GPIO_MODE_EVT_RISING         = (0x0UL << 0U | (0x2UL << 16U) | (0x1UL << 20U)),                    // Rising edge trigger External Event Mode
	GPIO_MODE_EVT_FALLING        = (0x0UL << 0U | (0x2UL << 16U) | (0x2UL << 20U)),                    // Falling edge trigger External Event Mode
	GPIO_MODE_EVT_RISING_FALLING = (0x0UL << 0U | (0x2UL << 16U) | (0x1UL << 20U) | (0x2UL << 20U))    // Rising/Falling edge trigger External Event Mode
} GPIO_MODE;

typedef enum {
	GPIO_NOPULL     = 0x0U,
	GPIO_PULLUP     = 0x1U,
	GPIO_PULLDOWN   = 0x2U
} GPIO_PULL;

typedef enum {
	// see datasheet for part and STM32CUBE hal
} GPIO_ALT_FUNCTION;

typedef GPIO_CTX* const GPIO_h;

typedef struct {
    GPIO_h gpio;
    GPIO_MODE mode;
    GPIO_PULL pull;
    GPIO_STATE intial_state;
    GPIO_ALT_FUNCTION alt_function;
} GPIO_CONFIG;

ERROR_CODE Gpio_Init(const GPIO_CONFIG* const config);
ERROR_CODE Gpio_Mode_Set(GPIO_h gpio, GPIO_MODE mode);
ERROR_CODE Gpio_Dir_Set(GPIO_h gpio, GPIO_DIR dir);
ERROR_CODE Gpio_Callback_Register(GPIO_h gpio, GPIO_CALLBACK function, void (*cb)(void));  // really need this?

GPIO_STATE Gpio_Read(const GPIO_h gpio);

ERROR_CODE Gpio_Set(GPIO_h gpio, GPIO_STATE state);
ERROR_CODE Gpio_Set_Low(GPIO_h gpio);
ERROR_CODE Gpio_Set_High(GPIO_h gpio);
ERROR_CODE Gpio_Toggle(GPIO_h gpio);

bool Gpio_Is_Set_High(GPIO_h gpio);  // for output
bool Gpio_Is_Set_Low(GPIO_h gpio);  // for output
bool Gpio_Is_High(GPIO_h gpio);  // for input
bool Gpio_Is_Low(GPIO_h gpio);  // for input

// Use these SPARINGLY to extend the API
ERROR_CODE Gpio_Reg_Write (uintptr address, uint32_t val);
uint32_t Gpio_Reg_Read (uintptr address);

#endif // HAL_GPIO_H