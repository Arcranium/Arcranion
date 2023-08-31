#ifndef ARCRANION_QUEUE_FAMILY_H
#define ARCRANION_QUEUE_FAMILY_H

#include "arcranion/internal/include_global.h"

namespace Arcranion::Vulkan::Device {
    struct QueueFamilyIndices
    {
        std::optional<unsigned int> graphicsFamily;
        std::optional<unsigned int> presentFamily;

        bool isComplete();
        unsigned int* array();
    };
}

#endif