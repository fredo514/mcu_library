#ifndef ADC_H
#define ADC_H

#include "error.h"
#include "platform.h"
#include "stdbool.h"
#include "stdint.h"

typedef enum {
    ADC_READY_CALLBACK,
    ADC_NEW_SAMPLE_CALLBACK,
    ADC_SEQ_DONE_CALLBACK,
    ADC_OVERRUN_ERROR_CALLBACK,
    ADC_CB_ID_MAX
} ADC_CALLBACK_ID_t;

typedef enum {
    ADC_MODE_POLL,
    ADC_MODE_INTERRUPT,
    ADC_MODE_DMA,
    ADC_MODE_MAX
} ADC_MODE_t;

typedef enum {
    ADC_CONV_MODE_ONESHOT_SINGLE_CHANNEL,
    ADC_CONV_MODE_ONESHOT_ALL_CHANNELS,
    ADC_CONV_MODE_CONTINUOUS,
    ADC_CONV_MODE_MAX
} ADC_CONV_MODE_t;

tyedef enum {
    ADC_RESOLUTION_12BITS,
    ADC_RESOLUTION_10BITS,
    ADC_RESOLUTION_MAX
} ADC_RESOLUTION_t;

tyedef enum {
    ADC_CLK_SRC_HSI,
    ADC_CLK_SRC_AHB,
    ADC_CLK_SRC_MAX
} ADC_CLK_SRC_t;

tyedef enum {
    ADC_CLK_DIV_1,
    ADC_CLK_DIV_2,
    ADC_CLK_DIV_4,
    ADC_CLK_DIV_MAX
} ADC_CLK_DIV_t;

tyedef enum {
    ADC_WAIT_CONV_FETCH,
    ADC_WAIT_CONV_FREERUN,
    ADC_WAIT_CONV_MAX
} ADC_WAIT_CONV_t;

tyedef enum {
    ADC_AUTO_OFF_DISABLE,
    ADC_AUTO_OFF_ENABLE,
    ADC_AUTO_OFF_MAX
} ADC_AUTO_OFF_t;

tyedef enum {
    ADC_SCAN_FORWARD,
    ADC_SCAN_BACKWARD,
    ADC_SCAN_DIR_MAX
} ADC_SCAN_DIR_t;

tyedef enum {
    ADC_TRIG_SOFTWARE,
    ADC_TRIG_TIM1_TRGO_RISING = (ADC_CFGR1_EXTEN | (ADC_CFGR1_EXTSEL << 4)),
    ADC_TRIG_TIM1_TRGO_FALLING,
    ADC_TRIG_TIM1_TRGO_RISING_FALLING,
    ADC_TRIG_TIM1_CC_RISING,
    ADC_TRIG_TIM1_CC_FALLING,
    ADC_TRIG_TIM1_CC_RISING_FALLING,
    ADC_TRIG_TIM2_TRGO_RISING,
    ADC_TRIG_TIM2_TRGO_FALLING,
    ADC_TRIG_TIM2_TRGO_RISING_FALLING,
    ADC_TRIG_TIM3_TRGO_RISING,
    ADC_TRIG_TIM3_TRGO_FALLING,
    ADC_TRIG_TIM3_TRGO_RISING_FALLING,
    ADC_TRIG_TIM15_TRGO_RISING,
    ADC_TRIG_TIM15_TRGO_FALLING,
    ADC_TRIG_TIM15_TRGO_RISING_FALLING,
    ADC_TRIG_MODE_MAX
} ADC_TRIG_MODE_t;

typedef enum {
    ADC_STATUS_RESET,
    ADC_STATUS_IDLE,
    ADC_STATUS_BUSY,
    ADC_STATUS_ERROR,
    ADC_STATUS_MAX
} ADC_STATUS_t;

typedef uint32_t ADC_SAMPLE_t;

typedef struct ADC_CTX const * const ADC_h;

typedef struct {
    ADC_RESOLUTION_t resolution;
    ADC_CLK_SRC_t clk_source;
    ADC_CLK_DIV_t clk_div;
    ADC_SAMPLING_TIME_t sampling_time;
    ADC_WAIT_CONV_t wait_conv;
    ADC_AUTO_OFF_t auto_off;
    ADC_CONV_MODE_t conv_mode;
    ADC_SCAN_DIR_t scan_dir;
    ADC_TRIG_MODE_t trig_mode;
} ADC_CONFIG_t;

ADC_h Adc_Create(ADC_REGS_t const * const regs);
ERROR_CODE_t Adc_Init(ADC_h adc, ADC_CONFIG_t const * const config);
ERROR_CODE_t Adc_Callback_Register(ADC_h adc, ADC_CALLBACK_ID_t const callback_id, void * cb);

ERROR_CODE_t Adc_Start(ADC_h adc, ADC_MODE_t mode);
ERROR_CODE_t Adc_Stop(ADC_h adc);
ERROR_CODE_t Adc_Calibrate(ADC_h adc);

ERROR_CODE_t Adc_Conversion_Start(ADC_h adc);
bool Adc_Is_Conversion_Done(ADC_h adc);
ADC_SAMPLE_t Adc_Reading_Get(ADC_h adc);
ADC_SAMPLE_t Adc_Read(ADC_h adc, uint32_t timeout_ms); // blocking

// Use these SPARINGLY to extend the API
ERROR_CODE_t Adc_Reg_Write (REG_SIZE_t * const address, uint32_t const val);
REG_SIZE_t Adc_Reg_Read (REG_SIZE_t * const address);

#endif // ADC_H