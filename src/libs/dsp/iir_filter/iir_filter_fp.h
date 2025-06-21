#ifndef IIR_FILTER_FP_H
#define IIR_FILTER_FP_H

#include "error.h"
#include "fixed_point_22_10.h"

typedef FIXED_POINT_22_10_t IIR_FP_DATA_t;

typedef struct IIR_FP_CTX const * const IIR_FP_h;

IIR_FP_h Fir_Filt_Fp_Create(void);
ERROR_CODE_t Fir_Filt_Fp_Init(IIR_FP_h filt, IIR_FP_DATA_t * coeffs);

IIR_FP_DATA_t Iir_Filt_Fp_Form1_Update(IIR_FP_h filt, IIR_FP_DATA_t input);
IIR_FP_DATA_t Iir_Filt_Fp_Output_Get(IIR_FP_h filt);

#endif // IIR_FILTER_FP_H