#ifndef ARCRANION_PIPELINE_H
#define ARCRANION_PIPELINE_H

#include "arcranion/internal/include_global.h"
#include "arcranion/graphics/vulkan/device/logical.h"
#include "arcranion/graphics/vulkan/shader/shader_module.h"
#include "arcranion/graphics/vulkan/render_pass.h"

namespace Arcranion::Vulkan {
    struct PipelineInformation {
        Device::Swapchain* swapchain;
        RenderPass* renderPass;

        std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    };

    class Pipeline {
    private:
        PipelineInformation information;

        VkPipelineLayout _layoutHandle;
        VkPipeline _handle;
    public:
        explicit Pipeline(PipelineInformation information);

        void createLayout();
        void destroyLayout();

        /// @brief Create graphics pipeline with given informations
        void create();
        /// @brief Destroy the created graphics pipeline
        void destroy();
    };
}

#endif