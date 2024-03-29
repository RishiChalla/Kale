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

#include <Kale/Vulkan/SwapChainSupportDetails/SwapChainSupportDetails.hpp>
#include <Kale/Vulkan/ChildResource/ChildResource.hpp>
#include <Kale/Vulkan/ParentResource/ParentResource.hpp>
#include <Kale/Vulkan/Device/Device.hpp>

#include <vulkan/vulkan.hpp>
#include <vector>

namespace Kale::Vulkan {

	/**
	 * Forward declaration of renderer
	 */
	class Renderer;

	/**
	 * Simple Kale Abstraction over vulkan swap chains
	 */
	class SwapChain : public ParentResource<SwapChain>, public ChildResource<Device> {
	private:

		/**
		 * Creates the swapchain
		 */
		void createSwapChain();

		/**
		 * Creates image views from the images and swapchain
		 */
		void createImageViews();

	public:

		/**
		 * The swapchain object
		 */
		vk::UniqueSwapchainKHR swapchain;

		/**
		 * The extent of the swapchain
		 */
		vk::Extent2D extent;

		/**
		 * The format of the swapchain
		 */
		vk::Format format;

		/**
		 * The swapchain support information
		 */
		SwapChainSupportDetails support;

		/**
		 * All available image views in this swap chain
		 */
		std::vector<vk::UniqueImageView> imageViews;

		/**
		 * The images stored within the swapchain
		 */
		std::vector<vk::Image> images;

		/**
		 * The frame buffers used for rendering
		 */
		std::vector<vk::UniqueFramebuffer> frameBuffers;

		/**
		 * Creates a new swap chain given the device to create it from
		 * @param device The device to create the swap chain from
		 */
		SwapChain(Device& device);

		/**
		 * Creates an uninstantiated SwapChain
		 */
		SwapChain();

		/**
		 * Initializes the object
		 * @param device 
		 */
		void init(Device& device) override;

		/**
		 * Creates the frame buffers from the swap chain images/image views given the renderer
		 * @param renderer The renderer to create from
		 */
		void createFrameBuffers(Renderer& renderer);

		/**
		 * Frees resources if not already freed
		 */
		void freeResources(bool remove = true) override;
	};
}

#endif
