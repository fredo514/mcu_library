#include "fir_filter.h"

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

    for (size_t i=0; i<filt->order; ++i) {
        filt->coeff[i] = coeffs[i];
        filt->input[i] = 0;
    }

    return SUCCESS;
}

FIR_DATA_t Fir_Filt_Form1_Update(FIR_h filt, FIR_DATA_t input) {
    filt->input[filt->index++] = input;

    if (filt->index >= filt->order) { 
        filt->index=0;
    } 

    filt->out=0; 

    Uint8_t sum_index = filt->index; 

    for (size_t i=0; i < filt->order; ++i) { 
        if (sum_index > 0) { 
            sum_index--; 
        } else { 
            sum_index = filt->order; 
        } 

        filt->out += filt->coeff[i] * filt->input[sum_index]; 
    } 

    return filt->out; 
}

FIR_DATA_t Fir_Filt_Linear_Update(FIR_h filt, FIR_DATA_t input) {
    filt->input[filt->index++] = input;

    if (filt->index >= filt->order) { 
        filt->Index=0; 
    } 

    FIR_DATA_t * new_data_ptr = &(filt->input[filt->index-1]); 

    if (filt->index == 0) { 
        new_data_ptr = &(filt->input[filt->order-1]); 
    } 

    FIR_DATA_t * old_data_ptr = &(filt->input[Me->index]); 
    FIR_DATA_t * tap_ptr = filt->coeff; 
    FIR_DATA_t acc = 0; 
    FIR_DATA_t presum; 

    do { 
        presum = (*old_data_ptr++) + (*new_data_ptr--); 
        acc += *tap_ptr++ * presum; 

        if (old_data_ptr > &(filt->input[filt->order-1])) { 
            old_data_ptr = &(filt->input[0]); 
        } 

        if (new_data_ptr < &(filt->input[0])) { 
            new_data_ptr = &(filt->input[filt->order-1]); 
        } 
    } while (old_data_ptr != new_data_ptr); 

    acc += (*tap_ptr) * (*old_data_ptr); 
    filt->out = acc; 

    return acc; 
}

FIR_DATA_t Fir_Filt_Output_Get(FIR_h filt) {
    return filt->out;
}