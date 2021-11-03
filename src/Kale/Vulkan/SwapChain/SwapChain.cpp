/*
   Copyright 2021 Rishi Challa

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "SwapChain.hpp"
#include <Kale/Vulkan/Renderer/Renderer.hpp>
#include <Kale/Vulkan/Device/Device.hpp>

using namespace Kale;
using namespace Kale::Vulkan;

/**
 * Creates a new swap chain given the device to create it from
 * @param device The device to create the swap chain from
 */
SwapChain::SwapChain(Device& device) : ChildResource(device),
	support(device.physicalDevice) {
	createSwapChain();
}

/**
 * Initializes the object
 * @param device 
 */
void SwapChain::init(Device& device) {
	ChildResource::init(device);
	support = SwapChainSupportDetails(device.physicalDevice);
	createSwapChain();
}

/**
 * Creates the swapchain
 */
void SwapChain::createSwapChain() {
	vk::SurfaceFormatKHR swapformat = support.chooseFormat();
	vk::PresentModeKHR presentMode = support.choosePresentMode();
	vk::Extent2D swapextent = support.chooseSwapExtent();

	uint32_t imageCount = support.capabilities.minImageCount + 1;
	if (support.capabilities.maxImageCount != 0 && imageCount > support.capabilities.maxImageCount)
		imageCount = support.capabilities.maxImageCount;
	
	vk::SwapchainCreateInfoKHR createInfo;
	createInfo.surface = renderer.surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = swapformat.format;
	createInfo.imageColorSpace = swapformat.colorSpace;
	createInfo.imageExtent = swapextent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

	uint32_t queueFamilyIndices[] = {
		parentPtr->queueIndices.graphicsFamilyIndex.value(),
		parentPtr->queueIndices.presentFamilyIndex.value()
	};

	if (parentPtr->queueIndices.graphicsFamilyIndex == parentPtr->queueIndices.presentFamilyIndex) {
		createInfo.imageSharingMode = vk::SharingMode::eExclusive;
	}
	else {
		createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}

	createInfo.preTransform = support.capabilities.currentTransform;
	createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = nullptr;
	
	swapchain = parentPtr->logicalDevice.createSwapchainKHR(createInfo);

	// Get swapchain images
	images = parentPtr->logicalDevice.getSwapchainImagesKHR(swapchain);
	extent = swapextent;
	format = swapformat.format;

	createImageViews();
}

/**
 * Creates image views from the images and swapchain
 */
void SwapChain::createImageViews() {
	imageViews.reserve(images.size());
	for (const vk::Image& image : images) {
		vk::ImageSubresourceRange range(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
		vk::ImageViewCreateInfo createInfo(vk::ImageViewCreateFlags(), image, vk::ImageViewType::e2D, format,
			vk::ComponentMapping(), range);
		imageViews.push_back(parentPtr->logicalDevice.createImageView(createInfo));
	}
}

/**
 * Creates the frame buffers from the swap chain images/image views given the render pass
 * @param renderPass The render pass to create from
 */
void SwapChain::createFrameBuffers(const vk::RenderPass& renderPass) {
	// Clear the current frame buffers
	for (const vk::Framebuffer& framebuffer : frameBuffers)
		renderer.device.logicalDevice.destroyFramebuffer(framebuffer);
	frameBuffers.clear();

	// Reserve memory for the frame buffers
	frameBuffers.reserve(imageViews.size());

	// Create the frame buffers from the iamge views
	for (const vk::ImageView& imageView : imageViews) {
		vk::FramebufferCreateInfo createInfo(vk::FramebufferCreateFlags(), renderPass, 1, &imageView,
			extent.width, extent.height, 1);
		frameBuffers.push_back(renderer.device.logicalDevice.createFramebuffer(createInfo));
	}
}

/**
 * Creates an uninstantiated SwapChain
 */
SwapChain::SwapChain() {
	// Empty Body
}

/**
 * Move Constructor
 * @param other Object to move from
 */
SwapChain::SwapChain(SwapChain&& other) : ChildResource(dynamic_cast<ChildResource&&>(other)),
	swapchain(other.swapchain), support(other.support), images(other.images),
	imageViews(other.imageViews), extent(other.extent), format(other.format) {
	other.parentPtr = nullptr;
}

/**
 * Move Assignment
 * @param other Object to move from
 */
void SwapChain::operator=(SwapChain&& other) {
	freeResources();
	ChildResource::operator=(dynamic_cast<ChildResource&&>(other));
	
	swapchain = other.swapchain;
	support = other.support;
	imageViews = other.imageViews;
	images = other.images;
	extent = other.extent;
	format = other.format;

	other.parentPtr = nullptr;
}

/**
 * Frees resources if not already freed
 */
SwapChain::~SwapChain() {
	freeResources();
}

/**
 * Frees resources if not already freed
 */
void SwapChain::freeResources(bool remove) {
	if (parentPtr == nullptr) return;
	
	for (const vk::Framebuffer& framebuffer : frameBuffers)
		renderer.device.logicalDevice.destroyFramebuffer(framebuffer);
	for (const vk::ImageView& imageView : imageViews)
		parentPtr->logicalDevice.destroyImageView(imageView);
	parentPtr->logicalDevice.destroySwapchainKHR(swapchain);
	ChildResource::freeResources(remove);
	parentPtr = nullptr;
}