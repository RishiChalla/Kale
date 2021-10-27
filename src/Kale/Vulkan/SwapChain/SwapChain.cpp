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
SwapChain::SwapChain(const Device& device) : support(device.physicalDevice), devicePtr(&device) {
	createSwapChain();
}

/**
 * Creates the swapchain
 */
void SwapChain::createSwapChain() {
	vk::SurfaceFormatKHR format = support.chooseFormat();
	vk::PresentModeKHR presentMode = support.choosePresentMode();
	vk::Extent2D extent = support.chooseSwapExtent();

	uint32_t imageCount = support.capabilities.minImageCount + 1;
	if (support.capabilities.maxImageCount != 0 && imageCount > support.capabilities.maxImageCount)
		imageCount = support.capabilities.maxImageCount;
	
	vk::SwapchainCreateInfoKHR createInfo;
	createInfo.surface = renderer.surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = format.format;
	createInfo.imageColorSpace = format.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

	uint32_t queueFamilyIndices[] = {
		devicePtr->queueIndices.graphicsFamilyIndex.value(),
		devicePtr->queueIndices.presentFamilyIndex.value()
	};

	if (devicePtr->queueIndices.graphicsFamilyIndex == devicePtr->queueIndices.presentFamilyIndex) {
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
	
	swapchain = devicePtr->logicalDevice.createSwapchainKHR(createInfo);
}

/**
 * Creates an uninstantiated SwapChain
 */
SwapChain::SwapChain() {
	// Empty Body
}

/**
 * Copy Constructor
 * @param other Object to copy from
 */
SwapChain::SwapChain(const SwapChain& other) : support(other.support), devicePtr(other.devicePtr) {
	createSwapChain();
}

/**
 * Move Constructor
 * @param other Object to move from
 */
SwapChain::SwapChain(SwapChain&& other) : swapchain(other.swapchain), support(other.support), images(other.images),
	imageViews(other.imageViews), devicePtr(other.devicePtr) {

	other.devicePtr = nullptr;
}

/**
 * Copy Assignment
 * @param other Object to copy from
 */
void SwapChain::operator=(const SwapChain& other) {
	support = other.support;
	devicePtr = other.devicePtr;
	createSwapChain();
}

/**
 * Move Assignment
 * @param other Object to move from
 */
void SwapChain::operator=(SwapChain&& other) {
	swapchain = other.swapchain;
	support = other.support;
	imageViews = other.imageViews;
	images = other.images;
	devicePtr = other.devicePtr;

	other.devicePtr = nullptr;
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
void SwapChain::freeResources() {
	if (devicePtr == nullptr) return;

	for (const vk::ImageView& imageView : imageViews) {
		vkDestroyImageView(devicePtr->logicalDevice, imageView, nullptr);
	}

	vkDestroySwapchainKHR(devicePtr->logicalDevice, swapchain, nullptr);
}