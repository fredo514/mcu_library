#ifndef ADC_H
#define ADC_H

#include "error.h"
#include "platform.h"
#include "stdbool.h"
#include "stdint.h"

typedef uint32_t ADC_SAMPLE_t;

typedef enum {
    ADC_READY_CALLBACK,
    ADC_NEW_SAMPLE_CALLBACK,
    ADC_SEQ_DONE_CALLBACK,
    ADC_OVERRUN_ERROR_CALLBACK,
    ADC_CB_ID_MAX
} ADC_CALLBACK_ID_t;

typedef enum {
   ADC_CHANNEL_0 = 0,
   ADC_CHANNEL_1,
   ADC_CHANNEL_2,
   ADC_CHANNEL_3,
   ADC_CHANNEL_4,
   ADC_CHANNEL_5,
   ADC_CHANNEL_6,
   ADC_CHANNEL_7,
   ADC_CHANNEL_8,
   ADC_CHANNEL_9,
   ADC_CHANNEL_10,
   ADC_CHANNEL_11,
   ADC_CHANNEL_12,
   ADC_CHANNEL_13,
   ADC_CHANNEL_14,
   ADC_CHANNEL_15,
   ADC_CHANNEL_16,
   ADC_CHANNEL_17,
   ADC_CHANNEL_18,
   ADC_CHANNEL_MAX
} ADC_CHANNEL_t;

typedef enum {
    ADC_CLK_SRC_HSI,
    ADC_CLK_SRC_PCLK,
    ADC_CLK_SRC_MAX
} ADC_CLK_SRC_t;

typedef enum {
    ADC_CLK_DIV_NONE = (0<<ADC_CFGR2_CKMODE_Pos),
    ADC_CLK_DIV_2 = (1<<ADC_CFGR2_CKMODE_Pos),
	ADC_CLK_DIV_4 = (2<<ADC_CFGR2_CKMODE_Pos),
    ADC_CLK_DIV_MAX
} ADC_CLK_DIV_t;

typedef enum {
    ADC_RESOLUTION_12BITS = (0<<ADC_CFGR1_RES_Pos),
    ADC_RESOLUTION_10BITS = (1<<ADC_CFGR1_RES_Pos),
	ADC_RESOLUTION_8BITS = (2<<ADC_CFGR1_RES_Pos),
    ADC_RESOLUTION_MAX
} ADC_RESOLUTION_t;

typedef enum {
	ADC_SAMPLING_1_5_CLK = (0<<ADC_SMPR_SMP_Pos),
	ADC_SAMPLING_7_5_CLK = (1<<ADC_SMPR_SMP_Pos),
	ADC_SAMPLING_13_5_CLK = (2<<ADC_SMPR_SMP_Pos),
	ADC_SAMPLING_28_5_CLK = (3<<ADC_SMPR_SMP_Pos),
	ADC_SAMPLING_41_5_CLK = (4<<ADC_SMPR_SMP_Pos),
	ADC_SAMPLING_55_5_CLK = (5<<ADC_SMPR_SMP_Pos),
	ADC_SAMPLING_71_5_CLK = (6<<ADC_SMPR_SMP_Pos),
	ADC_SAMPLING_239_5_CLK = (7<<ADC_SMPR_SMP_Pos),
	ADC_SAMPLING_TIME_MAX,
} ADC_SAMPLING_TIME_t;

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

typedef enum {
    ADC_WAIT_CONV_FETCH,
    ADC_WAIT_CONV_FREERUN,
    ADC_WAIT_CONV_MAX
} ADC_WAIT_CONV_t;

typedef enum {
    ADC_AUTO_OFF_DISABLE,
    ADC_AUTO_OFF_ENABLE,
    ADC_AUTO_OFF_MAX
} ADC_AUTO_OFF_t;

typedef enum {
    ADC_SCAN_FORWARD,
    ADC_SCAN_BACKWARD,
    ADC_SCAN_DIR_MAX
} ADC_SCAN_DIR_t;

typedef enum {
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
    ADC_STATE_RESET,
    ADC_STATE_IDLE,
    ADC_STATE_BUSY,
    ADC_STATE_ERROR,
    ADC_STATE_MAX
} ADC_STATE_t;

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

ERROR_CODE_t Adc_Enable(ADC_h adc, ADC_MODE_t mode);
ERROR_CODE_t Adc_Disable(ADC_h adc);
ERROR_CODE_t Adc_Calibrate(ADC_h adc);

ERROR_CODE_t Adc_Channel_Set(ADC_h adc, ADC_CHANNEL_t channel);
ERROR_CODE_t Adc_Conversion_Start(ADC_h adc);
bool Adc_Is_Conversion_Done(ADC_h adc);
ADC_SAMPLE_t Adc_Reading_Get(ADC_h adc);
ADC_SAMPLE_t Adc_Channel_Read(ADC_h adc, ADC_CHANNEL_t channel, uint32_t timeout_ms); // blocking

// Use these SPARINGLY to extend the API
ERROR_CODE_t Adc_Reg_Write (REG_SIZE_t * const address, uint32_t const val);
REG_SIZE_t Adc_Reg_Read (REG_SIZE_t * const address);

#endif // ADC_H