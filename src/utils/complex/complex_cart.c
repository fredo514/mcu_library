#include "complex_cart.h"

#include <math.h>

float ccart_real(complex_cart_t z) {
   return z.real;
}

float ccart_imag(complex_cart_t z) {
   return z.imag;
}

float ccart_mag(complex_cart_t z) {
   return sqrtf((z.real * z.real) + (z.imag * z.imag));
}

float ccart_phase(complex_cart_t z) {
   return atan2f(z.imag, z.real);
}

complex_cart_t ccart_mul(complex_cart_t z1, complex_cart_t z2) {
   complex_cart_t z = {.real = 0.0f, .imag = 0.0f};

   z.real = (ccart_real(z1) * ccart_real(z2)) - (ccart_imag(z1) * ccart_imag(z2));
   z.imag = (ccart_real(z1) * ccart_imag(z2)) + (ccart_imag(z1) * ccart_real(z2));

   return z;
}

complex_cart_t ccart_div(complex_cart_t num, complex_cart_t denum) {
   complex_cart_t z = {.real = 0.0f, .imag = 0.0f};

   float denum_conv = (ccart_real(denum) * ccart_real(denum)) + (ccart_imag(denum) * ccart_imag(denum));
   z.real = ((ccart_real(num) * ccart_real(denum)) + (ccart_imag(num) * ccart_imag(denum))) / denum_conv;
   z.imag = ((ccart_imag(num) * ccart_real(denum)) - (ccart_real(num) * ccart_imag(denum))) / denum_conv;

   return z;
}