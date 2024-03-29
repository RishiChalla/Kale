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

#include <Kale/Vulkan/QueueFamilyIndices/QueueFamilyIndices.hpp>
#include <Kale/Vulkan/ParentResource/ParentResource.hpp>
#include <Kale/Vulkan/DeviceMemory/DeviceMemory.hpp>

#include <vulkan/vulkan.hpp>
#include <map>
#include <list>

namespace Kale::Vulkan {

	/**
	 * All the types of queues used for the engine
	 */
	enum class QueueType {
		Graphics, Presentation
	};
	
	/**
	 * A simple Kale abstraction over the Vulkan Physical Device and Logical Devices
	 */
	class Device : public ParentResource<Device> {
	private:

		/**
		 * Creates the logical device needed
		 */
		void createLogicalDevice();

		/**
		 * Gets the appropriate queues from the logical device
		 */
		void getQueues();

	public:

		/**
		 * The logical device created by the physical device
		 */
		vk::UniqueDevice logicalDevice;

		/**
		 * The device properties
		 */
		vk::PhysicalDeviceProperties physicalDeviceProperties;

		/**
		 * The physical device
		 */
		vk::PhysicalDevice physicalDevice;

		/**
		 * A map of all queue types to their appropriate queues
		 */
		std::map<QueueType, vk::Queue> queueMap;

		/**
		 * All the available queue family indices
		 */
		QueueFamilyIndices queueIndices;

		/**
		 * The memory properties of the device
		 */
		vk::PhysicalDeviceMemoryProperties memoryProperties;

		/**
		 * Creates an uninstantiated device
		 */
		Device();
		
		/**
		 * Creates a logical device and all appropriate queues given the physical device
		 * @param device The physical device
		 * @throws If the given device is not supported
		 */
		Device(const vk::PhysicalDevice& device);

		/**
		 * Creates a logical device and all appropriate queues given the physical device
		 * @param deviceId The id of the physical device
		 * @throws If the device was not found
		 */
		Device(uint32_t deviceId);

		/**
		 * Initializes this object
		 * @param device The device to initialize with
		 */
		void init(const vk::PhysicalDevice& device);

		/**
		 * Initializes this object
		 * @param deviceId The device to initialize with
		 */
		void init(uint32_t deviceId);

		/**
		 * Frees the resources from this device
		 */
		void freeResources();

		/**
		 * Finds a memory type given the type filter and properties
		 * @param typeFilter the filters to apply on the type of memory
		 * @param properties The properties required for the memory
		 * @throws If the device does not have an applicable memory type
		 */
		uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) const;

		/**
		 * Gets all available and supported physical devices
		 * @returns The available physical devices
		 */
		static std::vector<vk::PhysicalDevice> availableDevices();

		/**
		 * Checks whether or not a given physical device is supported
		 * @param physicalDevice The physical device to check support for
		 * @returns Whether or not the device is supported
		 */
		static bool deviceSupported(const vk::PhysicalDevice& physicalDevice);
	};
}

#endif
