#ifndef ARCRANION_VERSION_H
#define ARCRANION_VERSION_H

#include "arcranion/internal/include_global.h"

namespace Arcranion {
    struct Version {
    public:
        int major;
        int minor;
        int patch;

        Version(int major, int minor, int patch) {
            this->major = major;
            this->minor = minor;
            this->patch = patch;
        }

        unsigned int makeVulkanVersion();
    };
}

#endif