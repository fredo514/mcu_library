#ifndef COBS_H
#define COBS_H

error_t Cobs_Encode(uint8_t* const dest, size_t dest_len, uint8_t const* const src, size_t src_len);
error_t Cobs_Decode(uint8_t* const dest, size_t dest_len, uint8_t const* const src, size_t src_len);

#endif