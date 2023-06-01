#include "fir_filter.h"
#include "stdint.h"
#include "error.h"

struct FIR_CTX {
    uint8_t order;
    FIR_DATA_t coeff[1];
    FIR_DATA_t input[1];
    FIR_DATA_t out;
    uint8_t index;
};

FIR_h Fir_Filt_Create(uint8_t order) {
    FIR_h inst = calloc(1, sizeof(struct FIR_CTX));
    inst->order = order;
    FIR_DATA_t * coeff = calloc(order, sizeof(FIR_DATA_t));
    inst->coeff = coeff;
    FIR_DATA_t * input = calloc(order/2, sizeof(FIR_DATA_t));
    inst->input = input;
    
    return inst;
}

ERROR_CODE_t Fir_Filt_Init(FIR_h filt, FIR_DATA_t * coeffs) {
    filt->index = 0;
    filt->out = 0;

    return SUCCESS;
}

FIR_DATA_t Fir_Filt_Form1_Update(FIR_h filt, FIR_DATA_t input) {
    
}