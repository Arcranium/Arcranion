#ifndef ARCRANION_PHYSICAL_H
#define ARCRANION_PHYSICAL_H

namespace Arcranion::Vulkan::Device {
    class Physical;
}

#include "arcranion/internal/include_global.h"
#include "arcranion/graphics/vulkan/instance.h"

namespace Arcranion::Vulkan::Device {
    class Physical {
    private:
        VkPhysicalDevice handle = VK_NULL_HANDLE;
    public:
        Physical(VkPhysicalDevice handle);

        VkPhysicalDeviceProperties properties();
        VkPhysicalDeviceFeatures features();

        int rating();
        
        static void bestDevice(Arcranion::Vulkan::Instance instance);
    };
}

#endif