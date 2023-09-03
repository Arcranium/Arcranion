#ifndef ARCRANION_RENDER_PASS_H
#define ARCRANION_RENDER_PASS_H

#include "arcranion/internal/include_global.h"
#include "arcranion/graphics/vulkan/device/swap_chain.h"

namespace Arcranion::Vulkan {
    class RenderPass {
    private:
        VkRenderPass _handle;
        Device::Swapchain* swapchain;
    public:
        RenderPass(Device::Swapchain* swapchain);

        VkRenderPass handle();

        void create();
        void destroy();
    };
}

#endif