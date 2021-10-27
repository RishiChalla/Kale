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

#include <vector>
#include <optional>
#include <vulkan/vulkan.hpp>

#include <Kale/Vulkan/Device/Device.hpp>
#include <Kale/Application/Application.hpp>

namespace Kale::Vulkan {

	/**
	 * This class handles nearly all vulkan related details including setup, instances, validation layers, etc.
	 */
	class Renderer {
	
	private:

		#ifdef KALE_DEBUG

		/**
		 * The debug messenger used for vulkan output messages
		 */
		VkDebugUtilsMessengerEXT debugMessenger;

		/**
		 * Sets up the debug message callback
		 */
		void setupDebugMessageCallback();

		/**
		 * Destroys the debug message callback
		 */
		void destroyDebugMessageCallback();

		#endif

		/**
		 * Creates the vulkan instance for this window
		 */
		void createInstance();
	
	protected:

		/**
		 * The vulkan instance used for this window
		 */
		vk::Instance instance;

		/**
		 * The vulkan surface used for rendering
		 */
		vk::SurfaceKHR surface;

		/**
		 * The GPU used for rendering
		 */
		Device device;

		/**
		 * Sets up the main renderer, any functions called prior to this will result in undefined behavior
		 * @param gpuID the ID of the GPU to use for rendering
		 */
		void setupRenderer(std::optional<uint32_t> gpuID = std::optional<uint32_t>());

		/**
		 * Cleans vulkan objects before the application closes
		 */
		void cleanupRenderer();

		friend class Kale::Application;
		friend class Device;
		friend class QueueFamilyIndices;
		friend class SwapChainSupportDetails;
	
	public:

		/**
		 * Gets the GPU information of the physical device currently being used for rendering
		 * @returns The GPU being used for rendering
		 */
		const Device& getGPU() const;

		/**
		 * Uses a specific GPU given the ID
		 * @param gpuID The id of the GPU to use
		 * @throws If the GPU wasn't found
		 */
		void useGPU(uint32_t gpuID);

		/**
		 * Uses a specific GPU given the physical device
		 * @param device The physical device
		 */
		void useGPU(const vk::PhysicalDevice& device);
	};

	/**
	 * The main vulkan renderer for this program's execution
	 */
	extern Renderer renderer;
}