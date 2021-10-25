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
#include <Kale/Vulkan/Extensions/Extensions.hpp>
#include <stdexcept>
#include <exception>

using namespace Kale;
using namespace Kale::Vulkan;

/**
 * Sets up the renderer, any functions called prior to this will result in undefined behavior
 * @param windowRequiredExtensions The required extensions form the lower level windowing API
 * @param gpuID the ID of the GPU to use for rendering
 */
void Renderer::setupRenderer(const std::vector<const char*>& windowRequiredExtensions, std::optional<uint32_t> gpuID) {

	try {
		createInstance(windowRequiredExtensions);

		#ifdef KALE_DEBUG
		setupDebugMessageCallback();
		#endif
		
		createSurface();

		// Choose the GPU, useGPU will handle logical device creation
		if (!gpuID.has_value()) {
			std::vector<std::tuple<uint32_t, std::string>> gpus = getAvailableGPUs();
			if (gpus.empty()) throw std::runtime_error("No Available GPU found.");
			useGPU(std::get<0>(gpus[0]));
		}
		else {
			useGPU(gpuID.value());
		}
	}
	catch (const std::exception& e) {
		console.error(e.what());
		exit(0);
	}
}

/**
 * Cleans vulkan objects before the application closes
 */
void Renderer::cleanupRenderer() {
	try {
		#ifdef KALE_DEBUG
		destroyDebugMessageCallback();
		#endif
		
		vkDestroySurfaceKHR(instance, surface, nullptr);
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
void Renderer::createInstance(const std::vector<const char*>& windowRequiredExtensions) {
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
	catch (const std::exception& e) {
		console.error("Validation Layers not Available");
		createInfo.enabledLayerCount = 0;
	}

	#else

	createInfo.enabledLayerCount = 0;

	#endif

	if (vk::createInstance(&createInfo, nullptr, &instance) != vk::Result::eSuccess) {
		console.error("Unable to Init Vulkan");
		exit(0);
	}
}

/**
 * Creates the vulkan window surface for rendering
 */
void Renderer::createSurface() {
	mainApp->getWindow().createWindowSurface(instance, surface);
}

/**
 * Creates the vulkan logical device object
 */
void Renderer::createLogicalDevice() {

	// Queues is not empty - meaning we've previously made a logical device which needs to be freed
	if (!queues.empty()) {

		// Clear all queues then destroy the logical device
		queues.clear();
		logicalDevice.destroy();
	}

	// Get all the required indices
	QueueFamilyIndices indices(physicalDevice, surface);
	
	// Create the queue create info
	std::vector<const float> priorities = {1.0f};
	std::unordered_set<uint32_t> uniqueIndices = indices.getUniqueIndices();
	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfo;

	// Populate the queue create info vector 
	for (uint32_t i : uniqueIndices) {
		queueCreateInfo.push_back(vk::DeviceQueueCreateInfo({}, i, priorities));
	}

	// Choose all required device features we desire
	vk::PhysicalDeviceFeatures features;
	// TODO - set the required device features to true

	// Set all required device extensions
	std::vector<const char*> extensions = getExtensions<vk::ExtensionProperties>(
		physicalDevice.enumerateDeviceExtensionProperties(), requiredDeviceExtensions,
		requestedDeviceExtensions, [](const vk::ExtensionProperties& p) {
		
		// Map extension proprty to a const char*
		return std::string(p.extensionName);
	});

	// Create the logical device create info
	vk::DeviceCreateInfo createInfo({}, queueCreateInfo, {}, extensions, &features);

	// Create the logical device
	logicalDevice = physicalDevice.createDevice(createInfo);
	queues[QueueType::Graphics] = logicalDevice.getQueue(indices.graphicsFamilyIndex.value(), 0);
	queues[QueueType::Presentation] = logicalDevice.getQueue(indices.presentFamilyIndex.value(), 0);
}