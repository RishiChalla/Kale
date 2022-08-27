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

#pragma once

#ifdef KALE_VULKAN

#include <Kale/Vulkan/ParentResource/ParentResource.hpp>
#include <Kale/Vulkan/ChildResource/ChildResource.hpp>
#include <Kale/Vulkan/DeviceMemory/DeviceMemory.hpp>
#include <Kale/Vulkan/Renderer/Renderer.hpp>
#include <Kale/Math/Vector/Vector.hpp>

#include <vulkan/vulkan.hpp>

namespace Kale::Vulkan {
	
	/**
	 * Represents a single framebuffer for offscreen rendering, NOT directly to the swapchain.
	 * To render to the swapchain use the swapchain's framebuffers instead
	 */
	class FrameBuffer : public ParentResource<FrameBuffer>, public ChildResource<Renderer> {
	private:

		/**
		 * Creates the frame buffer object
		 * @param imageView the image view to create the framebuffer from
		 * @param extent The extent of the image view to use
		 * @param renderPass The render pass which will be rendering to this frame buffer
		 */
		void createFrameBuffer(const vk::UniqueImageView& imageView, const vk::Extent2D& extent,
			const vk::UniqueRenderPass& renderPass);
	
	protected:

		/**
		 * Frees resources if not already freed
		 * ONLY frees the frame buffer itself, not the image view or image
		 */
		void freeResources(bool remove) override;

	public:

		/**
		 * The actual memory on the device storing the image
		 */
		DeviceMemory imageMemory;

		/**
		 * The image
		 */
		vk::UniqueImage image;

		/**
		 * The image view
		 */
		vk::UniqueImageView imageView;

		/**
		 * The frame buffer object
		 */
		vk::UniqueFramebuffer framebuffer;

		/**
		 * Creates an uninstantiated FrameBuffer
		 */
		FrameBuffer();

		/**
		 * Creates an initialized FrameBuffer
		 * @param renderer The renderer this frame buffer will be rendered from
		 */
		FrameBuffer(Renderer& renderer, const Vector2ui32& size);

		/**
		 * Initializes the object
		 * @param renderer The renderer this frame buffer will be rendered from
		 */
		void init(Renderer& renderer, const Vector2ui32& size);

		/**
		 * Frees resources if not already freed
		 * Frees BOTH the imageview/image, and framebuffer
		 */
		void freeResources();
	};
}

#endif
