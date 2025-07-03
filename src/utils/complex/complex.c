#include "complex.h"

float creal(complex_t z) {
    return z.real;
}

float cimag(complex_t z) {
    return z.imag;
}

float cmag(complex_t z) {
    return sqrtf((z.real * z.real) + (z.imag * z.imag));
}

float cphase(complex_t z) {
    return atan2f(z.imag / z.real);
}

complex_t cmul(complex_t z1, complex_t z2) {
    complex_t z = {0,0};

    z.real = (creal(z1) * creal(z2)) - (cimag(z1) * cimag(z2));
    z.imag = (creal(z1) * cimag(z2)) + (cimag(z1) * creal(z2));

    return z;
}

complex_t cdiv(complex_t num, complex_t denum){
    complex_t z = {0,0};

    float denum_conv = (creal(denum) * creal(denum)) + (cimag(denum) * cimag(denum));
    z.real = ((crealf(num) * crealf(denum)) + (cimagf(num) * cimagf(denum))) / denum_conv;
    z.imag = ((cimagf(num) * crealf(denum)) - (crealf(num) * cimagf(denum))) / denum_conv;
    
    return z;
}