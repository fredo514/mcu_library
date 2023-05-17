#include "timer.h"
#include "stm32_hal.h"
#include "assert.h"

typedef struct TIMER_CTX {
  TIMER_REGS_t * const regs;
};

TIMER_h Timer_Create(TIMER_REGS_t const * const regs) {
    TIMER_h inst = calloc(1, sizeof(struct I2C_CTX));
    inst->regs = regs;

    return inst;
}

ERROR_CODE_t Timer_Init(TIMER_h timer, TIMER_CONFIG_t const * const config) {
    // Enable Clock
    
    // Reset Timer
    
    // Clear Count
    
    // Calculate Period

    // Set Prescalers
    config->timer->reg = ((Sysclk_Get_Hz() / config->clk_div) / (tmr_div / config->period_us)) - 1

    // Configure Interrupts
    // if (config->isr_enable) {
      // enable timer irq
    //}
}