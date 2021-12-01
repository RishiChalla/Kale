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

#pragma once

#include <Kale/Vulkan/ParentResource/ParentResource.hpp>
#include <Kale/Vulkan/ChildResource/ChildResource.hpp>
#include <Kale/Math/Vector/Vector.hpp>

#include <vulkan/vulkan.hpp>

namespace Kale::Vulkan {

	/**
	 * Forward Declaration of renderer class
	 */
	class Renderer;

	/**
	 * Forward Declaration of swapchain class
	 */
	class SwapChain;
	
	/**
	 * Represents a single framebuffer
	 */
	class FrameBuffer : public ParentResource<FrameBuffer>, public ChildResource<Renderer> {
	private:

		/**
		 * The image (ONLY if the object was initialized externally, not from a swapchain)
		 */
		vk::UniqueImage image;

		/**
		 * The image view (ONLY if the object was initialized externally, not from a swapchain)
		 */
		vk::UniqueImageView imageView;

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
		 * Creates an initialized FrameBuffer
		 * @param renderer The renderer this frame buffer will be rendered from
		 * @param swapchain The swapchain this framebuffer is to be created from
		 * @param imageViewIndex the image view index from the swapchain
		 */
		FrameBuffer(Renderer& renderer, const SwapChain& swapchain, size_t imageViewIndex);

		/**
		 * Initializes the object
		 * @param renderer The renderer this frame buffer will be rendered from
		 * @param swapchain The swapchain this framebuffer is to be created from
		 * @param imageViewIndex the image view index from the swapchain
		 */
		void init(Renderer& renderer, const SwapChain& swapchain, size_t imageViewIndex);

		friend class SwapChain;

	public:

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
		 */
		void freeResources(bool remove = true) override;
	};
}
