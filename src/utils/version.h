#include "stdint.h"

/**
  Software version release type Enumeration

  @Summary
    Defines the Software version release type enumeration.

  @Description
    This enumeration defines the Software version release type.
*/
typedef enum {
  IMAGE_RELEASE_PRODUCTION = 0,
  IMAGE_RELEASE_CANDIDATE,
  IMAGE_RELEASE_ALPHA,
  IMAGE_RELEASE_TEST,
  IMAGE_RELEASE_CUSTOM,
  IMAGE_RELEASE_MAX
} image_release_t;

/**
  Software Version structure

  @Summary
    Defines the Software Version structure.

  @Description
    This Data structure is to implement a Software Version.
 
  @Members
    major     - major version
    minor     - minor version
    patch     - patch version
    release_type         - release type
    build  - build version
*/
typedef struct __attribute__((packed)) {
  uint8_t major;
  uint8_t minor;
  uint16_t rev;
  image_release_t release_type;
  uint32_t build;
} version_t;