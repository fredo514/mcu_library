#include "version.h"

#define STM32F415xx

#define HARDWARE_VERSION_MIN (1)  // earliest hardware revision compatible

extern const version_t version;

const version_t version = {
    .major = 1, .minor = 0, .rev = 0, .release_type = IMAGE_RELEASE_ALPHA
    // build to be populated by build script
};