#ifndef ARCRANION_WINDOW_H
#define ARCRANION_WINDOW_H

#include "arcranion/internal/include_global.h"
#include "arcranion/window/monitor.h"

namespace Arcranion {
    class Window {
        struct Size;
        struct FrameSize;
        struct Scale;
        struct Coordinates;
        struct SizeLimits;
        struct AspectRatio;

        struct CreateOptions;

    private:
        GLFWwindow* _handle;

    public:
        Window() = default;
        Window(GLFWwindow* handle);

        GLFWwindow* handle();

        bool shouldClose();
        void shouldClose(bool value);

        Size      size();
        FrameSize frameSize();
        Size      framebufferSize();
        Scale     contentScale();
        
        void sizeLimits(SizeLimits limits);
        void aspectRatio(AspectRatio ratio);

        void        position(Coordinates coordinates);
        Coordinates position();

        void title(std::string title);
        void icon(std::vector<GLFWimage> images);
        /**
         * @brief Revert to the default icon.
         */
        void icon();

        Monitor monitor();
        /**
         * @brief Set monitor based on the monitor's video mode. 
         */
        void monitor(Monitor monitor);
        void monitor(Monitor monitor, Size size, int refreshRate);

        /**
         * @brief Make the window windowed.
         */
        void windowed(Coordinates coordinates, Size size);

        /**
         * @brief Make the window iconified(minimized).
         */
        void iconify();

        /**
         * @brief Make the window maximized(zoomed);
         */
        void maximize();

        /**
         * @brief Get current iconification state.
         * @return true if the window is iconified(minimized). 
         */
        bool iconified();

        /**
         * @brief Get current maximization state.
         * @return true if the window is maximized(zoomed).
         */
        bool maximized();

        /**
         * @brief Restore the iconified(minimized) or maximized(zoomed) window.
         */
        void restore();

        void visible(bool value);
        bool visible();

        void focus();
        bool focused();

        void requestAttention();

        void opacity(float opacity);
        float opacity();

        // Callbacks
        void closeCallback(GLFWwindowclosefun callback);
        void sizeCallback(GLFWwindowsizefun callback);
        void framebufferSizeCallback(GLFWframebuffersizefun callback);
        void contentScaleCallback(GLFWwindowcontentscalefun callback);
        void positionCallback(GLFWwindowposfun callback);
        void iconifyCallback(GLFWwindowiconifyfun callback);
        void maximizeCallback(GLFWwindowmaximizefun callback);
        void focusCallback(GLFWwindowfocusfun callback);
        void refreshCallback(GLFWwindowrefreshfun callback);

        // Functions
        void swapBuffers();
        void destroy();

        // Static
        static Window create(CreateOptions options);

        // Structures
        struct Size {
            int width;
            int height;
        };

        struct FrameSize {
            int left;
            int top;
            int right;
            int bottom;
        };

        struct Scale {
            float x;
            float y;
        };

        struct Coordinates {
            int x;
            int y;
        };

        /**
         * @brief Struct for window size limits.
         * @details Set values -1 to set it default.
        */
        struct SizeLimits {
            int minWidth = -1;
            int minHeight = -1;
            int maxWidth = -1;
            int maxHeight = -1;
        };

        /**
         * @brief Struct for window aspect ratio.
         * @details Set values -1 to set it default.
         */
        struct AspectRatio {
            int numerator;
            int denominator;
        };

        struct CreateOptions {
            int width;
            int height;

            std::string title;

            Monitor* monitor = nullptr;

            bool resizable = false;
        };
    };
}

#endif