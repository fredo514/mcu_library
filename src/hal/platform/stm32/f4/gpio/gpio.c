#include "gpio.h"
#include "stm32_hal.h"
#include "assert.h"

typedef struct GPIO_CTX {
    GPIO_REGS_t const * const port;
    GPIO_PIN_t const pin;
};

GPIO_h Gpio_Create(GPIO_PORT_t const * const port, GPIO_PIN_t const pin) {
    GPIO_h inst = calloc(1, sizeof(struct GPIO_CTX));
    inst->port = port;
    inst->pin = pin;

    return inst;
}

ERROR_CODE_t Gpio_Init(GPIO_CONFIG_t const * const config) {
    ASSERT(config);   // config exists
    ASSERT(config->gpio);   // gpio exists
    ASSERT((config->gpio->pin >= 0) && (config->gpio->pin < GPIO_PIN_MAX));   // only 16 pins per port

    // Find the clock enable bit for the gpio port
    // uses if/else to be able to test with mock register structure.
    uint8_t port_index = 0;
    if (config->gpio->port == GPIO_PORTA) {
        port_index = 0;
    }
    else if (config->gpio->port == GPIO_PORTB) {
        port_index = 1;
    }
    else if (config->gpio->port == GPIO_PORTC) {
        port_index = 2;
    }
    else if (config->gpio->port == GPIO_PORTD) {
        port_index = 3;
    }
    else if (config->gpio->port == GPIO_PORTF) {
        port_index = 5;
    }
    else {
        return ERROR_NO_DEV;
    }

    // enable clock for the gpio port
    Core_Reg_Write(RCC->AHBENR, SET_MASK(RCC->AHBENR, 1 << port_index));

    // configure gpio registers
    if (((config->mode & GPIO_MODE_MASK) == GPIO_MODE_OUTPUT) || ((config->mode & GPIO_MODE_MASK) == GPIO_MODE_ALT)) {
        // output speed option is only available for output or alternate mode
        Gpio_Reg_Write(config->gpio->port->OSPEEDR, CLEAR_MASK(config->gpio->port->OSPEEDR, GPIO_OSPEEDR_MASK << (config->gpio->pin * 2)));       // clear the bits first
        Gpio_Reg_Write(config->gpio->port->OSPEEDR, SET_MASK(config->gpio->port->OSPEEDR, config->speed << (config->gpio->pin * 2)));

        // output type option is only available for output or alternate mode
        Gpio_Reg_Write(config->gpio->port->OTYPER, SET_MASK(config->gpio->port->OTYPER, ((config->mode >> GPIO_MODE_TYPE_OFFSET) & GPIO_MODE_MASK) << config->gpio->pin));
    }

    if (config->mode != GPIO_MODER_ANALOG) {
        // pull-up/down is not available for analog input mode
        Gpio_Reg_Write(config->gpio->port->PUPDR, CLEAR_MASK(config->gpio->port->PUPDR, GPIO_PUPDR_MASK << (config->gpio->pin * 2)));       // clear the bits first
        Gpio_Reg_Write(config->gpio->port->PUPDR, SET_MASK(config->gpio->port->PUPDR, config->pull << (config->gpio->pin * 2)));
    }

    if ((config->mode & GPIO_MODE_MASK) == GPIO_MODE_ALT) {
        // configure alternate function
        Gpio_Reg_Write(config->gpio->port->AFR[gpio->pin >> 3], \
                        CLEAR_MASK(config->gpio->port->AFR[config->gpio->pin >> 3], GPIO_AFR_MASK << ((config->gpio->pin & 0x7) * 4)));       // clear the bits first
        Gpio_Reg_Write(config->gpio->port->AFR[gpio->pin >> 3], \
                        SET_MASK(config->gpio->port->AFR[config->gpio->pin >> 3], config->alt_function << ((config->gpio->pin & 0x7) * 4)));
    }

    // set the gpio mode
    Gpio_Reg_Write(config->gpio->port->MODER, CLEAR_MASK(config->gpio->port->MODER, GPIO_MODER_MASK << (config->gpio->pin * 2)));       // clear the bits first
    Gpio_Reg_Write(config->gpio->port->MODER, SET_MASK(config->gpio->port->MODER, config->mode & GPIO_MODE_MASK << (config->gpio->pin * 2)));

    if ((config->mode >> GPIO_MODE_EXTI_OFFSET) & GPIO_MODE_MASK) {
        // configure external interrupt or event
        // enable syscfg clock
        Gpio_Reg_Write(RCC->APB2ENR, SET_MASK(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN));

        // attach exti to gpio port
        Gpio_Reg_Write(SYSCFG->EXTICR[config->gpio->pin >> 2], CLEAR_MASK(SYSCFG->EXTICR[config->gpio->pin >> 2], SYSCFG_EXTICR_MASK << ((config->gpio->pin & 0x7) * 4)));       // clear the bits first
        Gpio_Reg_Write(SYSCFG->EXTICR[config->gpio->pin >> 2], SET_MASK(SYSCFG->EXTICR[config->gpio->pin >> 2], port_index << ((config->gpio->pin & 0x7) * 4)));

        // set the rising/falling trigger configutation
        if (((config->mode >> GPIO_MODE_TRIG_EDGE_OFFSET) & GPIO_MODE_TRIG_RISE) == GPIO_MODE_TRIG_RISE) {
            Gpio_Reg_Write(EXTI->RTSR, SET_MASK(EXTI->RTSR, 1 << config->gpio->pin));
        }

        if (((config->mode >> GPIO_MODE_TRIG_EDGE_OFFSET) & GPIO_MODE_TRIG_FALL) == GPIO_MODE_TRIG_FALL) {
            Gpio_Reg_Write(EXTI->FTSR, SET_MASK(EXTI->FTSR, 1 << config->gpio->pin));
        }

        if (((config->mode >> GPIO_MODE_EXTI_OFFSET) & GPIO_MODE_MASK) == GPIO_MODE_EXTI_EVT) {
            Gpio_Reg_Write(EXTI->EMR, SET_MASK(EXTI->EMR, 1 << config->gpio->pin));
        }

        if (((config->mode >> GPIO_MODE_EXTI_OFFSET) & GPIO_MODE_MASK) == GPIO_MODE_EXTI_IT) {
            Gpio_Reg_Write(EXTI->IMR, SET_MASK(EXTI->IMR, 1 << config->gpio->pin));
        }
    }

    return SUCCESS;
}

