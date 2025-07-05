#include "complex_cart.h"
#include "unity.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_real(void) {
   complex_cart_t z = {.real = 24.0f, .imag = 42.0f};

   TEST_ASSERT_EQUAL_FLOAT(24.0f, ccart_real(z));
}

void test_imag(void) {
   complex_cart_t z = {.real = 24.0f, .imag = 42.0f};

   TEST_ASSERT_EQUAL_FLOAT(42.0f, ccart_imag(z));
}

void test_mag(void) {
   complex_cart_t z = {.real = 2.0f, .imag = -2.0f};

   TEST_ASSERT_EQUAL_FLOAT(2.828427f, ccart_mag(z));
}

void test_phase(void) {
   complex_cart_t z = {.real = 2.0f, .imag = -2.0f};

   TEST_ASSERT_EQUAL_FLOAT(-0.785398f, ccart_phase(z));
}

void test_mul(void) {
   complex_cart_t z1 = {.real = 1.0f, .imag = -2.0f};
   complex_cart_t z2 = {.real = -3.0f, .imag = 4.0f};

   complex_cart_t z = ccart_mul(z1, z2);

   TEST_ASSERT_EQUAL_FLOAT(5.0f, ccart_real(z));
   TEST_ASSERT_EQUAL_FLOAT(10.0f, ccart_imag(z));
}

void test_div(void) {
   complex_cart_t z1 = {.real = 1.0f, .imag = -2.0f};
   complex_cart_t z2 = {.real = -3.0f, .imag = 4.0f};

   complex_cart_t z = ccart_div(z1, z2);

   TEST_ASSERT_EQUAL_FLOAT(-0.44f, ccart_real(z));
   TEST_ASSERT_EQUAL_FLOAT(0.08f, ccart_imag(z));
}
