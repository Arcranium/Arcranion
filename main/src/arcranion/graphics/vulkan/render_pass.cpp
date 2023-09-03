#include "render_pass.h"

namespace Arcranion::Vulkan {
    RenderPass::RenderPass(Device::Swapchain* swapchain) {
        this->swapchain = swapchain;
    }

    VkRenderPass RenderPass::handle() {
        return this->_handle;
    }

    void RenderPass::create() {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = swapchain->imageFormat();
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkRenderPassCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        
        createInfo.attachmentCount = 1;
        createInfo.pAttachments = &colorAttachment;

        createInfo.subpassCount = 1;
        createInfo.pSubpasses = &subpass;

        if(vkCreateRenderPass(this->swapchain->device->handle(), &createInfo, nullptr, &this->_handle) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create render pass");
        }
    }

    void RenderPass::destroy() {
        vkDestroyRenderPass(this->swapchain->device->handle(), this->_handle, nullptr);
    }
}