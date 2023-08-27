#include "queue_family.h"

namespace Arcranion::Vulkan::Device {
    bool QueueFamilyIndices::isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
}