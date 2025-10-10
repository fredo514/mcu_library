#ifndef CBOR_H
#define CBOR_H

typedef enum {
   CBOR_TYPE_UINT = 0x0,
   CBOR_TYPE_INT = 0x1,
   CBOR_TYPE_BYTE_STR = 0x2,
   CBOR_TYPE_UTF8_STR = 0x3,
   CBOR_TYPE_TUPLE = 0x4,    // mixed type array
   CBOR_TYPE_MAP = 0x5,      // key-value pairs
   CBOR_TYPE_TAG = 0x6,      // see cbor_tag_t
   CBOR_TYPE_SPECIAL = 0x7,  // see cbor_special_t
} cbor_type_t;

typedef enum {} cbor_tag_t;

typedef enum {} cbor_special_t;

error_t Cbor_Encode(uint8_t* const dest, size_t dest_len, uint8_t const* const src, size_t src_len);
error_t Cbor_Decode(uint8_t* const dest, size_t dest_len, uint8_t const* const src, size_t src_len);

#endif