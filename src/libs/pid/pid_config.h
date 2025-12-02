#ifndef PID_PRIV_H
#define PID_PRIV_H

#include <stdint.h>

#define PID_USE_FLOAT

#ifdef PID_USE_FLOAT
typedef float pid_data_t;
#else
typedef uint32_t pid_data_t; // or fixed‐point
#endif

#endif PID_PRIV_H