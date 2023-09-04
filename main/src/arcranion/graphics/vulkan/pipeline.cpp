#include "pipeline.h"

namespace Arcranion::Vulkan {
    Pipeline::Pipeline(PipelineInformation information) {
        this->information = information;
        this->logger = spdlog::stdout_color_mt("Vulkan/GraphicsPipeline");
    }

    void Pipeline::createLayout() {
        VkPipelineLayoutCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        createInfo.setLayoutCount = 0;
        createInfo.pSetLayouts = nullptr;
        createInfo.pushConstantRangeCount = 0;
        createInfo.pPushConstantRanges = nullptr;

        if(vkCreatePipelineLayout(this->information.swapchain->device->handle(), &createInfo, nullptr, &this->_layoutHandle) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline layout");
        }
    }

    void Pipeline::destroyLayout() {
        vkDestroyPipelineLayout(this->information.swapchain->device->handle(), this->_layoutHandle, nullptr);
    }

    void Pipeline::create() {
        VkPipelineDynamicStateCreateInfo dynamicState;
        VkPipelineVertexInputStateCreateInfo vertexInputState;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyState;
        VkPipelineRasterizationStateCreateInfo rasterizationState;
        VkPipelineMultisampleStateCreateInfo multisampleState;
        VkPipelineColorBlendAttachmentState colorBlendAttachmentState;
        VkPipelineColorBlendStateCreateInfo colorBlendState;
        VkPipelineViewportStateCreateInfo viewportState;
        VkViewport viewport;
        VkRect2D scissor;
        
        std::vector dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        dynamicState = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            .dynamicStateCount = static_cast<unsigned int>(dynamicStates.size()),
            .pDynamicStates = dynamicStates.data()
        };

        vertexInputState = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .vertexBindingDescriptionCount = 0,
            .pVertexBindingDescriptions = nullptr,
            .vertexAttributeDescriptionCount = 0,
            .pVertexAttributeDescriptions = nullptr
        };

        inputAssemblyState = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE,
        };

        viewport = {
            .x = 0.0f,
            .y = 0.0f,
            .width = (float) this->information.swapchain->extent().width,
            .height = (float) this->information.swapchain->extent().height,
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        };

        scissor = {
            .offset = { 0, 0 },
            .extent = this->information.swapchain->extent()
        };

        viewportState = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .viewportCount = 1,
            .scissorCount = 1
        };

        rasterizationState = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .depthClampEnable = VK_FALSE,
            .rasterizerDiscardEnable = VK_FALSE,
            .polygonMode = VK_POLYGON_MODE_FILL,
            .cullMode = VK_CULL_MODE_BACK_BIT,
            .frontFace = VK_FRONT_FACE_CLOCKWISE,
            .depthBiasEnable = VK_FALSE,
            .depthBiasConstantFactor = 0.0f,
            .depthBiasClamp = 0.0f,
            .depthBiasSlopeFactor = 0.0f,
            .lineWidth = 1.0f
        };

        multisampleState = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
            .sampleShadingEnable = VK_FALSE,
            .minSampleShading = 1.0f,
            .pSampleMask = nullptr,
            .alphaToCoverageEnable = VK_FALSE,
            .alphaToOneEnable = VK_FALSE
        };

        colorBlendAttachmentState = {
            .blendEnable = VK_FALSE,
            .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
            .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
            .colorBlendOp = VK_BLEND_OP_ADD,
            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
            .alphaBlendOp = VK_BLEND_OP_ADD,
            .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
        };

        colorBlendState = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .logicOpEnable = VK_FALSE,
            .logicOp = VK_LOGIC_OP_COPY,
            .attachmentCount = 1,
            .pAttachments = &colorBlendAttachmentState
        };
        colorBlendState.blendConstants[0] = 0.0f;
        colorBlendState.blendConstants[1] = 0.0f;
        colorBlendState.blendConstants[2] = 0.0f;
        colorBlendState.blendConstants[3] = 0.0f;

        VkGraphicsPipelineCreateInfo pipelineInfo{
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .stageCount = static_cast<unsigned int>(this->information.shaderStages.size()),
            .pStages = this->information.shaderStages.data(),
            .pVertexInputState = &vertexInputState,
            .pInputAssemblyState = &inputAssemblyState,
            .pViewportState = &viewportState,
            .pRasterizationState = &rasterizationState,
            .pMultisampleState = &multisampleState,
            .pDepthStencilState = nullptr,
            .pColorBlendState = &colorBlendState,
            .pDynamicState = &dynamicState,
            .layout = this->_layoutHandle,
            .renderPass = this->information.renderPass->handle(),
            .basePipelineHandle = VK_NULL_HANDLE,
            .basePipelineIndex = -1,
        };

        if(vkCreateGraphicsPipelines(
            this->information.swapchain->device->handle(),
            VK_NULL_HANDLE,
            1,
            &pipelineInfo,
            nullptr,
            &this->_handle) != VK_SUCCESS
        ) {
            throw std::runtime_error("Failed to create graphics pipeline");
        }
    }

    void Pipeline::destroy() {
        vkDestroyPipeline(this->information.swapchain->device->handle(), this->_handle, nullptr);
        
        destroyLayout();
    }
}