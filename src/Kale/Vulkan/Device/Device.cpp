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

#include "Device.hpp"

#include <Kale/Vulkan/Core/Core.hpp>
#include <Kale/Vulkan/Extensions/Extensions.hpp>
#include <Kale/Vulkan/SwapChainSupportDetails/SwapChainSupportDetails.hpp>

#include <algorithm>
#include <stdexcept>
#include <exception>

using namespace Kale;
using namespace Kale::Vulkan;

/**
 * Creates an uninstantiated device
 */
Device::Device() {
	// Empty Body
}

/**
 * Creates a logical device and all appropriate queues given the physical device
 * @param device The physical device
 * @throws If the given device is not supported
 */
Device::Device(const vk::PhysicalDevice& device) : physicalDevice(device), physicalDeviceProperties(device.getProperties()),
	queueIndices(device) {
	
	if (!deviceSupported(device)) throw std::runtime_error("Unsupported Device Used");

	memoryProperties = physicalDevice.getMemoryProperties();
	createLogicalDevice();
	getQueues();
}

/**
 * Creates a logical device and all appropriate queues given the physical device
 * @param deviceId The id of the physical device
 * @throws If the device was not found
 */
Device::Device(uint32_t deviceId) {
	bool found = false;
	for (const vk::PhysicalDevice& device : Device::availableDevices()) {
		vk::PhysicalDeviceProperties properties = device.getProperties();
		if (properties.deviceID != deviceId) continue;
		physicalDevice = device;
		physicalDeviceProperties = properties;
		queueIndices = QueueFamilyIndices(device);
		found = true;
		break;
	}

	if (!found) throw std::runtime_error("Device not Found");

	memoryProperties = physicalDevice.getMemoryProperties();
	createLogicalDevice();
	getQueues();
}


/**
 * Initializes this object
 * @param device The device to initialize with
 */
void Device::init(const vk::PhysicalDevice& device) {
	freeResources();
	physicalDevice = device;
	physicalDeviceProperties = device.getProperties();
	queueIndices = QueueFamilyIndices(device);

	if (!deviceSupported(device)) throw std::runtime_error("Unsupported Device Used");

	memoryProperties = physicalDevice.getMemoryProperties();
	createLogicalDevice();
	getQueues();
}

/**
 * Initializes this object
 * @param deviceId The device to initialize with
 */
void Device::init(uint32_t deviceId) {
	freeResources();
	bool found = false;
	for (const vk::PhysicalDevice& device : Device::availableDevices()) {
		vk::PhysicalDeviceProperties properties = device.getProperties();
		if (properties.deviceID != deviceId) continue;
		physicalDevice = device;
		physicalDeviceProperties = properties;
		queueIndices = QueueFamilyIndices(device);
		found = true;
		break;
	}

	if (!found) throw std::runtime_error("Device not Found");

	memoryProperties = physicalDevice.getMemoryProperties();
	createLogicalDevice();
	getQueues();
}

/**
 * Creates the logical device needed
 */
void Device::createLogicalDevice() {
	
	// Create the queue create info
	std::vector<float> priorities = {1.0f};
	std::unordered_set<uint32_t> uniqueIndices = queueIndices.getUniqueIndices();
	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfo;

	// Populate the queue create info vector 
	for (uint32_t i : uniqueIndices) {
		queueCreateInfo.emplace_back(vk::DeviceQueueCreateFlags(), i, 1, priorities.data());
	}

	// Choose all required device features we desire
	vk::PhysicalDeviceFeatures features;
	// TODO - set the required device features to true

	// Set all required device extensions
	std::vector<const char*> extensions = getExtensions<vk::ExtensionProperties>(
		physicalDevice.enumerateDeviceExtensionProperties(), requiredDeviceExtensions,
		requestedDeviceExtensions, [](const vk::ExtensionProperties& p) {
		
		// Map extension property to a const char*
		return std::string(p.extensionName);
	});

	// Create the logical device create info
	vk::DeviceCreateInfo createInfo(vk::DeviceCreateFlags(), static_cast<uint32_t>(queueCreateInfo.size()), queueCreateInfo.data(),
		0, nullptr, static_cast<uint32_t>(extensions.size()), extensions.data(), &features);

	// Create the logical device
	logicalDevice = physicalDevice.createDeviceUnique(createInfo);
}

/**
 * Gets the appropriate queues from the logical device
 */
void Device::getQueues() {
	queueMap[QueueType::Graphics] = logicalDevice->getQueue(queueIndices.graphicsFamilyIndex.value(), 0);
	queueMap[QueueType::Presentation] = logicalDevice->getQueue(queueIndices.presentFamilyIndex.value(), 0);
}

/**
 * Frees the resources from this device
 */
void Device::freeResources() {
	ParentResource::freeChildren();
	logicalDevice.reset();
}

/**
 * Finds a memory type given the type filter and properties
 * @param typeFilter the filters to apply on the type of memory
 * @param properties The properties required for the memory
 * @throws If the device does not have an applicable memory type
 */
uint32_t Device::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) const {
	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
		// Check that the filter is correct and that all properties are within the property flags
		if (typeFilter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
			return i;

	throw std::runtime_error("Failed to find applicable memory type");
}

/**
 * Gets all available and supported physical devices
 * @returns The available physical devices
 */
std::vector<vk::PhysicalDevice> Device::availableDevices() {
	std::vector<vk::PhysicalDevice> devices = Core::instance->enumeratePhysicalDevices();
	devices.erase(std::remove_if(devices.begin(), devices.end(), [](const vk::PhysicalDevice& device) {
		return !Device::deviceSupported(device);
	}), devices.end());
	return devices;
}

/**
 * Checks whether or not a given physical device is supported
 * @param physicalDevice The physical device to check support for
 * @returns Whether or not the device is supported
 */
bool Device::deviceSupported(const vk::PhysicalDevice& physicalDevice) {
	// Ensure that the physical device is a GPU/has all required queue family indices
	QueueFamilyIndices queueFamilyIndices(physicalDevice);
	if (!queueFamilyIndices.hasAllIndices()) return false;

	// Ensure the GPU has all required extensions
	try {
		getExtensions<vk::ExtensionProperties>(
			physicalDevice.enumerateDeviceExtensionProperties(), requiredDeviceExtensions,
			requestedDeviceExtensions, [](const vk::ExtensionProperties& p) {
			
			// Map extension property to a const char*
			return std::string(p.extensionName);
		});
	}
	catch (const std::exception&) {
		// This device is lacking a required extension
		return false;
	}

	// Ensure the GPU has swap chain support
	SwapChainSupportDetails swapChainSupport(physicalDevice);
	if (!swapChainSupport.deviceIsAdequate()) return false;

	// All checks passed
	return true;
}
