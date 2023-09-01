#include "arcranion/graphics/glfw.h"
#include "arcranion/window/window.h"
#include "arcranion/graphics/vulkan/instance.h"
#include "arcranion/application/description.h"
#include "arcranion/graphics/vulkan/device/queue_family.h"
#include "arcranion/graphics/vulkan/device/physical.h"
#include "arcranion/graphics/vulkan/device/logical.h"
#include "arcranion/graphics/vulkan/device/swap_chain.h"

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
    void*                                            pUserData
) {
    auto logger = spdlog::get("ValidationLayer");
    logger->error("{}", pCallbackData->pMessage);

    return VK_FALSE;
}

int main() {
    logger_t logger = spdlog::stdout_color_mt("BasicApplication");
    spdlog::stdout_color_mt("ValidationLayer");

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

    logger->info("Reached M1");

    auto instance = Arcranion::Vulkan::Instance::create({
        .useValidationLayer = true,
        .applicationDescription = applicationDescription,
        .debugMessengerCallback = debugCallback,
    });
    logger->info("Reached M2");

    auto surface = Arcranion::Vulkan::Surface(&instance, &window);
    surface.create();

    logger->info("Reached M3");

    Arcranion::Vulkan::Device::Physical* physicalDevice;
    try {
        auto d = Arcranion::Vulkan::Device::Physical::bestDevice(&instance, &surface);
        physicalDevice = &d;
    } catch(std::runtime_error error) {
        logger->critical("Could not pick best device: {}", error.what());
        return -1;
    }
    logger->info(
        "Found device: {} [{}] (Type {})",
        physicalDevice->properties().deviceName,
        physicalDevice->properties().deviceID,
        Arcranion::Vulkan::Device::Physical::deviceTypeAsString(physicalDevice->properties())
    );

    Arcranion::Vulkan::Device::Logical* device;
    try {
        auto d = Arcranion::Vulkan::Device::Logical(physicalDevice);
        device = &d;
    } catch(std::runtime_error error) {
        logger->critical("Failed to create logical devic: {}", error.what());
    }

    device->create(&instance, &surface);

    auto swapchainInfo = physicalDevice->swapChainSupport(&surface);
    
    logger->info("Reached M4");

    auto swapchain = Arcranion::Vulkan::Device::Swapchain(swapchainInfo);
    logger->info("Reached M5");
    try {
        swapchain.create(device, &window, &surface);
        swapchain.updateImages();
        swapchain.createImageViews();

        logger->info("Created Swapchain!");
    } catch(std::runtime_error error) {
        logger->critical("Failed to create swapchain: {}", error.what());
    }

    while (!window.shouldClose())
    {
        Arcranion::GLFW::pollEvents();
    }
    
    // Cleanup
    try {
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