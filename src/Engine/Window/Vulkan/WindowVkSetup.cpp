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

#include "../Window.hpp"
#include <Engine/Application/Application.hpp>
#include <Engine/Settings/Settings.hpp>
#include <vector>
#include <algorithm>

using namespace Islands;

/**
 * Initializes vulkan for use with both windowing APIs
 */
void Window::initVulkan() {
	createVulkanInstance();

	#ifdef ISLANDS_DEBUG
	setupDebugMessageCallback();
	#endif

	chooseGPU();
}

/**
 * Creates the vulkan instance for this window
 */
void Window::createVulkanInstance() {
	vk::ApplicationInfo appInfo;
	appInfo.pApplicationName = getTitle();
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;

	vk::InstanceCreateInfo createInfo;
	createInfo.pApplicationInfo = &appInfo;
	std::vector<const char*> requiredExtensions = getCreateInfoExtensions();

	// Validation Layers
	#ifdef ISLANDS_DEBUG

	if (!checkValidationLayerSupport()) {
		error("Validation Layers not Available");
	}

	createInfo.enabledLayerCount = static_cast<uint32_t>(vulkanValidationLayers.size());
	createInfo.ppEnabledLayerNames = vulkanValidationLayers.data();
	requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	#else

	createInfo.enabledLayerCount = 0;

	#endif

	createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
	createInfo.ppEnabledExtensionNames = requiredExtensions.data();

	if (vk::createInstance(&createInfo, nullptr, &vulkanInstance) != vk::Result::eSuccess) {
		error("Unable to Init Vulkan");
		exit(0);
	}
}

/**
 * Chooses the GPU from the available GPUs that support vulkan based on the user settings
 */
void Window::chooseGPU() {
	std::vector<vk::PhysicalDevice> devices{vulkanInstance.enumeratePhysicalDevices()};

	// Remove devices which don't support swap chains/aren't compatible for our purposes
	info("Available GPUs - ");
	devices.erase(std::remove_if(devices.begin(), devices.end(), [](const vk::PhysicalDevice& device) {
		vk::PhysicalDeviceProperties properties = device.getProperties();
		
		// TODO - Check for things like swap chaining and return true if support not found
		// std::vector<vk::QueueFamilyProperties>{device.getQueueFamilyProperties()};

		info(properties.deviceName);
		return false;
	}), devices.end());

	if (devices.empty()) {
		error("No Vulkan Compatible GPU Found");
		exit(0);
	}

	// Select the GPU from the settings or set a default if there is none in the settings set yet
	uint32_t gpuID = settings.getGPUID();

	auto setDefaultGPU = [&]() {
		vk::PhysicalDeviceProperties properties = devices[0].getProperties();
		gpuID = properties.deviceID;
		settings.setGPUID(gpuID);
		vulkanPhysicalDevice = devices[0];
		info(std::string("Chose ") + properties.deviceName.data() + " as rendering GPU");
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
			vulkanPhysicalDevice = device;
			info(std::string("Settings GPU Found - ") + properties.deviceName.data());
			break;
		}

		if (!found) {
			warn("GPU in Settings not found - resetting default");
			setDefaultGPU();
		}
	}
}