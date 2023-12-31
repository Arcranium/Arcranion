#ifndef ARCRANION_SWAP_CHAIN_H
#define ARCRANION_SWAP_CHAIN_H

namespace Arcranion::Vulkan::Device {
    class SwapchainInformation;
    class Swapchain;
    class Logical;
}

#include "arcranion/internal/include_global.h"
#include "arcranion/window/window.h"
#include "arcranion/graphics/vulkan/surface.h"
#include "arcranion/graphics/vulkan/device/logical.h"

namespace Arcranion::Vulkan::Device {
    class SwapchainInformation {
    public:
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;

        VkSurfaceFormatKHR chooseSurfaceFormat();
        VkPresentModeKHR choosePresentMode();
        VkExtent2D chooseExtent(Arcranion::Window* window);
    };

    class Swapchain {
    private:
        VkSwapchainKHR _handle;
        
        std::vector<VkImage> images;

        VkFormat _imageFormat;
        VkExtent2D _extent;

        std::vector<VkImageView> imageViews;
    public:
        Arcranion::Vulkan::Device::Logical* device;

        SwapchainInformation information;

        Swapchain(SwapchainInformation information);

        VkSwapchainKHR handle();

        void updateImages();
        
        void create(
            Arcranion::Vulkan::Device::Logical* device,
            Arcranion::Window* window,
            Arcranion::Vulkan::Surface* surface
        );
        void destroy();

        void createImageViews();
        void destroyImageViews();

        VkFormat imageFormat();
        VkExtent2D extent();
    };
    
}

#endif