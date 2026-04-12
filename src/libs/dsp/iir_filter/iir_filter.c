#include "iir_filter.h"

struct iir_ctx {
    iir_data_t coeffs[5]; // a1, a2, b0, b1, b2
    iir_data_t buffer[2]; // w1, w2
    iir_data_t out;
};

iir_t * Iir_Filt_Create(void) {
    iir_t * inst = calloc(1, sizeof(struct iir_ctx));
    
    return inst;
}

void Iir_Filt_Init(iir_t * const filt, iir_data_t * coeffs) {
    filt->out = 0;

    for (int i=0; i<5; ++i) {
        filt->coeff[i] = coeffs[i];
    }

    filt->buffer[0] = 0;
    filt->buffer[1] = 0;
}

iir_data_t Iir_Filt_Update(iir_t * const filt, iir_data_t input) {
    // IIR_DATA_t acc = input; 
    // IIR_DATA_t output; 

    // acc -= filt->buffer[0] * filt->coeffs[0]; 
    // acc -= filt->buffer[1] * filt->coeffs[1]; 

    // output = acc * filt->coeffs[2]; 
    // output += filt->buffer[0] * filt->coeffs[3];    
    // output += filt->buffer[1] * filt->coeffs[4]; 

    // filt->buffer[1] = filt->buffer[0]; 
    // filt->buffer[0] = acc; 
    // filt->out = output;

    // using transform form 2
    // y_n = (b0 * x_n) + w1
    filt->out = (filt->coeffs[2] * input) + filt->buffer[0];

    // w1 = (b1 * x_n) - (a1 * y_n) + w2
    filt->buffer[0] = (filt->coeffs[3] * input) - (filt->coeffs[0] * filt->out) + filt->buffer[1];

    // w2 = (b2 * x_n) - (a2 * y_n)
    filt->buffer[1] = (filt->coeffs[4] * input) - (filt->coeffs[1] * filt->out);

    return filt->out; 
}

IIR_DATA_t Iir_Filt_Output_Get(IIR_h filt) {
    return filt->out;
}