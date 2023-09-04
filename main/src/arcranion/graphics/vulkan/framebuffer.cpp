#include "framebuffer.h"

namespace Arcranion::Vulkan {
    VkFramebuffer Framebuffers::create(std::vector<VkImageView> attachments) {
        VkFramebufferCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = this->renderPass->handle(),
            .attachmentCount = attachments.size(),
            .pAttachments = attachments.data(),
            .width = this->renderPass->swapchain->extent().width,
            .height = this->renderPass->swapchain->extent().height,
            .layers = 1
        };

        VkFramebuffer framebuffer;
        auto result = vkCreateFramebuffer(this->renderPass->swapchain->device->handle(), &createInfo, nullptr, &framebuffer);

        if(result != VK_SUCCESS) {
            throw std::runtime_error(fmt::format("Failed to create framebuffer (VkResult {})", result));
        }

        return framebuffer;
    }

    void Framebuffers::createAll() {
        this->destroyAll();

        this->_handles.resize(this->renderPass->swapchain->imageViews.size());

        for (size_t i = 0; i < this->renderPass->swapchain->imageViews.size(); i++) {
            std::vector attachments = {
                this->renderPass->swapchain->imageViews[i]
            };

            auto framebuffer = this->create(attachments);
            this->_handles.push_back(framebuffer);
        }
    }

    void Framebuffers::destroyAll() {
        for (auto handle : this->_handles) {
            vkDestroyFramebuffer(this->renderPass->swapchain->device->handle(), handle, nullptr);
        }

        this->_handles.clear();
        this->_handles.resize(0);
    }
}