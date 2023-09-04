#ifndef ARCRANION_LOGICAL_H
#define ARCRANION_LOGICAL_H

namespace Arcranion::Vulkan::Device {
    class Logical;
}

#include "arcranion/graphics/vulkan/device/physical.h"

namespace Arcranion::Vulkan::Device {
    class Logical {
    private:
        VkDevice _handle = VK_NULL_HANDLE;
        VkQueue graphicsQueue = VK_NULL_HANDLE;
        VkQueue presentQueue = VK_NULL_HANDLE;
    public:
        Arcranion::Vulkan::Device::Physical* physicalDevice;

        Logical() = default;
        Logical(Arcranion::Vulkan::Device::Physical* device);

        VkDevice handle();
        Logical* pointer();

        void create(Arcranion::Vulkan::Instance* instance, Arcranion::Vulkan::Surface* surface);
        void destroy();
    };
}

#endif