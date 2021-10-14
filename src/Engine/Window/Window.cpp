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

#include "Window.hpp"
#include <Engine/Application/Application.hpp>
#include <vector>
#include <sstream>

using namespace Islands;

#ifdef ISLANDS_DEBUG

const std::vector<const char*> Window::vulkanValidationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

#endif

/**
 * Starts listening to events, override the functions provided in EventHandler to handle the events
 */
void Window::registerEvents(EventHandler* handler) {
	eventHandlers.push_back(handler);
}

/**
 * Stops listening to events
 */
void Window::removeEvents(EventHandler* handler) {
	eventHandlers.remove(handler);
}

/**
 * Checks validation layer support for all given validation layers
 * @param validationLayers The validation layers to check support for, all of these must be present to return true
 * @returns Whether or not all validation layers given are supported
 */
static bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers) {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	
	for (const std::string& layerName : validationLayers) {
		bool layerFound = false;
		for (const auto& layerProperties : availableLayers) {
			if (std::string(layerName) == layerProperties.layerName) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

#ifdef ISLANDS_DEBUG

/**
 * The Vulkan Debug Validation Layer callback function
 * @param severity The severity of the message
 * @param type The type of the message
 * @param callbackData the callback data passed with the message (usually the message itself)
 * @param userData A user pointer - in our app will always be nullptr
 */
static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
	vk::DebugUtilsMessageTypeFlagsEXT type, const vk::DebugUtilsMessengerCallbackDataEXT* callbackData,
	void* userData) {
	
	switch (severity) {
		case vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose:
			print(callbackData->pMessage);
			return VK_FALSE;
		case vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo:
			info(callbackData->pMessage);
			return VK_FALSE;
		case vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning:
			warn(callbackData->pMessage);
			return VK_FALSE;
		case vk::DebugUtilsMessageSeverityFlagBitsEXT::eError:
			error(callbackData->pMessage);
			return VK_FALSE;
	}
}

/**
 * Sets up the debug message callback
 */
void Window::setupDebugMessageCallback() {
	// vk::DebugUtilsMessengerCreateInfoEXT createInfo;
	// createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	// createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	// createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	// createInfo.pfnUserCallback = debugCallback;
}

#endif

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

	if (!checkValidationLayerSupport(vulkanValidationLayers)) {
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

void Window::cleanupVulkan() {
	vulkanInstance.destroy();
}

/**
 * Initializes vulkan for use with both windowing APIs
 */
void Window::initVulkan() {
	createVulkanInstance();

	#ifdef ISLANDS_DEBUG
	setupDebugMessageCallback();
	#endif
}