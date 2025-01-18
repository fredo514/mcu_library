#ifndef KOOPMAN_H
#define KOOPMAN_H

// for 8bit data
// HD3 up to len=13, then HD2
uint8_t koopman8(uint8_t const* data, size_t len);
// HD4 up to len=5, then HD2
uint8_t koopman8p(puint8_t const* data, size_t len);

// for 16bit data
// HD3 up to len=4092, then HD2
uint16_t koopman16(uint16_t const* data, size_t len);
// HD4 up to len=2048, then HD2
uint16_t koopman16p(uint16_t const* data, size_t len);

// for 32bit data
// HD3 up to len=134217720, then HD2
uint32_t koopman32(uint32_t const* data, size_t len);
// HD4 up to len=134217720, then HD2
uint32_t koopman32(uint32_t const* data, size_t len);

#endif