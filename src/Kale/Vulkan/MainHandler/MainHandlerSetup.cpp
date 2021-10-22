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

#include "MainHandler.hpp"
#include <Kale/Vulkan/QueueFamilyIndices/QueueFamilyIndices.cpp>
#include <Kale/Settings/Settings.hpp>

using namespace Kale;
using namespace Kale::Vulkan;

/**
 * Sets up the main handler, any functions called prior to this will result in undefined behavior
 * @param windowRequiredExtensions The required extensions form the lower level windowing API
 */
void MainHandler::setupHandler(const std::vector<const char*>& windowRequiredExtensions) {

	try {
		createVulkanInstance(windowRequiredExtensions);

		#ifdef KALE_DEBUG
		setupDebugMessageCallback();
		#endif

		chooseGPU();
		createVulkanLogicalDevice();
	}
	catch (const std::exception& e) {
		console.error(e.what());
		exit(0);
	}
}

/**
 * Cleans vulkan objects before the application closes
 */
void MainHandler::cleanupHandler() {
	try {
		#ifdef KALE_DEBUG
		destroyDebugMessageCallback();
		#endif
		
		logicalDevice.destroy();
		instance.destroy();
	}
	catch (const std::exception& e) {
		console.error(e.what());
		exit(0);
	}
}

/**
 * Creates the vulkan instance for this window
 * @param windowRequiredExtensions The required extensions form the lower level windowing API
 */
void MainHandler::createVulkanInstance(const std::vector<const char*>& windowRequiredExtensions) {
	vk::ApplicationInfo appInfo;
	appInfo.pApplicationName = mainApp->applicationName.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;

	vk::InstanceCreateInfo createInfo;
	createInfo.pApplicationInfo = &appInfo;
	std::vector<const char*> requiredExtensions = windowRequiredExtensions;
	requiredExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

	// Validation Layers
	#ifdef KALE_DEBUG

	if (!checkValidationLayerSupport()) {
		console.error("Validation Layers not Available");
	}

	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	createInfo.ppEnabledLayerNames = validationLayers.data();
	requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	#else

	createInfo.enabledLayerCount = 0;

	#endif

	createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
	createInfo.ppEnabledExtensionNames = requiredExtensions.data();

	if (vk::createInstance(&createInfo, nullptr, &instance) != vk::Result::eSuccess) {
		console.error("Unable to Init Vulkan");
		exit(0);
	}
}

/**
 * Chooses the GPU from the available GPUs that support vulkan based on the user settings
 */
void MainHandler::chooseGPU() {
	std::vector<vk::PhysicalDevice> devices{instance.enumeratePhysicalDevices()};

	// Remove devices which don't support swap chains/aren't compatible for our purposes
	console.info("Available GPUs - ");
	devices.erase(std::remove_if(devices.begin(), devices.end(), [](const vk::PhysicalDevice& device) {

		// Get the GPU properties and queue family properties
		vk::PhysicalDeviceProperties properties = device.getProperties();

		// Ensure that the physical device is a GPU/has all required queue family indices
		Vulkan::QueueFamilyIndices queueFamilyIndices(properties.deviceID, device.getQueueFamilyProperties());
		if (!queueFamilyIndices.hasAllIndices()) return true;

		// Print the available GPUs for debugging and don't remove it since it passed all checks
		console.info(properties.deviceName);
		return false;
	}), devices.end());

	if (devices.empty()) {
		console.error("No Vulkan Compatible GPU Found");
		exit(0);
	}

	// Select the GPU from the settings or set a default if there is none in the settings set yet
	uint32_t gpuID = settings.getGPUID();

	auto setDefaultGPU = [&]() {
		vk::PhysicalDeviceProperties properties = devices[0].getProperties();
		gpuID = properties.deviceID;
		settings.setGPUID(gpuID);
		physicalDevice = devices[0];
		console.info(std::string("Chose ") + properties.deviceName.data() + " as rendering GPU");
	};

	if (gpuID == 0) {
		setDefaultGPU();
	}
	else {
		bool found = false;
		for (vk::PhysicalDevice& device : devices) {
			vk::PhysicalDeviceProperties properties = device.getProperties();
			if (properties.deviceID != gpuID) continue;
			found = true;
			physicalDevice = device;
			console.info(std::string("Settings GPU Found - ") + properties.deviceName.data());
			break;
		}

		if (!found) {
			console.warn("GPU in Settings not found - resetting default");
			setDefaultGPU();
		}
	}
}

/**
 * Creates the vulkan logical device object
 */
void MainHandler::createVulkanLogicalDevice() {

	// Get all the required indices
	Vulkan::QueueFamilyIndices indices(physicalDevice.getProperties().deviceID, physicalDevice.getQueueFamilyProperties());
	
	// Create the queue create info
	std::vector<const float> priorities = {1.0f};
	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfo = {
		vk::DeviceQueueCreateInfo({}, indices.graphicsFamilyIndex.value(), priorities)
	};

	// Choose all required device features we desire
	vk::PhysicalDeviceFeatures deviceFeatures;
	// TODO - set the required device features to true

	// Set all required device extensions
	std::vector<const char*> requiredDeviceExtensions;

	// Loop through all the supported device extension properties
	for (const vk::ExtensionProperties& property : physicalDevice.enumerateDeviceExtensionProperties()) {
		// requiredDeviceExtensions.push_back(property.extensionName);
	}

	// Create the logical device create info
	vk::DeviceCreateInfo createInfo({}, queueCreateInfo, {}, requiredDeviceExtensions, &deviceFeatures);

	// Create the logical device
	logicalDevice = physicalDevice.createDevice(createInfo);
	queues[QueueType::Graphics] = logicalDevice.getQueue(indices.graphicsFamilyIndex.value(), 0);
}