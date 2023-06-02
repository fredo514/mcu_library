#ifndef IIR_FILTER_H
#define IIR_FILTER_H

#include "error.h"

typedef float IIR_DATA_t;

typedef struct IIR_CTX const * const IIR_h;

IIR_h Fir_Filt_Create(void);
ERROR_CODE_t Fir_Filt_Init(IIR_h filt, IIR_DATA_t * coeffs);

IIR_DATA_t Iir_Filt_Update(IIR_h filt, IIR_DATA_t input);
IIR_DATA_t Iir_Filt_Output_Get(IIR_h filt);

#endif // IIR_FILTER_H