#ifndef ARCRANION_SURFACE_H
#define ARCRANION_SURFACE_H

namespace Arcranion::Vulkan {
    class Surface;
}

#include "arcranion/internal/include_global.h"
#include "arcranion/graphics/vulkan/instance.h"
#include "arcranion/window/window.h"

namespace Arcranion::Vulkan {
    class Surface {
    private:
        Arcranion::Vulkan::Instance* instance = nullptr;
        Arcranion::Window* window = nullptr;

        VkSurfaceKHR _handle = VK_NULL_HANDLE;
    public:
        Surface() = default;
        Surface(Arcranion::Vulkan::Instance* instance, Arcranion::Window* window);

        VkSurfaceKHR handle();

        void create();
        void destroy();
    };
}

#endif