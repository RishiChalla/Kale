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

#include <Kale/Vulkan/Device/Device.hpp>
#include <Kale/Vulkan/SwapChain/SwapChain.hpp>
#include <Kale/Core/Application/Application.hpp>

#include <vulkan/vulkan.hpp>

namespace Kale::Vulkan {

	/**
	 * This class handles nearly all vulkan related details including setup, instances, validation layers, etc.
	 */
	class Core {
	private:
		#ifdef KALE_DEBUG

		/**
		 * The debug messenger used for vulkan output messages
		 */
		static inline VkDebugUtilsMessengerEXT debugMessenger;

		/**
		 * Sets up the debug message callback
		 */
		static void setupDebugMessageCallback();

		/**
		 * Destroys the debug message callback
		 */
		static void destroyDebugMessageCallback();

		#endif

		/**
		 * Creates the vulkan instance for this window
		 */
		static void createInstance();

		/**
		 * Sets up the command pool
		 */
		static void createCommandPool();

		/**
		 * Chooses the physical device and sets up the physical device and logical device with the appropriate extensions
		 * @param gpuID the ID of the GPU to use for rendering
		 */
		static void chooseDevice(std::optional<uint32_t> gpuID);

	protected:

		/**
		 * Sets up the main core, any functions called prior to this will result in undefined behavior
		 * @param gpuID the ID of the GPU to use for rendering
		 */
		static void setupCore(std::optional<uint32_t> gpuID = std::optional<uint32_t>());

		/**
		 * Cleans vulkan objects before the application closes
		 */
		static void cleanupCore();

		friend class Kale::Application;
	
	public:

		/**
		 * The vulkan instance used for this window
		 */
		static inline vk::UniqueInstance instance;

		/**
		 * The vulkan surface used for rendering
		 */
		static inline vk::UniqueSurfaceKHR surface;

		/**
		 * The GPU used for rendering
		 */
		static inline Device device;

		/**
		 * The swapchain for rendering
		 */
		static inline SwapChain swapchain;

		/**
		 * The vulkan command pool from which command buffers are obtained
		 */
		static inline vk::UniqueCommandPool commandPool;
	};

}
