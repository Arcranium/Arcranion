#ifndef ARCRANION_COMMAND_POOL_H
#define ARCRANION_COMMAND_POOL_H

#include "arcranion/internal/include_global.h"
#include "arcranion/graphics/vulkan/device/logical.h"

namespace Arcranion::Vulkan {
    class CommandPool {
        enum Flag;

    private:
        VkCommandPool _handle = VK_NULL_HANDLE;
        
        Device::Logical* device = nullptr;
        Flag flag = Flag::MAX_ENUM;
        unsigned int queueFamilyIndex = 0;
    public:
        CommandPool() = default;
        CommandPool(Device::Logical* device, Flag flag, unsigned int queueFamilyIndex);

        VkCommandPool handle();

        void create();
        void destroy();

        enum Flag {
            TRANSIENT_BIT = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
            RESET_COMMAND_BUFFER = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            PROTECTED = VK_COMMAND_POOL_CREATE_PROTECTED_BIT,
            MAX_ENUM = VK_COMMAND_POOL_CREATE_FLAG_BITS_MAX_ENUM
        };
    };
}

#endif