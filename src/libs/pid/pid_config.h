//#define PID_SIMPLE_ERROR_CB

#define PID_USE_FLOAT
#ifdef PID_USE_FLOAT
typedef PID_DATA_t float;
#else
typedef PID_DATA_t uint32_t;
#endif