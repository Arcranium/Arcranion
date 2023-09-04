#ifndef ARCRANION_COMMAND_BUFFER_H
#define ARCRANION_COMMAND_BUFFER_H

#include "arcranion/internal/include_global.h"
#include "arcranion/graphics/vulkan/command_pool.h"
#include "arcranion/graphics/vulkan/device/logical.h"

namespace Arcranion::Vulkan {
    class CommandBuffer {
        enum Level;
        enum Flag;

    private:
        VkCommandBuffer _handle = VK_NULL_HANDLE;

        Device::Logical* device;
        CommandPool* commandPool;
        Level level;
        Flag flag;
    public:
        void allocate();
        void begin();
        void end();

        enum Level {
            PRIMARY = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            SECONDARY = VK_COMMAND_BUFFER_LEVEL_SECONDARY,
            MAX_ENUM = VK_COMMAND_BUFFER_LEVEL_MAX_ENUM
        };

        enum Flag {
            NONE = 0,
            ONE_TIME_SUBMIT = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
            RENDER_PASS_CONTINUE = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT,
            SIMULTANEOUS_USE = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
        };
    };
}

#endif