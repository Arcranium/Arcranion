#ifndef ARCRANION_LOGICAL_H
#define ARCRANION_LOGICAL_H

#include "arcranion/graphics/vulkan/device/physical.h"

namespace Arcranion::Vulkan::Device {
    class Logical {
    private:
        VkDevice handle;
        VkQueue graphicsQueue;
        VkQueue presentQueue;

        Arcranion::Vulkan::Device::Physical physicalDevice;
    public:
        Logical(const Arcranion::Vulkan::Device::Physical& device);

        void create(Arcranion::Vulkan::Instance instance);
        void destroy();
    };
}

#endif