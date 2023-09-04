#include "arcranion/graphics/glfw.h"
#include "arcranion/window/window.h"
#include "arcranion/graphics/vulkan/instance.h"
#include "arcranion/application/description.h"
#include "arcranion/graphics/vulkan/device/queue_family.h"
#include "arcranion/graphics/vulkan/device/physical.h"
#include "arcranion/graphics/vulkan/device/logical.h"
#include "arcranion/graphics/vulkan/device/swap_chain.h"
#include "arcranion/graphics/vulkan/shader/shader_module.h"
#include "arcranion/graphics/vulkan/render_pass.h"
#include "arcranion/graphics/vulkan/pipeline.h"
#include "arcranion/graphics/vulkan/framebuffer.h"
#include "arcranion/util/file.h"

class BasicApplication {
public:
    logger_t logger{nullptr};
    logger_t validationLayerLogger{nullptr};

    Arcranion::Window window;
    Arcranion::Vulkan::Instance instance;
    Arcranion::Vulkan::Surface surface;
    Arcranion::Vulkan::Device::Physical physicalDevice;
    Arcranion::Vulkan::Device::Logical device;
    Arcranion::Vulkan::Device::Swapchain swapchain;

    Arcranion::Vulkan::ShaderModule vertexShaderModule{};
    Arcranion::Vulkan::ShaderModule fragmentShaderModule{};
    
    Arcranion::Vulkan::RenderPass renderPass{};
    Arcranion::Vulkan::Pipeline graphicsPipeline{};
    Arcranion::Vulkan::Framebuffers framebuffers{};

    void run() {
        init();
        this->logger->info("Init phase finished, running the main loop");

        loop();
        this->logger->info("Loop finished, cleaning up");
        
        cleanup();
    }

    void init() {
        // Init loggers
        this->logger = spdlog::stdout_color_mt("BasicApplication");
        this->validationLayerLogger = spdlog::stdout_color_mt("ValidationLayer");
        this->validationLayerLogger->set_level(spdlog::level::trace);

        Arcranion::GLFW::initialize();

        try {
            createWindow();
            createInstance();
            createSurface();
            createDevice();
            createSwapchain();
            createShaderModules();
            createRenderPass();
            createGraphicsPipeline();
            createFramebuffers();
        } catch(std::exception e) {
            this->logger->critical("Failed to initialize: {}", e.what());
        }
    }

    void loop() {
        while (!window.shouldClose())
        {
            Arcranion::GLFW::pollEvents();
        }
    }

    void cleanup() {
        try {
            this->logger->info("Destroying framebuffers");
            this->framebuffers.destroyAll();

            this->logger->info("Destroying shader modules");
            this->vertexShaderModule.destroy();
            this->fragmentShaderModule.destroy();
            
            this->logger->info("Destroying graphics pipeline");
            this->graphicsPipeline.destroy();
            this->logger->info("Destroying render pass");
            this->renderPass.destroy();
            this->logger->info("Destroying swapchain");
            this->swapchain.destroy();
            this->logger->info("Destroying device");
            this->device.destroy();
            
            this->logger->info("Destroying instance debugging");
            this->instance.disposeDebugging();
            this->logger->info("Destroying surface");
            this->surface.destroy();
            this->logger->info("Destroying instance");
            this->instance.dispose();
            this->logger->info("Destroying window");
            this->window.destroy();

            Arcranion::GLFW::terminate();
        } catch(std::exception e) {
            this->logger->critical("Failed to cleanup: {}", e.what());
        }
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
        const VkDebugUtilsMessengerCallbackDataEXT*      pCallbackData,
        void*                                            pUserData
    ) {
        auto logger = spdlog::get("ValidationLayer");
        logger->trace("{}", pCallbackData->pMessage);

        return VK_FALSE;
    }

    void createWindow() {
        this->logger->info("Creating window");

        this->window = Arcranion::Window::create({
            .width = 800,
            .height = 600
        });
    }

    void createInstance() {
        this->logger->info("Creating instance");

        Arcranion::ApplicationDescription applicationDescription{
            .name = "BasicApplication",
            .version = Arcranion::Version(1, 0, 0)
        };

        bool validationLayerSupported = Arcranion::Vulkan::Debugging::isValidationLayerSupported();
        this->logger->info("Checking validation layer support: {}", validationLayerSupported);
        
        this->instance = Arcranion::Vulkan::Instance::create({
            .useValidationLayer = validationLayerSupported,
            .applicationDescription = applicationDescription,
            .debugMessengerCallback = debugCallback,
        });
    }

    void createSurface() {
        this->logger->info("Creating surface");

        this->surface = Arcranion::Vulkan::Surface(&this->instance, &this->window);
        this->surface.create();
    }

    void createDevice() {
        this->logger->info("Picking physical device");

        this->physicalDevice = Arcranion::Vulkan::Device::Physical::bestDevice(&instance, &surface);
        this->logger->info(
            "Found device: {} [{}] (Type {})",
            this->physicalDevice.properties().deviceName,
            this->physicalDevice.properties().deviceID,
            Arcranion::Vulkan::Device::Physical::deviceTypeAsString(this->physicalDevice.properties())
        );

        this->logger->info("Creating device");

        this->device = Arcranion::Vulkan::Device::Logical(&physicalDevice);
        this->device.create(&instance, &surface);
    }

    void createSwapchain() {
        this->logger->info("Creating swapchain");

        auto swapchainInfo = this->physicalDevice.swapChainSupport(&surface);
        
        this->swapchain = Arcranion::Vulkan::Device::Swapchain(swapchainInfo);

        swapchain.create(&this->device, &this->window, &this->surface);
        swapchain.updateImages();
        swapchain.createImageViews();
    }

    void createShaderModules() {
        this->logger->info("Loading shaders");

        this->vertexShaderModule = Arcranion::Vulkan::ShaderModule(&this->device, Arcranion::File::readText("D:\\Projects\\Arcranion\\test\\shaders\\compiled\\shader.vert.spv"));
        this->fragmentShaderModule = Arcranion::Vulkan::ShaderModule(&this->device, Arcranion::File::readText("D:\\Projects\\Arcranion\\test\\shaders\\compiled\\shader.frag.spv"));
        
        this->logger->info("Creating shader modules");

        this->vertexShaderModule.create();
        this->fragmentShaderModule.create();
    }

    void createRenderPass() {
        this->logger->info("Creating render pass");

        this->renderPass = Arcranion::Vulkan::RenderPass(&this->swapchain);
        this->renderPass.create();
    }

    void createGraphicsPipeline() {
        this->logger->info("Creating graphics pipeline");

        std::vector shaderStages = {
            this->vertexShaderModule.createStageInfo(Arcranion::Vulkan::ShaderType::VERTEX, "main"),
            this->fragmentShaderModule.createStageInfo(Arcranion::Vulkan::ShaderType::FRAGMENT, "main")
        };

        auto pipelineInfo = Arcranion::Vulkan::PipelineInformation {
            .swapchain = &swapchain,
            .renderPass = &renderPass,
            .shaderStages = shaderStages
        };

        this->graphicsPipeline = Arcranion::Vulkan::Pipeline(pipelineInfo);
        this->graphicsPipeline.createLayout();
        this->graphicsPipeline.create();
    }

    void createFramebuffers() {
        this->logger->info("Creating framebuffers");

        this->framebuffers = Arcranion::Vulkan::Framebuffers(&this->renderPass);
        this->framebuffers.createAll();
    }
};

int main() {
    BasicApplication().run();
    return 0;
}