#include "gpio.h"
#include "stm32_hal.h"
#include "assert.h"

typedef struct
{
    GPIO_TypeDef* port;
	GPIO_PIN pin;
} GPIO_CTX;

GPIO_STATE Gpio_Read(GPIO_h gpio)
{
	assert(gpio != 0);   // gpio exists
    assert((gpio->pin >= 0) && (gpio->pin < 16));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as input?
    
    // if pin'th bit of the port's IDR register is set, then return SET. Otherwise, return RESET
    // Done using ternary operator because it generates the most efficient assembly
    return (gpio->port->IDR & (1 << gpio->pin)) ? GPIO_SET : GPIO_RESET;
}

ERROR_CODE Gpio_Set(GPIO_h gpio, GPIO_STATE state)
{
	assert(gpio != 0);   // gpio exists
    assert((gpio->pin >= 0) && (gpio->pin < 16));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as output?

    ERROR_CODE ret = ERROR;

    // Access both the set and reset bits of the BSRR register at the same time
    // with the reset bit using the invert of the requested state
    // Done this way because it generates the most efficient assembly
    gpio->port->BSRR = ((uint32_t)state << gpio->pin) << 16) | (((uint32_t)!state) << gpio->pin);
    ret = SUCCESS;

    return ret;
}

ERROR_CODE Gpio_Set_High(GPIO_h gpio)
{
	assert(gpio != 0);   // gpio exists
    assert((gpio->pin >= 0) && (gpio->pin < 16));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as output?

    ERROR_CODE ret = ERROR;

    gpio->port->BSRR = 1 << gpio->pin;
    ret = SUCCESS;

    return ret;
}

ERROR_CODE Gpio_Set_Low(GPIO_h gpio)
{
	assert(gpio != 0);   // gpio exists
    assert((gpio->pin >= 0) && (gpio->pin < 16));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as output?

    ERROR_CODE ret = ERROR;

    gpio->port->BRR = 1 << gpio->pin;
    ret = SUCCESS;

    return ret;
}

ERROR_CODE Gpio_Toggle(GPIO_h gpio)
{
	assert(gpio != 0);   // gpio exists
    assert((gpio->pin >= 0) && (gpio->pin < 16));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as output?

    ERROR_CODE ret = ERROR;

    // Access both the set and reset bits of the BSRR register at the same time
    // with the reset bit using the invert of the ODR register current state
    // Done this way because it generates the most efficient assembly
	gpio->port->BSRR = ((gpio->port->ODR & (1 << gpio->pin)) << 16) | (~(gpio->port->ODR) & (1 << gpio->pin));
    ret = SUCCESS;

    return ret;
}