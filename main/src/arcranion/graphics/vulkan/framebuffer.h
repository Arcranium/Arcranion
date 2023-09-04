#ifndef ARCRANION_FRAMEBUFFER_H
#define ARCRANION_FRAMEBUFFER_H

#include "arcranion/internal/include_global.h"
#include "arcranion/graphics/vulkan/render_pass.h"

namespace Arcranion::Vulkan {
    class Framebuffers {
    private:
        std::vector<VkFramebuffer> _handles;

        RenderPass* renderPass = nullptr;
    public:
        Framebuffers() = default;
        Framebuffers(RenderPass* renderPass);

        VkFramebuffer create(std::vector<VkImageView> attachments);
        
        void createAll();
        void destroyAll();
    };
}

#endif