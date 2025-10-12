#ifndef CBOR_H
#define CBOR_H

typedef struct cbor_encoder cbor_encoder_t;

typedef struct {
   uint8_t* dest;
   size_t dest_len;
} cbor_config_t;

void Cbor_Encoder_Init(cbor_encoder_t* const encoder, cbor_config_t const* const config);

error_t Cbor_Put_Tstr(cbor_encoder_t* const encoder, char const* const str);
error_t Cbor_Put_Float(cbor_encoder_t* const encoder, float x);

#endif