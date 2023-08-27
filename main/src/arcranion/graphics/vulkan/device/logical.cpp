#include "logical.h"

namespace Arcranion::Vulkan::Device {
    Logical::Logical(const Arcranion::Vulkan::Device::Physical& device): physicalDevice(device) {}

    void Logical::create(Arcranion::Vulkan::Instance instance) {
        // Queue create informations
        auto indices = this->physicalDevice.queueFamilies();

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<unsigned int> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        float queuePriority = 1.0f;
        for(unsigned int queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;

            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        
        createInfo.queueCreateInfoCount = static_cast<unsigned int>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        
        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<unsigned int>(Arcranion::Globals::Graphics::deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = Arcranion::Globals::Graphics::deviceExtensions.data();

        if(instance.configuration->useValidationLayer) {
            createInfo.enabledLayerCount = static_cast<unsigned int>(Arcranion::Globals::Graphics::vulkanValidationLayers.size());
            createInfo.ppEnabledLayerNames = Arcranion::Globals::Graphics::vulkanValidationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if(vkCreateDevice(physicalDevice.handle(), &createInfo, nullptr, &handle) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create logical device!");
        }

        // Get queues
        vkGetDeviceQueue(handle, indices.graphicsFamily.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(handle, indices.presentFamily.value(), 0, &presentQueue);
    }

    void Logical::destroy() {
        vkDestroyDevice(handle, nullptr);
    }
}