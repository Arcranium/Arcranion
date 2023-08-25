#include "window.h"

namespace Arcranion {
    Window::Window(GLFWwindow* handle) {
        this->handle = handle;
    }

    Window::~Window() {
        destroy();
    }

    bool Window::shouldClose() {
        return glfwWindowShouldClose(handle);
    }

    void Window::shouldClose(bool value) {
        glfwSetWindowShouldClose(handle, value);
    }

    Window::Size Window::size() {
        int width, height;
        glfwGetWindowSize(handle, &width, &height);

        return Window::Size {
            .width = width,
            .height = height
        };
    }

    Window::FrameSize Window::frameSize() {
        int left, top, right, bottom;
        glfwGetWindowFrameSize(handle, &left, &top, &right, &bottom);

        return Window::FrameSize {
            .left = left,
            .top = top,
            .right = right,
            .bottom = bottom
        };
    }

    Window::Size Window::framebufferSize() {
        int width, height;
        glfwGetFramebufferSize(handle, &width, &height);

        return Window::Size {
            .width = width,
            .height = height
        };
    }

    Window::Scale Window::contentScale() {
        float x, y;
        glfwGetWindowContentScale(handle, &x, &y);

        return Window::Scale {
            .x = x,
            .y = y
        };
    }

    void Window::sizeLimits(Window::SizeLimits limits) {
        glfwSetWindowSizeLimits(handle, limits.minWidth, limits.minHeight, limits.maxWidth, limits.maxHeight);
    };

    void Window::aspectRatio(Window::AspectRatio ratio) {
        glfwSetWindowAspectRatio(handle, ratio.numerator, ratio.denominator);
    }

    void Window::position(Window::Coordinates coordinates) {
        glfwSetWindowPos(handle, coordinates.x, coordinates.y);
    }

    Window::Coordinates Window::position() {
        int x, y;
        glfwGetWindowPos(handle, &x, &y);

        return Window::Coordinates {
            .x = x,
            .y = y
        };
    }

    void Window::title(std::string title) {
        glfwSetWindowTitle(handle, title.c_str());
    }

    void Window::icon(std::vector<GLFWimage> images) {
        glfwSetWindowIcon(handle, images.size(), images.data());
    }

    void Window::icon() {
        glfwSetWindowIcon(handle, 0, 0);
    }

    Monitor Window::monitor() {
        return Monitor(glfwGetWindowMonitor(handle));
    }

    void Window::monitor(Monitor monitor) {
        const auto mode = monitor.videoMode();

        glfwSetWindowMonitor(handle, monitor.handle(), 0, 0, mode->width, mode->height, mode->refreshRate);
    }

    void Window::monitor(Monitor monitor, Window::Size size, int refreshRate) {
        glfwSetWindowMonitor(handle, monitor.handle(), 0, 0, size.width, size.height, refreshRate);
    }

    void Window::windowed(Window::Coordinates coordinates, Window::Size size) {
        glfwSetWindowMonitor(handle, 0, coordinates.x, coordinates.y, size.width, size.height, 0);
    }

    void Window::iconify() {
        glfwIconifyWindow(handle);
    }

    void Window::maximize() {
        glfwMaximizeWindow(handle);
    }

    bool Window::iconified() {
        return glfwGetWindowAttrib(handle, GLFW_ICONIFIED);
    }

    bool Window::maximized() {
        return glfwGetWindowAttrib(handle, GLFW_MAXIMIZED);
    }

    void Window::restore() {
        glfwRestoreWindow(handle);
    }

    void Window::visible(bool value) {
        if(value) {
            glfwShowWindow(handle);
        } else {
            glfwHideWindow(handle);
        }
    }

    bool Window::visible() {
        return glfwGetWindowAttrib(handle, GLFW_VISIBLE);
    }

    void Window::focus() {
        glfwFocusWindow(handle);
    }

    bool Window::focused() {
        return glfwGetWindowAttrib(handle, GLFW_FOCUSED);
    }

    void Window::requestAttention() {
        glfwRequestWindowAttention(handle);
    }

    void Window::opacity(float opacity) {
        glfwSetWindowOpacity(handle, opacity);
    }

    float Window::opacity() {
        return glfwGetWindowOpacity(handle);
    }

    // Callbacks
    void Window::closeCallback(GLFWwindowclosefun callback) {
        glfwSetWindowCloseCallback(handle, callback);
    }

    void Window::sizeCallback(GLFWwindowsizefun callback) {
        glfwSetWindowSizeCallback(handle, callback);
    }

    void Window::framebufferSizeCallback(GLFWframebuffersizefun callback) {
        glfwSetFramebufferSizeCallback(handle, callback);
    }

    void Window::contentScaleCallback(GLFWwindowcontentscalefun callback) {
        glfwSetWindowContentScaleCallback(handle, callback);
    }

    void Window::positionCallback(GLFWwindowposfun callback) {
        glfwSetWindowPosCallback(handle, callback);
    }

    void Window::iconifyCallback(GLFWwindowiconifyfun callback) {
        glfwSetWindowIconifyCallback(handle, callback);
    }

    void Window::maximizeCallback(GLFWwindowmaximizefun callback) {
        glfwSetWindowMaximizeCallback(handle, callback);
    }

    void Window::focusCallback(GLFWwindowfocusfun callback) {
        glfwSetWindowFocusCallback(handle, callback);
    }

    void Window::refreshCallback(GLFWwindowrefreshfun callback) {
        glfwSetWindowRefreshCallback(handle, callback);
    }

    // Functions
    void Window::swapBuffers() {
        glfwSwapBuffers(handle);
    }

    void Window::destroy() {
        glfwDestroyWindow(handle);
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