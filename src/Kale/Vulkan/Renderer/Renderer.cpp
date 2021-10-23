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

#include "Renderer.hpp"

#include <Kale/Vulkan/QueueFamilyIndices/QueueFamilyIndices.hpp>
#include <stdexcept>
#include <algorithm>

using namespace Kale;
using namespace Kale::Vulkan;

Renderer Kale::Vulkan::renderer;

/**
 * Gets the vulkan instance for this program execution
 * @returns The vulkan instance
 */
const vk::Instance& Renderer::getInstance() const {
	return instance;
}

/**
 * Gets the physical device used for this program, Kale only supports a single
 * physical device at a time. The user may configure this device however
 * @returns The physical device
 */
const vk::PhysicalDevice& Renderer::getPhysicalDevice() const {
	return physicalDevice;
}

/**
 * Gets the logical device used for commands for this program
 * @returns The logical device
 */
const vk::Device& Renderer::getLogicalDevice() const {
	return logicalDevice;
}

/**
 * Gets a Command Queue for passing commands through
 * @param type The type of queue to get
 * @returns The queue to pass commands to, should be thread safe
 */
vk::Queue& Renderer::operator[](QueueType type) {
	return queues[type];
}

/**
 * Gets a Command Queue for passing commands through
 * @param type The type of queue to get
 * @returns The queue to pass commands to, should be thread safe
 */
const vk::Queue& Renderer::operator[](QueueType type) const {
	return queues.at(type);
}

/**
 * Gets all available GPUs to choose from with their IDs and Names
 * @returns A vector of the available GPUs with their ID and name
 */
std::vector<std::tuple<uint32_t, std::string>> Renderer::getAvailableGPUs() const {
	std::vector<std::tuple<uint32_t, std::string>> availableGPUs;

	// Loop through all available devices
	for (const vk::PhysicalDevice& device : instance.enumeratePhysicalDevices()) {

		// Ensure that the physical device is a GPU/has all required queue family indices
		Vulkan::QueueFamilyIndices queueFamilyIndices(0, device.getQueueFamilyProperties());
		if (!queueFamilyIndices.hasAllIndices()) continue;

		// Get the GPU properties and queue family properties
		vk::PhysicalDeviceProperties properties = device.getProperties();

		// Add it to the list of available GPUs
		availableGPUs.push_back(std::make_tuple(properties.deviceID, std::string(properties.deviceName)));
	}

	return availableGPUs;
}

/**
 * Gets the GPU information of the physical device currently being used for rendering
 * @returns A tuple of the GPU id and the name
 */
std::tuple<uint32_t, std::string> Renderer::getCurrentGPU() const {
	// Get the GPU properties
	vk::PhysicalDeviceProperties properties = physicalDevice.getProperties();
	return std::make_tuple(properties.deviceID, std::string(properties.deviceName));
}

/**
 * Uses a specific GPU given the ID
 * @param gpuID The id of the GPU to use
 * @throws If the GPU wasn't found
 */
void Renderer::useGPU(uint32_t gpuID) {

	// Loop over all the devices
	for (vk::PhysicalDevice& device : instance.enumeratePhysicalDevices()) {
		vk::PhysicalDeviceProperties properties = device.getProperties();
		if (properties.deviceID != gpuID) continue;
		console.info("Now using GPU - " + std::string(properties.deviceName));
		physicalDevice = device;
		createLogicalDevice();
		return;
	}

	throw std::runtime_error("GPU search by ID not found");
}