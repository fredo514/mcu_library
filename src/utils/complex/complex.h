#ifndef COMPLEX_H
#define COMPLEX_H

#include <float.h>

typedef struct {
    float real;
    float imag;
} complex_t;

float creal(complex_t z);
float cimag(complex_t z);
float cmag(complex_t z);
float cphase(complex_t z);

complex_t cmul(complex_t z1, complex_t z2);
complex_t cdiv(complex_t num, complex_t denum);
complex_t cconj(complex_t z);
complex_t cexp(complex_t z);
complex_t clog(complex_t z);
complex_t cpow(complex_t z, float exp);
complex_t csqrt(complex_t z);

complex_t csin(complex_t z);
complex_t casin(complex_t z);
complex_t csinh(complex_t z);
complex_t casinh(complex_t z);
complex_t ccos(complex_t z);
complex_t cacos(complex_t z);
complex_t ccosh(complex_t z);
complex_t cacosh(complex_t z);
complex_t ctan(complex_t z);
complex_t catan(complex_t z);
complex_t ctanh(complex_t z);
complex_t catanh(complex_t z);

#endif