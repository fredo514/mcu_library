#ifndef CORE_H
#define CORE_H

#include "stdint.h"
#include <stdbool.h>
#include "error.h"
#include "macros.h"
#include "platform.h"

// TODO: power modes?

// disables specified interrupt before running enclosed block, then restore its
// original state after. 
// Usage: CRITICAL_SECTION(FOO_IRQn) {
//     baz();
// }
#define CRITICAL_SECTION(IRQ_ID)                    \
  DEFER(bool UNIQUE_VAR_NAME(_int_state_) =         \
            Core_Interrupts_SaveAndDisable(IRQ_ID), \
        Core_Interrupts_Restore(IRQ_ID, UNIQUE_VAR_NAME(_int_state_)))

typedef enum {
  CORE_CLK_SRC_HSI,
  CORE_CLK_SRC_HSE,
  CORE_CLK_SRC_HSI_PLL,
  CORE_CLK_SRC_HSE_PLL,
  CORE_CLK_SRC_MAX
} core_clk_src_t;

typedef enum {
  CORE_APB_CLK_PRE_1 = 1,
  CORE_APB_CLK_PRE_2 = 2,
  CORE_APB_CLK_PRE_4 = 4,
  CORE_APB_CLK_PRE_8 = 8,
  CORE_APB_CLK_PRE_16 = 16,
  CORE_APB_CLK_PRE_MAX
} core_apb_clk_prescaler_t;

typedef enum {
  IRQ_ALL = SIZE_MAX
} CORE_IRQ_t;

typedef enum { 
  CORE_CB_SYSTICK, 
  CORE_CB_MAX 
} core_cb_t;

typedef struct {
  uint16_t m;
  uint16_t n;
  uint16_t p;
  uint16_t q;
} core_pll_params_t;

typedef struct {
  core_clk_src_t clock_src;
  core_pll_params_t pll_params;
  core_apb_clk_prescaler_t apb1_prescaler;
  core_apb_clk_prescaler_t apb2_prescaler;
} core_clock_config_t;



ERROR_CODE_t Core_Init(void);
void Core_DeInit(void);

error_t Core_Clock_Init(core_clock_config_t const* const config);

ERROR_CODE_t Core_Interrupt_Callback_Attach(CORE_IRQ_t const irq_id, void* cb);

void core_interrupt_disable_all(void);
bool Core_Save_And_Disable_Interrupt(CORE_IRQ_t const IRQn);
void Core_Restore_Interrupt(CORE_IRQ_t const irq_id, bool const wasEnabled);

uint32_t Core_SysTicks_Get(void);

void Core_Breakpoint(void);

void Core_Nop(void);

#endif // CORE_H