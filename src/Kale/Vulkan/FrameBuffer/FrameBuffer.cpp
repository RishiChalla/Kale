/*
   Copyright 2022 Rishi Challa

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

#ifdef KALE_VULKAN

#include "FrameBuffer.hpp"

#include <Kale/Vulkan/Renderer/Renderer.hpp>
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
	const vk::ImageCreateInfo imageCreateInfo(vk::ImageCreateFlags(), vk::ImageType::e2D, Core::swapchain.format,
		{size.x, size.y, 1}, 1, 1, vk::SampleCountFlagBits::e1, vk::ImageTiling::eOptimal,
		vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled);
	image = Core::device.logicalDevice.get().createImageUnique(imageCreateInfo);

	// Allocate memory for the image and bind it
	imageMemory.init(Core::device, Core::device.logicalDevice.get().getImageMemoryRequirements(image.get()),
		vk::MemoryPropertyFlagBits::eDeviceLocal);
	Core::device.logicalDevice.get().bindImageMemory(image.get(), imageMemory.deviceMemory.get(), imageMemory.memoryInfo.alignment);
	
	// Create the image view
	const vk::ImageSubresourceRange range(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
	const vk::ImageViewCreateInfo imageViewCreateInfo(vk::ImageViewCreateFlags(), image.get(), vk::ImageViewType::e2D,
		Core::swapchain.format, vk::ComponentMapping(), range);
	imageView = Core::device.logicalDevice.get().createImageViewUnique(imageViewCreateInfo);

	// Sets up the frame buffer
	createFrameBuffer(imageView, {size.x, size.y}, renderer.renderPass);
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

	// Sets up the frame buffer
	createFrameBuffer(imageView, {size.x, size.y}, renderer.renderPass);
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
}

/**
 * Frees resources if not already freed
 * Frees BOTH the imageview/image, and framebuffer
 */
void FrameBuffer::freeResources() {
	ChildResource::freeResources();
	ParentResource::freeChildren();
	framebuffer.reset();
	imageView.reset();
	image.reset();
	imageMemory.freeResources();
}

#endif
