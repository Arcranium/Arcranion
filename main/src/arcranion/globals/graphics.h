#ifndef ARCRANION_GRAPHICS_H
#define ARCRANION_GRAPHICS_H

#include "arcranion/internal/include_global.h"

namespace Arcranion::Globals::Graphics {
    // Vulkan
    const std::vector<const char*> vulkanValidationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
}

#endif