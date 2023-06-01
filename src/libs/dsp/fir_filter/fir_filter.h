#ifndef FIR_FILTER_H
#define FIR_FILTER_H

typedef float FIR_DATA_t;

typedef struct FIR_CTX const * const FIR_h;

FIR_h Fir_Filt_Create(uint8_t order);
ERROR_CODE_t Fir_Filt_Init(FIR_h filt, FIR_DATA_t * coeffs);

FIR_DATA_t Fir_Filt_Form1_Update(FIR_h filt, FIR_DATA_t input);
FIR_DATA_t Fir_Filt_Linear_Update(FIR_h filt, FIR_DATA_t input);

#endif // FIR_FILTER_H