#ifndef ADC_H
#define ADC_H

#include "error.h"
#include "platform.h"
#include "stdbool.h"
#include "stdint.h"

typedef enum {
    ADC_CONV_DONE_CALLBACK,
    ADC_ERROR_CALLBACK,
    ADC_CB_ID_MAX
} ADC_CALLBACK_ID_t;

typedef enum {
    ADC_MODE_MANUAL,
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
    ADC_STATUS_RESET,
    ADC_STATUS_IDLE,
    ADC_STATUS_BUSY,
    ADC_STATUS_ERROR,
    ADC_STATUS_MAX
} ADC_STATUS_t;

typedef uint32_t ADC_SAMPLE_t;

typedef struct ADC_CTX const * const ADC_h;

typedef struct {
    
} ADC_CONFIG_t;

ADC_h Adc_Create(ADC_REGS_t const * const regs);
ERROR_CODE_t Adc_Init(ADC_h adc, ADC_CONFIG_t const * const config);
ERROR_CODE_t Adc_Callback_Register(ADC_h adc, ADC_CALLBACK_ID_t const callback_id, void * cb);

ERROR_CODE_t Adc_Start(ADC_h adc, ADC_MODE_t mode);
ERROR_CODE_t Adc_Stop(ADC_h adc);

ERROR_CODE_t Adc_Conversion_Start(ADC_h adc);
bool Adc_Is_Conversion_Done(ADC_h adc);
ADC_SAMPLE_t Adc_Reading_Get(ADC_h adc);
ADC_SAMPLE_t Adc_Read(ADC_h adc, uint32_t timeout_ms); // blocking

// Use these SPARINGLY to extend the API
ERROR_CODE_t Adc_Reg_Write (REG_SIZE_t * const address, uint32_t const val);
REG_SIZE_t Adc_Reg_Read (REG_SIZE_t * const address);

#endif // ADC_H