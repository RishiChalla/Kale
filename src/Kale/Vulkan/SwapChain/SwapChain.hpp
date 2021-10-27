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

#include <Kale/Vulkan/SwapChainSupportDetails/SwapChainSupportDetails.hpp>
#include <vulkan/vulkan.hpp>
#include <vector>

namespace Kale::Vulkan {

	/**
	 * Forward declaration of device class
	 */
	class Device;
	
	/**
	 * Simple Kale Abstraction over vulkan swap chains
	 */
	struct SwapChain {
	private:

		/**
		 * Creates the swapchain
		 */
		void createSwapChain();

	public:

		/**
		 * The swapchain object
		 */
		vk::SwapchainKHR swapchain;

		/**
		 * The swapchain support information
		 */
		SwapChainSupportDetails support;

		/**
		 * All available image views in this swap chain
		 */
		std::vector<vk::ImageView> imageViews;

		/**
		 * The images stored within the swapchain
		 */
		std::vector<vk::Image> images;

		/**
		 * A pointer to the device if available
		 */
		const Device* devicePtr;

		/**
		 * Creates a new swap chain given the device to create it from
		 * @param device The device to create the swap chain from
		 */
		SwapChain(const Device& device);

		/**
		 * Creates an uninstantiated SwapChain
		 */
		SwapChain();

		/**
		 * Copy Constructor
		 * @param other Object to copy from
		 */
		SwapChain(const SwapChain& other);
		
		/**
		 * Move Constructor
		 * @param other Object to move from
		 */
		SwapChain(SwapChain&& other);

		/**
		 * Copy Assignment
		 * @param other Object to copy from
		 */
		void operator=(const SwapChain& other);

		/**
		 * Move Assignment
		 * @param other Object to move from
		 */
		void operator=(SwapChain&& other);

		/**
		 * Frees resources if not already freed
		 */
		~SwapChain();

		/**
		 * Frees resources if not already freed
		 */
		void freeResources();
	};
}