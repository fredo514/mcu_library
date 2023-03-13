#include "timer.h"
#include "stm32_hal.h"
#include "assert.h"

typedef struct {
  TIMER_TypeDef * const regs;
} TIMER_CTX;

ERROR_CODE Timer_Init(TIMER_CONFIG const * const config) {
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