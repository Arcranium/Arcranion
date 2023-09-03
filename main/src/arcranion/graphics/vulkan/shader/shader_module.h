#ifndef ARCRANION_SHADER_MODULE_H
#define ARCRANION_SHADER_MODULE_H

#include "arcranion/internal/include_global.h"
#include "arcranion/graphics/vulkan/device/logical.h"
#include "arcranion/graphics/vulkan/shader/shader_type.h"

namespace Arcranion::Vulkan {
    class ShaderModule {
    private:
        Device::Logical* device;
        VkShaderModule _handle;

        std::vector<char> code;
    public:
        ShaderModule(Device::Logical* device, std::vector<char> code);

        void create();
        void destroy();

        VkPipelineShaderStageCreateInfo createStageInfo(ShaderType shaderType, std::string name = "main");
    };
}

#endif