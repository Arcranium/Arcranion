#ifndef ARCRANION_LOGICAL_H
#define ARCRANION_LOGICAL_H

namespace Arcranion::Vulkan::Device {
    class Logical;
}

#include "arcranion/graphics/vulkan/device/physical.h"

namespace Arcranion::Vulkan::Device {
    class Logical {
    private:
        VkDevice _handle;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
    public:
        Arcranion::Vulkan::Device::Physical* physicalDevice;

        Logical(Arcranion::Vulkan::Device::Physical* device);

        VkDevice handle();
        Logical* pointer();

        void create(Arcranion::Vulkan::Instance* instance, Arcranion::Vulkan::Surface* surface);
        void destroy();
    };
}

#endif