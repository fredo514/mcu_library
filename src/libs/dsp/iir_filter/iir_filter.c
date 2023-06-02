#include "iir_filter.h"

struct IIR_CTX {
    IIR_DATA_t coeffs[1];
    IIR_DATA_t buffer[1];
    IIR_DATA_t out;
};

IIR_h Fir_Filt_Create(void) {
    IIR_h inst = calloc(1, sizeof(struct IIR_CTX));
    IIR_DATA_t * coeff = calloc(5, sizeof(IIR_DATA_t));
    inst->coeffs = coeff;
    IIR_DATA_t * buffer = calloc(2, sizeof(IIR_DATA_t));
    inst->buffer = buffer;
    
    return inst;
}

ERROR_CODE_t Iir_Filt_Init(IIR_h filt, IIR_DATA_t * coeffs) {
    filt->out = 0;

    for (int i=0; i<5; ++i) {
        filt->coeff[i] = coeffs[i];
    }

    filt->buffer[0] = 0;
    filt->buffer[1] = 0;

    return SUCCESS;
}

IIR_DATA_t Iir_Filt_Update(IIR_h filt, IIR_DATA_t input) {
    IIR_DATA_t acc = input; 
    IIR_DATA_t output; 

    acc -= filt->buffer[0] * filt->coeffs[0]; 
    acc -= filt->buffer[1] * filt->coeffs[1]; 

    output = acc * filt->coeffs[2]; 
    output += filt->buffer[0] * filt->coeffs[3];    
    output += filt->buffer[1] * filt->coeffs[4]; 

    filt->buffer[1] = filt->buffer[0]; 
    filt->buffer[0] = acc; 
    filt->out = output;

    return output; 
}

IIR_DATA_t Iir_Filt_Output_Get(IIR_h filt) {
    return filt->out;
}