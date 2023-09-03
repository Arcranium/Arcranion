#include "arcranion/graphics/glfw.h"
#include "arcranion/window/window.h"
#include "arcranion/graphics/vulkan/instance.h"
#include "arcranion/application/description.h"
#include "arcranion/graphics/vulkan/device/queue_family.h"
#include "arcranion/graphics/vulkan/device/physical.h"
#include "arcranion/graphics/vulkan/device/logical.h"
#include "arcranion/graphics/vulkan/device/swap_chain.h"
#include "arcranion/graphics/vulkan/shader/shader_module.h"
#include "arcranion/graphics/vulkan/render_pass.h"
#include "arcranion/graphics/vulkan/pipeline.h"
#include "arcranion/util/file.h"

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
    void*                                            pUserData
) {
    auto logger = spdlog::get("ValidationLayer");
    logger->trace("{}", pCallbackData->pMessage);

    return VK_FALSE;
}

int main() {
    logger_t logger = spdlog::stdout_color_mt("BasicApplication");
    spdlog::stdout_color_mt("ValidationLayer")->set_level(spdlog::level::trace);

    Arcranion::GLFW::initialize();

    auto window = Arcranion::Window::create({
        .width = 100,
        .height = 100
    });

    Arcranion::ApplicationDescription applicationDescription{
        .name = "BasicApplication",
        .version = Arcranion::Version(1, 0, 0)
    };

    logger->info("Checking validation layer support: {}", Arcranion::Vulkan::Debugging::isValidationLayerSupported());

    auto instance = Arcranion::Vulkan::Instance::create({
        .useValidationLayer = true,
        .applicationDescription = applicationDescription,
        .debugMessengerCallback = debugCallback,
    });

    auto surface = Arcranion::Vulkan::Surface(&instance, &window);
    surface.create();

    Arcranion::Vulkan::Device::Physical* physicalDevice = Arcranion::Vulkan::Device::Physical::bestDevice(&instance, &surface).pointer();
    logger->info(
        "Found device: {} [{}] (Type {})",
        physicalDevice->properties().deviceName,
        physicalDevice->properties().deviceID,
        Arcranion::Vulkan::Device::Physical::deviceTypeAsString(physicalDevice->properties())
    );

    Arcranion::Vulkan::Device::Logical* device = Arcranion::Vulkan::Device::Logical(physicalDevice).pointer();

    device->create(&instance, &surface);

    auto swapchainInfo = physicalDevice->swapChainSupport(&surface);

    auto swapchain = Arcranion::Vulkan::Device::Swapchain(swapchainInfo);

    swapchain.create(device, &window, &surface);
    swapchain.updateImages();
    swapchain.createImageViews();

    logger->info("Loading shaders");
    auto vertexShaderModule = Arcranion::Vulkan::ShaderModule(device, Arcranion::File::readText("D:\\Projects\\Arcranion\\test\\shaders\\compiled\\shader.vert.spv"));
    auto fragmentShaderModule = Arcranion::Vulkan::ShaderModule(device, Arcranion::File::readText("D:\\Projects\\Arcranion\\test\\shaders\\compiled\\shader.frag.spv"));
    vertexShaderModule.create();
    fragmentShaderModule.create();

    std::vector shaderStages = {
        vertexShaderModule.createStageInfo(Arcranion::Vulkan::ShaderType::VERTEX, "main"),
        fragmentShaderModule.createStageInfo(Arcranion::Vulkan::ShaderType::FRAGMENT, "main")
    };

    auto renderPass = Arcranion::Vulkan::RenderPass(&swapchain);
    renderPass.create();

    auto pipelineInfo = Arcranion::Vulkan::PipelineInformation {
        .swapchain = &swapchain,
        .renderPass = &renderPass,
        .shaderStages = shaderStages
    };

    auto pipeline = Arcranion::Vulkan::Pipeline(pipelineInfo);
    pipeline.createLayout();
    pipeline.create();

    while (!window.shouldClose())
    {
        Arcranion::GLFW::pollEvents();
    }
    
    // Cleanup
    try {
        vertexShaderModule.destroy();
        fragmentShaderModule.destroy();

        pipeline.destroy();

        renderPass.destroy();

        swapchain.destroy();
        device->destroy();
        instance.disposeDebugging();
        surface.destroy();
        instance.dispose();
        window.destroy();
    } catch(std::exception e) {
        logger->critical("Failed to cleanup: {}", e.what());
    }

    Arcranion::GLFW::terminate();

    return 0;
}