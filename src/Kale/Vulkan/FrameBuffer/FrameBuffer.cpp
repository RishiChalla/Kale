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

#include "FrameBuffer.hpp"

#include <Kale/Vulkan/Renderer/Renderer.hpp>
#include <Kale/Vulkan/SwapChain/SwapChain.hpp>
#include <Kale/Vulkan/Core/Core.hpp>

#ifdef KALE_DEBUG

#include <cassert>

#endif

using namespace Kale;
using namespace Kale::Vulkan;

/**
 * Creates an uninstantiated FrameBuffer
 */
FrameBuffer::FrameBuffer() {
	// Empty Body
}

/**
 * Creates an initialized FrameBuffer
 * @param renderer The renderer this frame buffer will be rendered from
 */
FrameBuffer::FrameBuffer(Renderer& renderer, const Vector2ui32& size) : ChildResource(renderer) {

	// Create the image
	const vk::Extent3D extent(size.x, size.y, 1);
	const vk::ImageCreateInfo imageCreateInfo(vk::ImageCreateFlags(), vk::ImageType::e2D, Core::swapchain.format, extent);
	image = Core::device.logicalDevice.get().createImageUnique(imageCreateInfo);
	
	// Create the image view
	const vk::ImageSubresourceRange range(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
	const vk::ImageViewCreateInfo imageViewCreateInfo(vk::ImageViewCreateFlags(), image.get(), vk::ImageViewType::e2D,
		Core::swapchain.format, vk::ComponentMapping(), range);
	imageView = Core::device.logicalDevice.get().createImageViewUnique(imageViewCreateInfo);

	// Setup the frame buffer
	createFrameBuffer(imageView, {size.x, size.y}, renderer.renderPass);
}

/**
 * Creates an initialized FrameBuffer
 * @param renderer The renderer this frame buffer will be rendered from
 * @param swapchain The swapchain this framebuffer is to be created from
 * @param imageViewIndex the image view index from the swapchain
 */ // TODO - remember to static assert that &swapchain == parentPtr->parentPtr (Our Parent Renderer->SwapChain)
FrameBuffer::FrameBuffer(Renderer& renderer, const SwapChain& swapchain, size_t imageViewIndex) : ChildResource(renderer) {

	// Ensure that the swapchain given is the same as our renderer's parent
	#ifdef KALE_DEBUG
	assert(&swapchain == renderer.getSwapChain());
	#endif

	// create the frame buffer
	createFrameBuffer(swapchain.imageViews[imageViewIndex], swapchain.extent, renderer.renderPass);
}

/**
 * Initializes the object
 * @param renderer The renderer this frame buffer will be rendered from
 */
void FrameBuffer::init(Renderer& renderer, const Vector2ui32& size) {

	ChildResource::init(renderer);

	// Create the image
	const vk::Extent3D extent(size.x, size.y, 1);
	const vk::ImageCreateInfo imageCreateInfo(vk::ImageCreateFlags(), vk::ImageType::e2D, Core::swapchain.format, extent);
	image = Core::device.logicalDevice.get().createImageUnique(imageCreateInfo);
	
	// Create the image view
	const vk::ImageSubresourceRange range(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
	const vk::ImageViewCreateInfo imageViewCreateInfo(vk::ImageViewCreateFlags(), image.get(), vk::ImageViewType::e2D,
		Core::swapchain.format, vk::ComponentMapping(), range);
	imageView = Core::device.logicalDevice.get().createImageViewUnique(imageViewCreateInfo);

	// Setup the frame buffer
	createFrameBuffer(imageView, {size.x, size.y}, renderer.renderPass);
}

/**
 * Initializes the object
 * @param renderer The renderer this frame buffer will be rendered from
 * @param swapchain The swapchain this framebuffer is to be created from
 * @param imageViewIndex the image view index from the swapchain
 */ // TODO - remember to static assert that &swapchain == parentPtr->parentPtr (Our Parent Renderer->SwapChain)
void FrameBuffer::init(Renderer& renderer, const SwapChain& swapchain, size_t imageViewIndex) {

	ChildResource::init(renderer);

	// Ensure that the swapchain given is the same as our renderer's parent
	#ifdef KALE_DEBUG
	assert(&swapchain == renderer.getSwapChain());
	#endif

	// create the frame buffer
	createFrameBuffer(swapchain.imageViews[imageViewIndex], swapchain.extent, renderer.renderPass);
}

/**
 * Creates the frame buffer object
 * @param imageView the image view to create the framebuffer from
 * @param extent The extent of the image view to use
 * @param renderPass The render pass which will be rendering to this frame buffer
 */
void FrameBuffer::createFrameBuffer(const vk::UniqueImageView& imageView, const vk::Extent2D& extent,
	const vk::UniqueRenderPass& renderPass) {
	
	vk::FramebufferCreateInfo createInfo(vk::FramebufferCreateFlags(), renderPass.get(), 1, &imageView.get(),
		extent.width, extent.height, 1);
	framebuffer = Core::device.logicalDevice->createFramebufferUnique(createInfo);
}

/**
 * Frees resources if not already freed
 */
void FrameBuffer::freeResources(bool remove) {
	ChildResource::freeResources(remove);
	ParentResource::freeChildren();
	framebuffer.reset();
	imageView.reset();
	image.reset();
}
