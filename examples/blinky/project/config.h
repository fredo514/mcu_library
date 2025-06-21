#include "version.h"

#define STM32F415xx

#define VERSION_MAJOR      (1)
#define VERSION_MINOR      (0)
#define VERSION_PATCH      (0)
#define VERSION_TYPE        (IMAGE_RELEASE_ALPHA)
#if !defined(VERSION_BUILD)
#define VERSION_BUILD      (0)
#endif

#define HARDWARE_VERSION_MIN (1)  // earliest hardware revision compatible

extern const version_t version;

const version_t version = {
    .major = VERSION_MAJOR, .minor = VERSION_MINOR, .rev = VERSION_PATCH, .release_type = VERSION_TYPE, .build = VERSION_BUILD
};