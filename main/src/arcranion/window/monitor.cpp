#include "monitor.h"

namespace Arcranion {
    Monitor::Monitor(GLFWmonitor* handle) {
        this->_handle = handle;
    }

    GLFWmonitor* Monitor::handle() {
        return this->_handle;
    }

    const GLFWvidmode* Monitor::videoMode() {
        return glfwGetVideoMode(_handle);
    }
}