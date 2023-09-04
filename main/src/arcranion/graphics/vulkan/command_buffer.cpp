#include "command_buffer.h"

namespace Arcranion::Vulkan {
    void CommandBuffer::allocate() {
        VkCommandBufferAllocateInfo allocateInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = this->commandPool->handle(),
            .level = static_cast<VkCommandBufferLevel>(this->level),
            .commandBufferCount = 1
        };

        auto result = vkAllocateCommandBuffers(this->device->handle(), &allocateInfo, &this->_handle);
        if(result != VK_SUCCESS) {
            throw std::runtime_error(fmt::format("Failed to allocate command buffers: {}", result));
        }
    }

    void CommandBuffer::begin() {
        VkCommandBufferBeginInfo beginInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags = this->flag,
            .pInheritanceInfo = nullptr
        };

        auto result = vkBeginCommandBuffer(this->_handle, &beginInfo);
        if(result != VK_SUCCESS) {
            throw std::runtime_error(fmt::format("Failed to begin command buffer(Code {})", result));
        }
    }
}