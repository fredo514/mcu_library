#ifndef COMPLEX_CART_H
#define COMPLEX_CART_H

#include <float.h>

typedef struct {
   float real;
   float imag;
} complex_cart_t;

float ccart_real(complex_cart_t z);
float ccart_imag(complex_cart_t z);
float ccart_mag(complex_cart_t z);
float ccart_phase(complex_cart_t z);

complex_cart_t ccart_mul(complex_cart_t z1, complex_cart_t z2);
complex_cart_t ccart_div(complex_cart_t num, complex_cart_t denum);
// complex_cart_t cconj(complex_cart_t z);
// complex_cart_t cexp(complex_cart_t z);
// complex_cart_t clog(complex_cart_t z);
// complex_cart_t cpow(complex_cart_t z, float exp);
// complex_cart_t csqrt(complex_cart_t z);

// complex_cart_t csin(complex_cart_t z);
// complex_cart_t casin(complex_cart_t z);
// complex_cart_t csinh(complex_cart_t z);
// complex_cart_t casinh(complex_cart_t z);
// complex_cart_t ccos(complex_cart_t z);
// complex_cart_t cacos(complex_cart_t z);
// complex_cart_t ccosh(complex_cart_t z);
// complex_cart_t cacosh(complex_cart_t z);
// complex_cart_t ctan(complex_cart_t z);
// complex_cart_t catan(complex_cart_t z);
// complex_cart_t ctanh(complex_cart_t z);
// complex_cart_t catanh(complex_cart_t z);

#endif