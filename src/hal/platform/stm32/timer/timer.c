#include "timer.h"
#include "stm32_hal.h"
#include "assert.h"

typedef struct {
  TIMER_REGS_t * const regs;
} TIMER_CTX_t;

ERROR_CODE_t Timer_Init(TIMER_CONFIG_t const * const config) {
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