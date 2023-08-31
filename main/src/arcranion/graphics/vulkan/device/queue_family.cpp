#include "queue_family.h"

namespace Arcranion::Vulkan::Device {
    bool QueueFamilyIndices::isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }

    unsigned int* QueueFamilyIndices::array() {
        unsigned int queueFamilyIndices[] = { this->graphicsFamily.value(), this->presentFamily.value() };
        return queueFamilyIndices;
    }
}