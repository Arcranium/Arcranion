#ifndef ARCRANION_PHYSICAL_H
#define ARCRANION_PHYSICAL_H

namespace Arcranion::Vulkan::Device {
    class Physical;
    class SwapchainInformation;
    class Swapchain;
}

#include "arcranion/internal/include_global.h"
#include "arcranion/graphics/vulkan/instance.h"
#include "arcranion/graphics/vulkan/device/queue_family.h"
#include "arcranion/graphics/vulkan/surface.h"
#include "arcranion/graphics/vulkan/device/logical.h"
#include "arcranion/graphics/vulkan/device/swap_chain.h"

namespace Arcranion::Vulkan::Device {
    class Physical {
    private:
        VkPhysicalDevice _handle = VK_NULL_HANDLE;
    public:
        Physical(VkPhysicalDevice handle);

        VkPhysicalDevice handle();
        Physical* pointer();

        VkPhysicalDeviceProperties properties();
        VkPhysicalDeviceFeatures features();

        Arcranion::Vulkan::Device::QueueFamilyIndices queueFamilies(Arcranion::Vulkan::Surface* surface);

        int rating();
        bool suitable(Arcranion::Vulkan::Surface* surface);
        bool deviceExtensionSupported();
        Arcranion::Vulkan::Device::SwapchainInformation swapChainSupport(Arcranion::Vulkan::Surface* surface);
        
        static std::string deviceTypeAsString(VkPhysicalDeviceProperties properties);
        static Arcranion::Vulkan::Device::Physical bestDevice(Arcranion::Vulkan::Instance* instance, Arcranion::Vulkan::Surface* surface);
    };
}

#endif