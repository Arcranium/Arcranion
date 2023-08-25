#ifndef ARCRANION_MONITOR_H
#define ARCRANION_MONITOR_H

#include "arcranion/internal/include_global.h"

namespace Arcranion {
    class Monitor {
    private:
        GLFWmonitor* _handle;

    public:
        Monitor(GLFWmonitor* handle);

        GLFWmonitor* handle();
        const GLFWvidmode* videoMode();
    };
}

#endif