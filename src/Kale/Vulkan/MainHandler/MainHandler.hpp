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
#include <optional>
#include <tuple>
#include <string>
#include <functional>
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
		 * @param windowRequiredExtensions The required extensions form the lower level windowing API
		 */
		void createInstance(const std::vector<const char*>& windowRequiredExtensions);

		/**
		 * Creates the vulkan logical device object
		 */
		void createLogicalDevice();

		/**
		 * Gets the list of vulkan extensions given the available extensions, required extensions, and requested extensions
		 * @throws Throws when a required extension is not found
		 * @param availableExtensions The available extensions directly from a vulkan enumeration
		 * @param requiredExtensions The required extensions, if any of these are missing an exception will be thrown
		 * @param requestedExtensions The requested extensions, these will be added if available
		 * @param mappingFn A lambda to map from a vulkan type or any other type to strings
		 * @returns The list of extensions accounting for all of the above
		 */
		template <typename T> std::vector<const char*> getExtensions(const std::vector<T>& availableExtensions,
			const std::vector<std::string>& requiredExtensions, const std::vector<std::string>& requestedExtensions,
			std::function<std::string (const T&)> mappingFn) {

			// Filter the extensions out
			std::vector<const char*> extensions;

			// Find all required extensions
			for (const std::string& requiredExtension : requiredExtensions) {

				// If a required extension isn't found then throw an error
				if (std::find_if(availableExtensions.begin(), availableExtensions.end(), [&](const T& availableExtension) {
					return requiredExtension == mappingFn(availableExtension);
				}) == availableExtensions.end())
					throw std::runtime_error("Unable to find required extension - " + requiredExtension);
				
				// Add the extension
				extensions.push_back(requiredExtension.c_str());
			}

			// Find requested extensions
			for (const std::string& requestedExtension : requestedExtensions) {

				// Skip this extension if its not found
				if (std::find_if(availableExtensions.begin(), availableExtensions.end(), [&](const T& availableExtension) {
					return requestedExtension == mappingFn(availableExtension);
				}) == availableExtensions.end())
					continue;
				
				// If found add the extension
				extensions.push_back(requestedExtension.c_str());
			}

			// Return our processed extensions
			return extensions;
		}
	
	protected:

		/**
		 * Sets up the main handler, any functions called prior to this will result in undefined behavior
		 * @param windowRequiredExtensions The required extensions form the lower level windowing API
		 * @param gpuID the ID of the GPU to use for rendering
		 */
		void setupHandler(const std::vector<const char*>& windowRequiredExtensions,
			std::optional<uint32_t> gpuID = std::optional<uint32_t>());

		/**
		 * Cleans vulkan objects before the application closes
		 */
		void cleanupHandler();

		friend class Kale::Application;
	
	public:

		/**
		 * Gets all available GPUs to choose from with their IDs and Names
		 * @returns A vector of the available GPUs with their ID and name
		 */
		std::vector<std::tuple<uint32_t, std::string>> getAvailableGPUs() const;

		/**
		 * Gets the GPU information of the physical device currently being used for rendering
		 * @returns A tuple of the GPU id and the name
		 */
		std::tuple<uint32_t, std::string> getCurrentGPU() const;

		/**
		 * Uses a specific GPU given the ID
		 * @param gpuID The id of the GPU to use
		 * @throws If the GPU wasn't found
		 */
		void useGPU(uint32_t gpuID);

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