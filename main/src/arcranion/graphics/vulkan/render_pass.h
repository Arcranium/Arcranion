#ifndef ARCRANION_RENDER_PASS_H
#define ARCRANION_RENDER_PASS_H

#include "arcranion/internal/include_global.h"
#include "arcranion/graphics/vulkan/device/swap_chain.h"

namespace Arcranion::Vulkan {
    class RenderPass {
    private:
        VkRenderPass _handle = VK_NULL_HANDLE;
    public:
        Device::Swapchain* swapchain = nullptr;
        
        RenderPass() = default;
        RenderPass(Device::Swapchain* swapchain);

        VkRenderPass handle();

        void create();
        void destroy();
    };
}

#endif