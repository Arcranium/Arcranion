#ifndef ARCRANION_GLFW_H
#define ARCRANION_GLFW_H

#include "arcranion/internal/include_global.h"

namespace Arcranion
{
    class GLFW {
    private:
        static logger_t logger;

        static bool isInitialized;

    public:
        static void initialize();
        static void terminate();
        static void pollEvents();

        static std::vector<const char*> getRequiredExtensions();
    };
}


#endif