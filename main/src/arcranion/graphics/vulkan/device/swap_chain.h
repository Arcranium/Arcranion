#ifndef ARCRANION_SWAP_CHAIN_H
#define ARCRANION_SWAP_CHAIN_H

#include "arcranion/internal/include_global.h"

namespace Arcranion::Vulkan::Device {
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
    
}

#endif