#include "iir_filter_fp.h"

struct IIR_FP_CTX {
    IIR_FP_DATA_t coeffs[1];
    IIR_FP_DATA_t buffer[1];
    IIR_FP_DATA_t out;
    uint8_t scale_num;
    uint8_t scale_denum;
};

IIR_FP_h Fir_Filt_Fp_Create(void) {
    IIR_h inst = calloc(1, sizeof(struct IIR_FP_CTX));
    IIR_FP_DATA_t * coeff = calloc(5, sizeof(IIR_FP_DATA_t));
    inst->coeffs = coeff;
    IIR_FP_DATA_t * buffer = calloc(4, sizeof(IIR_FP_DATA_t));
    inst->buffer = buffer;
    
    return inst;
}

ERROR_CODE_t Iir_Filt_Fp_Init(IIR_FP_h filt, IIR_FP_DATA_t * coeffs, uint8_t scale_num, uint8_t scale_denum) {
    filt->out = 0;
    filt->scale_num = scale_num;
    filt->scale_denum = scale_denum;

    for (int i=0; i<5; ++i) {
        filt->coeff[i] = coeffs[i];
    }

    for (int i=0; i<4; ++i) {
        filt->buffer[i] = 0;
    }

    return SUCCESS;
}

IIR_FP_DATA_t Iir_Filt_Fp_Update(IIR_FP_h filt, IIR_FP_DATA_t input) {
    FP_LARGER_INT_t acc = 0; 
    FP_LARGER_INT_t interimSum = 0;
    IIR_FP_DATA_t output; 

    acc += (FP_LARGER_INT_t)input * (FP_LARGER_INT_t)(filt->coeffs[0]); 
    acc += (FP_LARGER_INT_t)filt->buffer[0] * (FP_LARGER_INT_t)(filt->coeffs[1]); 
    acc += (FP_LARGER_INT_t)filt->buffer[1] * (FP_LARGER_INT_t)(filt->coeffs[2]); 

    if (filt->scaleNum > 0) { 
        acc = acc >> filt->scale_num; 
    } else { 
        acc = acc << abs(filt->scale_num); 
    } 

    filt->buffer[1] = filt->buffer[0]; 
    filt->buffer[0] = input; 

    interimSum += (FP_LARGER_INT_t)filt->buffer[2] * (FP_LARGER_INT_t)(filt->coeffs[3]); 
    interimSum += (FP_LARGER_INT_t)filt->buffer[3] * (FP_LARGER_INT_t)(filt->coeffs[4]); 

    if (filt->scaleDenum > 0) {
        acc -= (interimSum >> filt->scale_denum); 
    } else { 
        acc -= (interimSum << abs(filt->scale_denum)); 
    }

    output = (IIR_FP_DATA_t)(acc >> (8*sizeof(IIR_FP_DATA_t)); 

    // TO DO: add min/max clamp 
    filt->buffer[3] = filt->buffer[2]; 
    filt->buffer[2] = output; 

    return output; 
}

IIR_FP_DATA_t Iir_Filt_Fp_Output_Get(IIR_FP_h filt) {
    return filt->buffer[2];
}