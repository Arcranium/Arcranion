#include "window.h"

namespace Arcranion {
    Window::Window(GLFWwindow* handle) {
        this->_handle = handle;
    }

    GLFWwindow* Window::handle() {
        return this->_handle;
    }

    bool Window::shouldClose() {
        return glfwWindowShouldClose(_handle);
    }

    void Window::shouldClose(bool value) {
        glfwSetWindowShouldClose(_handle, value);
    }

    Window::Size Window::size() {
        int width, height;
        glfwGetWindowSize(_handle, &width, &height);

        return Window::Size {
            .width = width,
            .height = height
        };
    }

    Window::FrameSize Window::frameSize() {
        int left, top, right, bottom;
        glfwGetWindowFrameSize(_handle, &left, &top, &right, &bottom);

        return Window::FrameSize {
            .left = left,
            .top = top,
            .right = right,
            .bottom = bottom
        };
    }

    Window::Size Window::framebufferSize() {
        int width, height;
        glfwGetFramebufferSize(_handle, &width, &height);

        return Window::Size {
            .width = width,
            .height = height
        };
    }

    Window::Scale Window::contentScale() {
        float x, y;
        glfwGetWindowContentScale(_handle, &x, &y);

        return Window::Scale {
            .x = x,
            .y = y
        };
    }

    void Window::sizeLimits(Window::SizeLimits limits) {
        glfwSetWindowSizeLimits(_handle, limits.minWidth, limits.minHeight, limits.maxWidth, limits.maxHeight);
    };

    void Window::aspectRatio(Window::AspectRatio ratio) {
        glfwSetWindowAspectRatio(_handle, ratio.numerator, ratio.denominator);
    }

    void Window::position(Window::Coordinates coordinates) {
        glfwSetWindowPos(_handle, coordinates.x, coordinates.y);
    }

    Window::Coordinates Window::position() {
        int x, y;
        glfwGetWindowPos(_handle, &x, &y);

        return Window::Coordinates {
            .x = x,
            .y = y
        };
    }

    void Window::title(std::string title) {
        glfwSetWindowTitle(_handle, title.c_str());
    }

    void Window::icon(std::vector<GLFWimage> images) {
        glfwSetWindowIcon(_handle, images.size(), images.data());
    }

    void Window::icon() {
        glfwSetWindowIcon(_handle, 0, 0);
    }

    Monitor Window::monitor() {
        return Monitor(glfwGetWindowMonitor(_handle));
    }

    void Window::monitor(Monitor monitor) {
        const auto mode = monitor.videoMode();

        glfwSetWindowMonitor(_handle, monitor.handle(), 0, 0, mode->width, mode->height, mode->refreshRate);
    }

    void Window::monitor(Monitor monitor, Window::Size size, int refreshRate) {
        glfwSetWindowMonitor(_handle, monitor.handle(), 0, 0, size.width, size.height, refreshRate);
    }

    void Window::windowed(Window::Coordinates coordinates, Window::Size size) {
        glfwSetWindowMonitor(_handle, 0, coordinates.x, coordinates.y, size.width, size.height, 0);
    }

    void Window::iconify() {
        glfwIconifyWindow(_handle);
    }

    void Window::maximize() {
        glfwMaximizeWindow(_handle);
    }

    bool Window::iconified() {
        return glfwGetWindowAttrib(_handle, GLFW_ICONIFIED);
    }

    bool Window::maximized() {
        return glfwGetWindowAttrib(_handle, GLFW_MAXIMIZED);
    }

    void Window::restore() {
        glfwRestoreWindow(_handle);
    }

    void Window::visible(bool value) {
        if(value) {
            glfwShowWindow(_handle);
        } else {
            glfwHideWindow(_handle);
        }
    }

    bool Window::visible() {
        return glfwGetWindowAttrib(_handle, GLFW_VISIBLE);
    }

    void Window::focus() {
        glfwFocusWindow(_handle);
    }

    bool Window::focused() {
        return glfwGetWindowAttrib(_handle, GLFW_FOCUSED);
    }

    void Window::requestAttention() {
        glfwRequestWindowAttention(_handle);
    }

    void Window::opacity(float opacity) {
        glfwSetWindowOpacity(_handle, opacity);
    }

    float Window::opacity() {
        return glfwGetWindowOpacity(_handle);
    }

    // Callbacks
    void Window::closeCallback(GLFWwindowclosefun callback) {
        glfwSetWindowCloseCallback(_handle, callback);
    }

    void Window::sizeCallback(GLFWwindowsizefun callback) {
        glfwSetWindowSizeCallback(_handle, callback);
    }

    void Window::framebufferSizeCallback(GLFWframebuffersizefun callback) {
        glfwSetFramebufferSizeCallback(_handle, callback);
    }

    void Window::contentScaleCallback(GLFWwindowcontentscalefun callback) {
        glfwSetWindowContentScaleCallback(_handle, callback);
    }

    void Window::positionCallback(GLFWwindowposfun callback) {
        glfwSetWindowPosCallback(_handle, callback);
    }

    void Window::iconifyCallback(GLFWwindowiconifyfun callback) {
        glfwSetWindowIconifyCallback(_handle, callback);
    }

    void Window::maximizeCallback(GLFWwindowmaximizefun callback) {
        glfwSetWindowMaximizeCallback(_handle, callback);
    }

    void Window::focusCallback(GLFWwindowfocusfun callback) {
        glfwSetWindowFocusCallback(_handle, callback);
    }

    void Window::refreshCallback(GLFWwindowrefreshfun callback) {
        glfwSetWindowRefreshCallback(_handle, callback);
    }

    // Functions
    void Window::swapBuffers() {
        glfwSwapBuffers(_handle);
    }

    void Window::destroy() {
        glfwDestroyWindow(_handle);
    }

    Window Window::create(CreateOptions options) {
        // We use Vulkan, so leave this GLFW_NO_API
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        glfwWindowHint(GLFW_RESIZABLE, options.resizable);

        GLFWwindow* handle;
        
        if(options.monitor != nullptr) {
            handle = glfwCreateWindow(options.width, options.height, options.title.c_str(), options.monitor->handle(), nullptr);
        } else {
            handle = glfwCreateWindow(options.width, options.height, options.title.c_str(), nullptr, nullptr);
        }

        return Window(handle);
    }
}