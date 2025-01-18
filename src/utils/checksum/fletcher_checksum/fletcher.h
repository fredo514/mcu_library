#ifndef FLETCHER_H
#define FLETCHER_H

// for 8bit data
uint16_t fletcher16(uint8_t const* data, size_t len);

// for 16bit data
uint32_t fletcher32(uint16_t const* data, size_t len);

// for 32bit data
uint64_t fletcher64(uint32_t const* data, size_t len);

#endif