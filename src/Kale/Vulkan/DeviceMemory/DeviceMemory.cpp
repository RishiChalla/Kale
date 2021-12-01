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

#include "DeviceMemory.hpp"

#include <Kale/Vulkan/Device/Device.hpp>

using namespace Kale;
using namespace Kale::Vulkan;

/**
 * Creates an unitialized object
 */
DeviceMemory::DeviceMemory() {
	// Empty Body
}

/**
 * Creates and initializes the device memory object
 * @param device The device to allocate the memory on
 * @param requirements The requirements of the memory to allocate
 * @param properties Properties regarding the memory to allocate
 */
DeviceMemory::DeviceMemory(Device& device, const vk::MemoryRequirements& requirements, vk::MemoryPropertyFlags properties) :
	ChildResource(device), memoryInfo(requirements), memoryProperties(properties) {
	allocateMemory();
}

/**
 * Initializes the device memory object
 * @param device The device to allocate the memory on
 * @param requirements The requirements of the memory to allocate
 * @param properties Properties regarding the memory to allocate
 */
void DeviceMemory::init(Device& device, const vk::MemoryRequirements& requirements, vk::MemoryPropertyFlags properties) {
	ChildResource::init(device);
	memoryInfo = requirements;
	memoryProperties = properties;
	allocateMemory();
}

/**
 * Creates and initializes the device memory object
 * @param device The device to allocate the memory on
 * @param requirements The requirements of the memory to allocate
 * @param properties Properties regarding the memory to allocate
 */
DeviceMemory::DeviceMemory(Device& device, vk::MemoryRequirements&& requirements, vk::MemoryPropertyFlags properties) :
	ChildResource(device), memoryInfo(std::move(requirements)), memoryProperties(properties) {
	allocateMemory();
}

/**
 * Initializes the device memory object
 * @param device The device to allocate the memory on
 * @param requirements The requirements of the memory to allocate
 * @param properties Properties regarding the memory to allocate
 */
void DeviceMemory::init(Device& device, vk::MemoryRequirements&& requirements, vk::MemoryPropertyFlags properties) {
	ChildResource::init(device);
	memoryInfo = std::move(requirements);
	memoryProperties = properties;
	allocateMemory();
}

/**
 * Allocates memory into the device memory
 */
void DeviceMemory::allocateMemory() {
	uint32_t memoryType = parentPtr->findMemoryType(memoryInfo.memoryTypeBits, memoryProperties);
	vk::MemoryAllocateInfo allocInfo(memoryInfo.size, memoryType);
	deviceMemory = parentPtr->logicalDevice.get().allocateMemoryUnique(allocInfo);
}

/**
 * Frees the memory on the device
 */
void DeviceMemory::freeResources(bool remove) {
	ChildResource::freeResources(remove);
	ParentResource::freeChildren();
	deviceMemory.reset();
	memoryInfo = vk::MemoryRequirements();
}
