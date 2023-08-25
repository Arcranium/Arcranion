#ifndef ARCRANION_DEBUGGING_H
#define ARCRANION_DEBUGGING_H

#include "arcranion/internal/include_global.h"
#include "arcranion/globals/graphics.h"

namespace Arcranion::Vulkan::Debugging {
    typedef PFN_vkDebugUtilsMessengerCallbackEXT DebugMessengerCallback;

    bool isValidationLayerSupported();
}

#endif