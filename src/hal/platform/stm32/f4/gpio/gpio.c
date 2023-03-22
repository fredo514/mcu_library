#include "gpio.h"
#include "stm32_hal.h"
#include "assert.h"

typedef struct
{
    GPIO_PORT_t * const port;
    GPIO_PIN_t const pin;
} GPIO_CTX_t;

ERROR_CODE_t Gpio_Init(GPIO_h gpio, GPIO_CONFIG_t const * const config) {
    ASSERT(config != 0);   // config exists
    ASSERT(gpio != 0);   // gpio exists
    ASSERT((gpio->pin >= 0) && (gpio->pin < 16));   // only 16 pins per port

    // enable clock for gpio port
}

GPIO_STATE_t Gpio_Read(GPIO_h gpio)
{
	ASSERT_DEBUG(gpio != 0);   // gpio exists
    ASSERT_DEBUG((gpio->pin >= 0) && (gpio->pin < 16));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as input?
    
    // if pin'th bit of the port's IDR register is set, then return SET. Otherwise, return RESET
    // Done using ternary operator because it generates the most efficient assembly
    return (gpio->port->IDR & (1 << gpio->pin)) ? GPIO_SET : GPIO_RESET;
}

ERROR_CODE_t Gpio_Set(GPIO_h gpio, GPIO_STATE_t const state)
{
	ASSERT_DEBUG(gpio != 0);   // gpio exists
    ASSERT_DEBUG((gpio->pin >= 0) && (gpio->pin < 16));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as output?

    ERROR_CODE_t ret = ERROR;

    // Access both the set and reset bits of the BSRR register at the same time
    // with the reset bit using the invert of the requested state
    // Done this way because it generates the most efficient assembly
    gpio->port->BSRR = ((REG_SIZE)state << gpio->pin) << 16) | (((REG_SIZE)!state) << gpio->pin);
    ret = SUCCESS;

    return ret;
}

ERROR_CODE_t Gpio_Set_High(GPIO_h gpio)
{
	ASSERT_DEBUG(gpio != 0);   // gpio exists
    ASSERT_DEBUG((gpio->pin >= 0) && (gpio->pin < 16));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as output?

    ERROR_CODE_t ret = ERROR;

    gpio->port->BSRR = 1 << gpio->pin;
    ret = SUCCESS;

    return ret;
}

ERROR_CODE_t Gpio_Set_Low(GPIO_h gpio)
{
	ASSERT_DEBUG(gpio != 0);   // gpio exists
    ASSERT_DEBUG((gpio->pin >= 0) && (gpio->pin < 16));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as output?

    ERROR_CODE_t ret = ERROR;

    gpio->port->BRR = 1 << gpio->pin;
    ret = SUCCESS;

    return ret;
}

ERROR_CODE_t Gpio_Toggle(GPIO_h gpio)
{
	ASSERT_DEBUG(gpio != 0);   // gpio exists
    ASSERT_DEBUG((gpio->pin >= 0) && (gpio->pin < 16));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as output?

    ERROR_CODE_t ret = ERROR;

    // Access both the set and reset bits of the BSRR register at the same time
    // with the reset bit using the invert of the ODR register current state
    // Done this way because it generates the most efficient assembly
	gpio->port->BSRR = ((gpio->port->ODR & (1 << gpio->pin)) << 16) | (~(gpio->port->ODR) & (1 << gpio->pin));
    ret = SUCCESS;

    return ret;
}

ERROR_CODE_t Gpio_Reg_Write (REG_SIZE_t const address, uint32_t const val) {
    *((REG_SIZE_t * const)address) = val;
}

REG_SIZE_t Gpio_Reg_Read (REG_SIZE_t const address) {
    return *((REG_SIZE_t * const)address);
}