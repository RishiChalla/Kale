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
#include <map>
#include <vulkan/vulkan.hpp>

#include <Kale/Application/Application.hpp>

namespace Kale::Vulkan {

	/**
	 * All the types of queues used for the engine
	 */
	enum class QueueType {
		Graphics
	};

	/**
	 * This class handles nearly all vulkan related details including setup, instances, validation layers, etc.
	 */
	class MainHandler {
	
	private:

		/**
		 * The vulkan instance used for this window
		 */
		vk::Instance instance;

		/**
		 * The physical GPU used for rendering with vulkan for this application
		 */
		vk::PhysicalDevice physicalDevice;

		/**
		 * The Device used for using render commands to the chosen physical device
		 */
		vk::Device logicalDevice;

		/**
		 * A map of all the queues this program is using from their type to the command queue
		 */
		std::map<QueueType, vk::Queue> queues;

		#ifdef KALE_DEBUG

		/**
		 * All the validation layers used when debugging
		 */
		static const std::vector<const char*> validationLayers;

		/**
		 * The debug messenger used for vulkan output messages
		 */
		VkDebugUtilsMessengerEXT debugMessenger;

		/**
		 * Sets up the debug message callback
		 */
		void setupDebugMessageCallback();

		/**
		 * Checks validation layer support for all given validation layers
		 * @returns Whether or not all validation layers given are supported
		 */
		bool checkValidationLayerSupport() const;

		/**
		 * Destroys the debug message callback
		 */
		void destroyDebugMessageCallback();

		#endif

		/**
		 * Creates the vulkan instance for this window
		 * @param windowRequiredExtensions The required extensions form the lower level windowing API
		 */
		void createVulkanInstance(const std::vector<const char*>& windowRequiredExtensions);

		/**
		 * Chooses the GPU from the available GPUs that support vulkan based on the user settings
		 */
		void chooseGPU();

		/**
		 * Creates the vulkan logical device object
		 */
		void createVulkanLogicalDevice();
	
	protected:

		/**
		 * Sets up the main handler, any functions called prior to this will result in undefined behavior
		 * @param windowRequiredExtensions The required extensions form the lower level windowing API
		 */
		void setupHandler(const std::vector<const char*>& windowRequiredExtensions);

		/**
		 * Cleans vulkan objects before the application closes
		 */
		void cleanupHandler();

		friend class Kale::Application;
	
	public:

		/**
		 * Gets the vulkan instance for this program execution
		 * @returns The vulkan instance
		 */
		const vk::Instance& getInstance() const;

		/**
		 * Gets the physical device used for this program, Kale only supports a single
		 * physical device at a time. The user may configure this device however
		 * @returns The physical device
		 */
		const vk::PhysicalDevice& getPhysicalDevice() const;

		/**
		 * Gets the logical device used for commands for this program
		 * @returns The logical device
		 */
		const vk::Device& getLogicalDevice() const;

		/**
		 * Gets a Command Queue for passing commands through
		 * @param type The type of queue to get
		 * @returns The queue to pass commands to, should be thread safe
		 */
		vk::Queue& operator[](QueueType type);

		/**
		 * Gets a Command Queue for passing commands through
		 * @param type The type of queue to get
		 * @returns The queue to pass commands to, should be thread safe
		 */
		const vk::Queue& operator[](QueueType type) const;
	};

	/**
	 * The main vulkan handler for this program's execution
	 */
	extern MainHandler mainHandler;
}