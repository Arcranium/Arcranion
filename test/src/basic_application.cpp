#include "arcranion/graphics/glfw.h"
#include "arcranion/window/window.h"
#include "arcranion/graphics/vulkan/instance.h"
#include "arcranion/application/description.h"

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

    auto instance = Arcranion::Vulkan::Instance::create({
        .applicationDescription = applicationDescription
    });

    Arcranion::Vulkan::Device::Physical::bestDevice(instance);

    std::cout << "Point2" << std::endl;

    while (!window.shouldClose())
    {
        Arcranion::GLFW::pollEvents();
    }
    

    Arcranion::GLFW::terminate();

    return 0;
}