#include "debugging.h"

bool Arcranion::Vulkan::Debugging::isValidationLayerSupported() {
    unsigned int layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (auto layerName : Arcranion::Globals::Graphics::vulkanValidationLayers) {
        bool layerFound = false;

        for(auto layerProperties : availableLayers) {
            if(strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if(!layerFound) {
            return false;
        }
    }

    return true;
}
