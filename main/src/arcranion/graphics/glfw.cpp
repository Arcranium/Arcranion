#include "glfw.h"

namespace Arcranion {
    logger_t GLFW::logger = spdlog::stdout_color_mt("Graphics/GLFW");

    bool GLFW::isInitialized = false;

    void GLFW::initialize() {
        if(isInitialized) {
            logger->warn("[Initialize] Already initialized, aborting");
            return;
        }

        if(!glfwInit()) {
            logger->error("[Initialize] Failed to initialize GLFW");
            throw std::runtime_error("Failed to initialize GLFW");
        }

        logger->info("[Initialize] Initialization success");
        isInitialized = true;
    }

    void GLFW::terminate() {
        if(!isInitialized) {
            logger->warn("[Terminate] Not yet initialized, aborting");
            return;
        }

        glfwTerminate();

        logger->info("[Terminate] Termination success");
        isInitialized = false;
    }

    void GLFW::pollEvents() {
        glfwPollEvents();
    }

    std::vector<const char*> GLFW::getRequiredExtensions() {
        unsigned int glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        return std::vector<const char*> { glfwExtensions, glfwExtensions + glfwExtensionCount };
    }
}