GPIO_STATE_t Gpio_Read(GPIO_h gpio)
{
	ASSERT_DEBUG(gpio != 0);   // gpio exists
    ASSERT_DEBUG((gpio->pin >= 0) && (gpio->pin < GPIO_PIN_MAX));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as input?
    
    // if pin'th bit of the port's IDR register is set, then return SET. Otherwise, return RESET
    // Done using ternary operator because it generates the most efficient assembly
    return (gpio->port->IDR & (1 << gpio->pin)) ? GPIO_SET : GPIO_RESET;
}

ERROR_CODE_t Gpio_Set(GPIO_h gpio, GPIO_STATE_t const state)
{
	ASSERT_DEBUG(gpio != 0);   // gpio exists
    ASSERT_DEBUG((gpio->pin >= 0) && (gpio->pin < GPIO_PIN_MAX));   // only 16 pins per port

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
    ASSERT_DEBUG((gpio->pin >= 0) && (gpio->pin < GPIO_PIN_MAX));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as output?

    ERROR_CODE_t ret = ERROR;

    gpio->port->BSRR = 1 << gpio->pin;
    ret = SUCCESS;

    return ret;
}

ERROR_CODE_t Gpio_Set_Low(GPIO_h gpio)
{
	ASSERT_DEBUG(gpio != 0);   // gpio exists
    ASSERT_DEBUG((gpio->pin >= 0) && (gpio->pin < GPIO_PIN_MAX));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as output?

    ERROR_CODE_t ret = ERROR;

    gpio->port->BRR = 1 << gpio->pin;
    ret = SUCCESS;

    return ret;
}

ERROR_CODE_t Gpio_Toggle(GPIO_h gpio)
{
	ASSERT_DEBUG(gpio != 0);   // gpio exists
    ASSERT_DEBUG((gpio->pin >= 0) && (gpio->pin < GPIO_PIN_MAX));   // only 16 pins per port

    // TODO: What to do if the gpio is not set as output?

    ERROR_CODE_t ret = ERROR;

    // Access both the set and reset bits of the BSRR register at the same time
    // with the reset bit using the invert of the ODR register current state
    // Done this way because it generates the most efficient assembly
	gpio->port->BSRR = ((gpio->port->ODR & (1 << gpio->pin)) << 16) | (~(gpio->port->ODR) & (1 << gpio->pin));
    ret = SUCCESS;

    return ret;
}

ERROR_CODE_t inline Gpio_Reg_Write (REG_SIZE_t const address, uint32_t const val) {
    *((REG_SIZE_t * const)address) = val;
}

REG_SIZE_t inline Gpio_Reg_Read (REG_SIZE_t const address) {
    return *((REG_SIZE_t * const)address);
}