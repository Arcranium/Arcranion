#include "swap_chain.h"

namespace Arcranion::Vulkan::Device {
    VkSurfaceFormatKHR SwapchainInformation::chooseSurfaceFormat() {
        for(const auto& availableFormat : this->formats) {
            if(availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB
            && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return this->formats[0];
    }

    VkPresentModeKHR SwapchainInformation::choosePresentMode() {
        for(const auto& availablePresentMode : this->presentModes) {
            if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D SwapchainInformation::chooseExtent(Arcranion::Window* window) {
        if(this->capabilities.currentExtent.width != std::numeric_limits<unsigned int>::max()) {
            return this->capabilities.currentExtent;
        } else {
            auto windowSize = window->size();

            VkExtent2D actualExtent = {
                static_cast<unsigned int>(windowSize.width),
                static_cast<unsigned int>(windowSize.height)
            };

            actualExtent.width = std::clamp(
                actualExtent.width,
                this->capabilities.minImageExtent.width,
                this->capabilities.maxImageExtent.width
            );
            actualExtent.height = std::clamp(
                actualExtent.height,
                this->capabilities.minImageExtent.height,
                this->capabilities.maxImageExtent.height
            );

            return actualExtent;
        }
    }

    Swapchain::Swapchain(SwapchainInformation information) {
        this->information = information;
    }

    VkSwapchainKHR Swapchain::handle() {
        return this->_handle;
    }

    void Swapchain::updateImages() {
        unsigned int imageCount;
        vkGetSwapchainImagesKHR(this->device->handle(), this->_handle, &imageCount, nullptr);
        images.resize(imageCount);
        vkGetSwapchainImagesKHR(this->device->handle(), this->_handle, &imageCount, images.data());
    }

    void Swapchain::create(
        Arcranion::Vulkan::Device::Logical* device,
        Arcranion::Window* window,
        Arcranion::Vulkan::Surface* surface
    ) {
        this->device = device;

        auto surfaceFormat = this->information.chooseSurfaceFormat();
        auto presentMode = this->information.choosePresentMode();
        auto extent = this->information.chooseExtent(window);

        unsigned int imageCount = this->information.capabilities.minImageCount + 1;

        if(this->information.capabilities.maxImageCount > 0 && imageCount > this->information.capabilities.maxImageCount) {
            imageCount = this->information.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface->handle();

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        Arcranion::Vulkan::Device::QueueFamilyIndices indices = device->physicalDevice->queueFamilies(surface);

        if(indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = indices.array();
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        createInfo.preTransform = this->information.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        // Set variables
        this->imageFormat = surfaceFormat.format;
        this->extent = extent;

        if(vkCreateSwapchainKHR(this->device->handle(), &createInfo, nullptr, &this->_handle) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create swap chain");
        }
    }

    void Swapchain::destroy() {
        this->destroyImageViews();
        
        vkDestroySwapchainKHR(this->device->handle(), this->_handle, nullptr);
    }

    void Swapchain::createImageViews() {
        this->imageViews.resize(this->images.size());

        for (size_t i = 0; i < this->images.size(); i++) {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = this->images[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = this->imageFormat;

            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if(vkCreateImageView(this->device->handle(), &createInfo, nullptr, &this->imageViews[i]) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create image view");
            }
        }
    }

    void Swapchain::destroyImageViews() {
        for (auto imageView : this->imageViews) {
            vkDestroyImageView(this->device->handle(), imageView, nullptr);
        }
    }
}