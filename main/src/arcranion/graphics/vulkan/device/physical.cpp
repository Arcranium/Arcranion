#include "physical.h"

namespace Arcranion::Vulkan::Device {
    Physical::Physical(VkPhysicalDevice handle) {
        this->_handle = handle;
    }

    VkPhysicalDevice Physical::handle() {
        return this->_handle;
    }

    Physical* Physical::pointer() {
        return this;
    }

    VkPhysicalDeviceProperties Physical::properties() {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(this->_handle, &properties);
        return properties;
    }

    VkPhysicalDeviceFeatures Physical::features() {
        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(this->_handle, &features);
        return features;
    }

    Arcranion::Vulkan::Device::QueueFamilyIndices Physical::queueFamilies(Arcranion::Vulkan::Surface* surface) {
        Arcranion::Vulkan::Device::QueueFamilyIndices indices;
        std::optional<unsigned int> queueFamily;

        unsigned int queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(this->_handle, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(this->_handle, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for(const auto& family : queueFamilies) {
            if(family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(this->_handle, i, surface->handle(), &presentSupport);
            if(presentSupport) {
                indices.presentFamily = i;
            }

            if(indices.isComplete()) break;
            
            i++;
        }

        return indices;
    }

    int Physical::rating() {
        auto properties = this->properties();
        auto features = this->features();

        int score = 0;

        if(properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += 1000;
        }

        score += properties.limits.maxImageDimension2D;

        if(!features.geometryShader) {
            return 0;
        }

        return score;
    }

    bool Physical::suitable(Arcranion::Vulkan::Surface* surface) {
        auto indices = this->queueFamilies(surface);

        auto deviceExtensionSupported = this->deviceExtensionSupported();

        bool swapChainAdequate = false;
        if(deviceExtensionSupported) {
            auto swapChainSupport = this->swapChainSupport(surface);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        return indices.isComplete() && deviceExtensionSupported && swapChainAdequate;
    }

    bool Physical::deviceExtensionSupported() {
        unsigned int extensionCount;
        vkEnumerateDeviceExtensionProperties(this->_handle, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(this->_handle, nullptr, &extensionCount, availableExtensions.data());

        auto deviceExtensions = Arcranion::Globals::Graphics::deviceExtensions;
        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for(const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    Arcranion::Vulkan::Device::SwapchainInformation Physical::swapChainSupport(Arcranion::Vulkan::Surface* surface) {
        Arcranion::Vulkan::Device::SwapchainInformation details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(this->_handle, surface->handle(), &details.capabilities);

        unsigned int formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(this->_handle, surface->handle(), &formatCount, nullptr);

        if(formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(this->_handle, surface->handle(), &formatCount, details.formats.data());
        }

        unsigned int presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(this->_handle, surface->handle(), &presentModeCount, nullptr);

        if(presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(this->_handle, surface->handle(), &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    std::string Physical::deviceTypeAsString(VkPhysicalDeviceProperties properties) {
        auto type = properties.deviceType;

        switch(type) {
            case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_OTHER:
                return "OTHER";
            case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                return "INTEGRATED_GPU";
            case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                return "DISCRETE_GPU";
            case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                return "VIRTUAL_GPU";
            case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU:
                return "CPU";
            case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM:
                return "MAX_ENUM";
            default:
                return "UNKNOWN";
        }
    }

    Arcranion::Vulkan::Device::Physical Physical::bestDevice(Arcranion::Vulkan::Instance* instance, Arcranion::Vulkan::Surface* surface) {
        auto devices = instance->enumeratePhysicalDevices();


        std::multimap<int, Arcranion::Vulkan::Device::Physical> candidates;

        for(auto device : devices) {
            candidates.insert(std::make_pair(device.rating(), device));
        }

        if(candidates.rbegin()->first > 0 && candidates.rbegin()->second.suitable(surface)) {
            return candidates.rbegin()->second;
        } else {
            throw std::runtime_error("Failed to find a suitable GPU!");
        }
    }
}