#include "instance.h"

namespace Arcranion {
    Vulkan::Instance::Instance(VkInstance instance, InstanceConfiguration* configuration) {
        this->instance = instance;
        this->configuration = configuration;
    }

    Vulkan::Instance Vulkan::Instance::create(Vulkan::InstanceConfiguration configuration) {
        auto description = configuration.applicationDescription;

        // Configure application info
        VkApplicationInfo applicationInfo{
                .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pApplicationName = description.name.c_str(),
                .applicationVersion = description.version.makeVulkanVersion(),

                .pEngineName = Arcranion::Globals::Engine::engineName.c_str()
        };

        // Configure create info
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &applicationInfo;

        // Configure extensions
        auto requiredExtensions = Arcranion::GLFW::getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();

        // Configure validation layer if it is enabled
        if(configuration.useValidationLayer) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(Arcranion::Globals::Graphics::vulkanValidationLayers.size());
            createInfo.ppEnabledLayerNames = Arcranion::Globals::Graphics::vulkanValidationLayers.data();

            // Configure debug create info
            VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
            debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            debugCreateInfo.messageSeverity =
                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            debugCreateInfo.messageType =
                    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            debugCreateInfo.pfnUserCallback = configuration.debugMessengerCallback;

            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
        } else {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        VkInstance vkInstance;
        VkResult result = vkCreateInstance(&createInfo, nullptr, &vkInstance);
        int resultNumber = static_cast<std::underlying_type<VkResult>::type>(result);

        if(result != VK_SUCCESS) {
            throw std::runtime_error(std::format("Failed to create a Vulkan instance: {}", resultNumber));
        }

        return {vkInstance, &configuration};
    }

    void Vulkan::Instance::configureDebugging() {
        if (!configuration->useValidationLayer) return;
        if (!Debugging::isValidationLayerSupported()) return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        if(configuration->debugMessengerCallback != nullptr)
            createInfo.pfnUserCallback = configuration->debugMessengerCallback;

        auto createMessengerProc =
                (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

        if(createMessengerProc == nullptr) {
            throw std::runtime_error("Vulkan error: Extension not present");
        }

        VkResult result = createMessengerProc(instance, &createInfo, nullptr, &debugMessenger);
        if(result != VK_SUCCESS) {
            throw std::runtime_error("Vulkan Error: Failed to configure debugging");
        }
    }

    void Vulkan::Instance::disposeDebugging() {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

        if(func != nullptr) {
            func(instance, debugMessenger, nullptr);
        }
    }

    void Vulkan::Instance::configure() {
        configureDebugging();
    }

    void Vulkan::Instance::dispose() {
        disposeDebugging();
    }

    std::vector<Vulkan::Device::Physical> Vulkan::Instance::enumeratePhysicalDevices() {
        unsigned int count = 0;
        vkEnumeratePhysicalDevices(instance, &count, nullptr);

        std::vector<VkPhysicalDevice> devices(count);
        vkEnumeratePhysicalDevices(instance, &count, devices.data());

        std::vector<Vulkan::Device::Physical> objects;
        for(auto device : devices) {
            objects.push_back(Vulkan::Device::Physical(device));
        }

        return objects;
    }
}