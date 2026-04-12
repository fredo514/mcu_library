#ifndef IIR_FILTER_H
#define IIR_FILTER_H

#include "error.h"

typedef float iir_data_t;

typedef struct iir_ctx iir_t;

iir_t * Iir_Filt_Create(void);
error_t Iir_Filt_Init(iir_t * const filt, IIR_DATA_t * coeffs);

// change gains using ramp every update
error_t Iir_Compute_Coeffs(iir_t * const filt, iir_filt_type_t type, iir_data_t gain, iir_data_t cutoff)

iir_data_t Iir_Filt_Update(iir_t * const filt, iir_data_t input);
iir_data_t Iir_Filt_Output_Get(iir_t * const filt);

#endif // IIR_FILTER_H