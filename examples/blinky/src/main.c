#include "config.h"

#include "bsp.h"
#include "core.h"
#include "delay.h"

static void system_init(void);

static void system_init(void) {
  // initialize PLL
  core_clock_config_t clk_config = {
      .clock_src = CORE_CLK_SRC_HSI_PLL,
      .pll_params =
          {// core clock at 168 MHz
           .m = 8,
           .n = 168,
           .p = 2,
           .q = 4},
      .apb1_prescaler = CORE_APB_CLK_PRE_4,  // APB1 bus clocks at 42MHz
      .apb2_prescaler = CORE_APB_CLK_PRE_2   // APB2 bus clocks at 84MHz
  };
  Core_Clock_Init(&clk_config);

  // Enable Interrupt system clock
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

  // Set up Systick timer interrupt at 1ms.
  SysTick_Config(SystemCoreClock / 1000);
}

int main(void) {
  system_init();
  
  // initialize peripherals
  bsp_init();

  // initialize delay
  delay_config_t delay_config = {.time_now_cb = &Core_Ticks_Get};
  Delay_Init(&delay_config);

  uint32_t led_time = Delay_Time_Now();

  for (;;) {
    // infinite loop

    // blink led every 500ms
    if (Delay_Is_Elapsed(led_time, 500)) {
      Hb_Led_Toggle();
      led_time = Delay_Time_Now();
    }
  }
}