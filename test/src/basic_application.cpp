#include "arcranion/graphics/glfw.h"
#include "arcranion/window/window.h"
#include "arcranion/graphics/vulkan/instance.h"
#include "arcranion/application/description.h"
#include "arcranion/graphics/vulkan/device/queue_family.h"
#include "arcranion/graphics/vulkan/device/swap_chain.h"

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
    void*                                            pUserData
) {
    std::cout << "Debug: " << pCallbackData->pMessage;

    return VK_FALSE;
}

int main() {
    logger_t logger = spdlog::stdout_color_mt("BasicApplication");

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
        .applicationDescription = applicationDescription
    });
    logger->info("Reached M2");

    auto surface = Arcranion::Vulkan::Surface(&instance, &window);
    surface.create();

    logger->info("Reached M3");

    Arcranion::Vulkan::Device::Physical* device;
    try {
        auto d = Arcranion::Vulkan::Device::Physical::bestDevice(instance, surface);
        device = &d;
    } catch(std::runtime_error error) {
        logger->critical("Could not pick best device: {}", error.what());
    }
    logger->info(
        "Found device: {} [{}] (Type {})",
        device->properties().deviceName,
        device->properties().deviceID,
        Arcranion::Vulkan::Device::Physical::deviceTypeAsString(device->properties())
    );

    while (!window.shouldClose())
    {
        Arcranion::GLFW::pollEvents();
    }
    
    // Cleanup
    surface.destroy();
    instance.dispose();

    Arcranion::GLFW::terminate();

    return 0;
}