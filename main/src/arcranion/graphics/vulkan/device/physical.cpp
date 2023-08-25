#include "physical.h"

namespace Arcranion::Vulkan::Device {
    Physical::Physical(VkPhysicalDevice handle) {
        this->handle = handle;
    }

    VkPhysicalDeviceProperties Physical::properties() {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(this->handle, &properties);
        return properties;
    }

    VkPhysicalDeviceFeatures Physical::features() {
        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(this->handle, &features);
        return features;
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

    void Physical::bestDevice(Arcranion::Vulkan::Instance instance) {
        std::cout << "Point1" << std::endl;
        // auto devices = instance.enumeratePhysicalDevices();


        // std::multimap<int, Arcranion::Vulkan::Device::Physical> candidates;

        // for(auto device : devices) {
        //     candidates.insert(std::make_pair(device.rating(), device));
        // }

        // if(candidates.rbegin()->first > 0) {
        //     // return candidates.rbegin()->second;
        // } else {
        //     throw std::runtime_error("Failed to find a suitable GPU!");
        // }
    }

}