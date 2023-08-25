#include "version.h"

unsigned int Arcranion::Version::makeVulkanVersion() {
    return VK_MAKE_VERSION(major, minor, patch);
}