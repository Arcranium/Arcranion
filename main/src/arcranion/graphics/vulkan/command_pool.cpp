#include "command_pool.h"

namespace Arcranion::Vulkan {
    CommandPool::CommandPool(Device::Logical *device, Flag flag, unsigned int queueFamilyIndex) {
        this->device = device;
        this->flag = flag;
        this->queueFamilyIndex = queueFamilyIndex;
    }

    void CommandPool::create() {
        VkCommandPoolCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .flags = this->flag,
            .queueFamilyIndex = this->queueFamilyIndex
        };

        auto result = vkCreateCommandPool(this->device->handle(), &createInfo, nullptr, &this->_handle);
        if(result != VK_SUCCESS) {
            throw std::runtime_error(fmt::format("Failed to create command pool(Code {})", result));
        }
    }

    void CommandPool::destroy() {
        vkDestroyCommandPool(this->device->handle(), this->_handle, nullptr);
    }
}