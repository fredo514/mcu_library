#include "stdint.h"

typedef enum {
  IMAGE_RELEASE_PRODUCTION = 0,
  IMAGE_RELEASE_CANDIDATE,
  IMAGE_RELEASE_ALPHA,
  IMAGE_RELEASE_TEST,
  IMAGE_RELEASE_CUSTOM,
  IMAGE_RELEASE_MAX
} image_release_t;

typedef struct __attribute__((packed)) {
  uint8_t major;
  uint8_t minor;
  uint16_t rev;
  uint32_t build;
  image_release_t release_type;
} version_t;