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

#include "Core.hpp"

#include <Kale/Vulkan/QueueFamilyIndices/QueueFamilyIndices.hpp>
#include <Kale/Vulkan/Extensions/Extensions.hpp>
#include <Kale/Vulkan/SwapChainSupportDetails/SwapChainSupportDetails.hpp>

#include <stdexcept>
#include <exception>
#include <algorithm>
#include <limits>

using namespace Kale;
using namespace Kale::Vulkan;

/**
 * Sets up the core, any functions called prior to this will result in undefined behavior
 * @param gpuID the ID of the GPU to use for rendering
 */
void Core::setupCore(std::optional<uint32_t> gpuID) {

	try {
		createInstance();

		#ifdef KALE_DEBUG
		setupDebugMessageCallback();
		#endif
		
		// Create window surface for rendering
		mainApp->getWindow().createWindowSurface(instance, surface);

		// Choose the physical device and set it up
		chooseDevice(gpuID);

		swapchain.init(device);
		// pipeline.init("shader.vert.spv", "shader.frag.spv", device);
		// swapchain.createFrameBuffers(pipeline.renderPass);
		createCommandPool();
		// commandPool.init(device, Vector4f(0.5f, 0.0f, 1.0f, 1.0f));
	}
	catch (const std::exception& e) {
		console.error(e.what());
		exit(0);
	}
}


/**
 * Cleans vulkan objects before the application closes
 */
void Core::cleanupCore() {
	try {
		#ifdef KALE_DEBUG
		destroyDebugMessageCallback();
		#endif
		
		commandPool.reset();
		device.freeResources();
		surface.reset();
		instance.reset();
	}
	catch (const std::exception& e) {
		console.error(e.what());
		exit(0);
	}
}

/**
 * Creates the vulkan instance for this window
 */
void Core::createInstance() {
	vk::ApplicationInfo appInfo;
	appInfo.pApplicationName = mainApp->applicationName.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;

	vk::InstanceCreateInfo createInfo;
	createInfo.pApplicationInfo = &appInfo;

	std::vector<const char*> extensions;

	// Load all required extensions
	try {
		// Get all available extensions
		std::vector<vk::ExtensionProperties> availableExtensions(vk::enumerateInstanceExtensionProperties());

		// Get all extensions given the available required and requested
		extensions = getExtensions<vk::ExtensionProperties>(availableExtensions, requiredInstanceExtensions,
			requestedInstanceExtensions, [](const vk::ExtensionProperties& p) {
			
			// Map the vulkan extension type to a const char*
			return std::string(p.extensionName);
		});

		// Add the lower windowing API required extensions, usually don't need to check for support for these
		std::vector<const char*> windowRequiredExtensions(mainApp->getWindow().getInstanceExtensions());
		extensions.reserve(extensions.size() + windowRequiredExtensions.size());
		extensions.insert(extensions.end(), windowRequiredExtensions.begin(), windowRequiredExtensions.end());

		// Add the extensions to the create info
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();
	}
	catch (const std::exception& e) {
		console.error(e.what());
		exit(0);
	}

	// Validation Layers
	#ifdef KALE_DEBUG
	
	std::vector<const char*> layers;

	try {

		// Get all available layers
		std::vector<vk::LayerProperties> availableLayers(vk::enumerateInstanceLayerProperties());

		// Get all layers given the available required and requested
		layers = getExtensions<vk::LayerProperties>(availableLayers, requiredValidationLayers,
			requestedValidationLayers, [](const vk::LayerProperties& p) {
			
			// Map the vulkan layer type to a const char*
			return std::string(p.layerName);
		});

		// Add the layers to the create info
		createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
		createInfo.ppEnabledLayerNames = layers.data();
	}
	catch (const std::exception&) {
		console.warn("Validation Layers not Available");
		createInfo.enabledLayerCount = 0;
	}

	#else

	createInfo.enabledLayerCount = 0;

	#endif

	instance = vk::createInstanceUnique(createInfo);
}

/**
 * Chooses the physical device and sets up the physical device and logical device with the appropriate extensions
 * @param gpuID the ID of the GPU to use for rendering
 */
void Core::chooseDevice(std::optional<uint32_t> gpuID) {
	// Choose the GPU, useGPU will handle logical device creation
	if (gpuID.has_value()) device.init(gpuID.value());
	else {
		std::vector<vk::PhysicalDevice> devices(Device::availableDevices());
		if (devices.empty()) throw std::runtime_error("No Available GPU Found");
		device.init(devices[0]);
	}
}

/**
 * Sets up the command pool
 */
void Core::createCommandPool() {
	vk::CommandPoolCreateInfo createInfo(vk::CommandPoolCreateFlags(),
		device.queueIndices.graphicsFamilyIndex.value());
	commandPool = device.logicalDevice->createCommandPoolUnique(createInfo);
}
