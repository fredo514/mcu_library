#ifndef PID_CONFIG_H
#define PID_CONFIG_H

#include <stdint.h>

#define PID_USE_FLOAT

#ifdef PID_USE_FLOAT
typedef float pid_data_t;
#else
typedef uint32_t pid_data_t; // or fixed‐point
#endif

#endif // PID_CONFIG_H