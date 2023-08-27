#ifndef ARCRANION_INSTANCE_H
#define ARCRANION_INSTANCE_H

namespace Arcranion::Vulkan {
    struct InstanceConfiguration;
    class Instance;
}

#include "arcranion/internal/include_global.h"
#include "arcranion/graphics/vulkan/debugging.h"
#include "arcranion/application/description.h"
#include "arcranion/globals/engine.h"
#include "arcranion/graphics/glfw.h"
#include "arcranion/graphics/vulkan/device/physical.h"

namespace Arcranion::Vulkan {
    struct InstanceConfiguration {
        bool useValidationLayer = false;
        Arcranion::ApplicationDescription applicationDescription;
        Arcranion::Vulkan::Debugging::DebugMessengerCallback debugMessengerCallback = nullptr;
    };

    class Instance {
    private:
        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger = nullptr;
    public:
        InstanceConfiguration* configuration;
        
        Instance(VkInstance instance, InstanceConfiguration* configuration);

        VkInstance handle();

        void configureDebugging();
        void disposeDebugging();

        void configure();
        void dispose();

        std::vector<Arcranion::Vulkan::Device::Physical> enumeratePhysicalDevices();

        static Instance create(InstanceConfiguration configuration);
    };
}

#endif