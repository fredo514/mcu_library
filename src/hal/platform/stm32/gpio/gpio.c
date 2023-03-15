#include "gpio.h"
#include "stm32_hal.h"
#include "assert.h"

typedef struct
{
    GPIO_PORT * const port;
    GPIO_PIN const pin;
} GPIO_CTX;

ERROR_CODE Gpio_Init(GPIO_h gpio, GPIO_CONFIG const * const config) {
    ASSERT(config != 0);   // config exists
    ASSERT(gpio != 0);   // gpio exists
    ASSERT((gpio->pin >= 0) && (gpio->pin < 16));   // only 16 pins per port

    // enable clock for gpio port
}

GPIO_STATE Gpio_Read(GPIO_h gpio)
{
	ASSERT_DEBUG(gpio != 0);   // gpio exists
    ASSERT_DEBUG((gpio->pin >= 0) && (gpio->pin < 16));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as input?
    
    // if pin'th bit of the port's IDR register is set, then return SET. Otherwise, return RESET
    // Done using ternary operator because it generates the most efficient assembly
    return (gpio->port->IDR & (1 << gpio->pin)) ? GPIO_SET : GPIO_RESET;
}

ERROR_CODE Gpio_Set(GPIO_h gpio, GPIO_STATE const state)
{
	ASSERT_DEBUG(gpio != 0);   // gpio exists
    ASSERT_DEBUG((gpio->pin >= 0) && (gpio->pin < 16));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as output?

    ERROR_CODE ret = ERROR;

    // Access both the set and reset bits of the BSRR register at the same time
    // with the reset bit using the invert of the requested state
    // Done this way because it generates the most efficient assembly
    gpio->port->BSRR = ((REG_SIZE)state << gpio->pin) << 16) | (((REG_SIZE)!state) << gpio->pin);
    ret = SUCCESS;

    return ret;
}

ERROR_CODE Gpio_Set_High(GPIO_h gpio)
{
	ASSERT_DEBUG(gpio != 0);   // gpio exists
    ASSERT_DEBUG((gpio->pin >= 0) && (gpio->pin < 16));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as output?

    ERROR_CODE ret = ERROR;

    gpio->port->BSRR = 1 << gpio->pin;
    ret = SUCCESS;

    return ret;
}

ERROR_CODE Gpio_Set_Low(GPIO_h gpio)
{
	ASSERT_DEBUG(gpio != 0);   // gpio exists
    ASSERT_DEBUG((gpio->pin >= 0) && (gpio->pin < 16));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as output?

    ERROR_CODE ret = ERROR;

    gpio->port->BRR = 1 << gpio->pin;
    ret = SUCCESS;

    return ret;
}

ERROR_CODE Gpio_Toggle(GPIO_h gpio)
{
	ASSERT_DEBUG(gpio != 0);   // gpio exists
    ASSERT_DEBUG((gpio->pin >= 0) && (gpio->pin < 16));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as output?

    ERROR_CODE ret = ERROR;

    // Access both the set and reset bits of the BSRR register at the same time
    // with the reset bit using the invert of the ODR register current state
    // Done this way because it generates the most efficient assembly
	gpio->port->BSRR = ((gpio->port->ODR & (1 << gpio->pin)) << 16) | (~(gpio->port->ODR) & (1 << gpio->pin));
    ret = SUCCESS;

    return ret;
}

ERROR_CODE Gpio_Reg_Write (REG_SIZE const address, uint32_t const val) {
    *((REG_SIZE * const)address) = val;
}

REG_SIZE Gpio_Reg_Read (REG_SIZE const address) {
    return *((REG_SIZE * const)address);
}