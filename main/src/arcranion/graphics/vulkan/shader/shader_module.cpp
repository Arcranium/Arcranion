#include "shader_module.h"

namespace Arcranion::Vulkan {
    ShaderModule::ShaderModule(Device::Logical* device, std::vector<char> code) {
        this->device = device;
        this->code = code;
    }

    void ShaderModule::create() {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const unsigned int*>(code.data());

        if(vkCreateShaderModule(this->device->handle(), &createInfo, nullptr, &this->_handle) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create shader module!");
        }
    }

    void ShaderModule::destroy() {
        vkDestroyShaderModule(this->device->handle(), this->_handle, nullptr);
    }

    VkPipelineShaderStageCreateInfo ShaderModule::createStageInfo(ShaderType shaderType, std::string name) {
        VkPipelineShaderStageCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        if(shaderType == ShaderType::VERTEX) createInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        if(shaderType == ShaderType::FRAGMENT) createInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        createInfo.module = this->_handle;
        createInfo.pName = "main";

        return createInfo;
    }
}