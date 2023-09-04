#include "surface.h"

namespace Arcranion::Vulkan {
    Surface::Surface(Arcranion::Vulkan::Instance* instance, Arcranion::Window* window) {
        this->instance = instance;
        this->window = window;
    }

    VkSurfaceKHR Surface::handle() {
        return this->_handle;
    }
    
    void Surface::create() {
        auto result = glfwCreateWindowSurface(instance->handle(), window->handle(), nullptr, &this->_handle);
        if(result != VK_SUCCESS) {
            throw std::runtime_error(fmt::format("Failed to create window surface: {}", result));
        }
    }

    void Surface::destroy() {
        vkDestroySurfaceKHR(instance->handle(), this->_handle, nullptr);
    }
}