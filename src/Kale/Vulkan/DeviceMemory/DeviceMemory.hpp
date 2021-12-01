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

#include <Kale/Vulkan/ChildResource/ChildResource.hpp>
#include <Kale/Vulkan/ParentResource/ParentResource.hpp>

#include <vulkan/vulkan.hpp>

namespace Kale::Vulkan {

	/**
	 * Forward declaration of device class
	 */
	class Device;

	/**
	 * Handles physical device memory allocation and deletion automatically using RAII to avoid memory leaks
	 * TODO - Delete this class and create a device memory pool class
	 * See https://www.youtube.com/watch?v=rXSdDE7NWmA for more info - getting memory is expensive so a pool can help FPS a ton
	 */
	class DeviceMemory : public ParentResource<DeviceMemory>, public ChildResource<Device> {
	private:
		
		/**
		 * Allocates memory into the device memory
		 */
		void allocateMemory();

	public:

		/**
		 * Creates an unitialized object
		 */
		DeviceMemory();

		/**
		 * Creates and initializes the device memory object
		 * @param device The device to allocate the memory on
		 * @param requirements The requirements of the memory to allocate
		 * @param properties Properties regarding the memory to allocate
		 */
		DeviceMemory(Device& device, const vk::MemoryRequirements& requirements, vk::MemoryPropertyFlags properties);

		/**
		 * Initializes the device memory object
		 * @param device The device to allocate the memory on
		 * @param requirements The requirements of the memory to allocate
		 * @param properties Properties regarding the memory to allocate
		 */
		void init(Device& device, const vk::MemoryRequirements& requirements, vk::MemoryPropertyFlags properties);

		/**
		 * Creates and initializes the device memory object
		 * @param device The device to allocate the memory on
		 * @param requirements The requirements of the memory to allocate
		 * @param properties Properties regarding the memory to allocate
		 */
		DeviceMemory(Device& device, vk::MemoryRequirements&& requirements, vk::MemoryPropertyFlags properties);

		/**
		 * Initializes the device memory object
		 * @param device The device to allocate the memory on
		 * @param requirements The requirements of the memory to allocate
		 * @param properties Properties regarding the memory to allocate
		 */
		void init(Device& device, vk::MemoryRequirements&& requirements, vk::MemoryPropertyFlags properties);

		/**
		 * The device memory block
		 */
		vk::UniqueDeviceMemory deviceMemory;

		/**
		 * The size of the memory block
		 */
		vk::MemoryRequirements memoryInfo;

		/**
		 * The properties of the memory, ie vram or ram, etc
		 */
		vk::MemoryPropertyFlags memoryProperties;

		/**
		 * Frees the memory on the device
		 */
		void freeResources(bool remove = true) override;

	};
}
