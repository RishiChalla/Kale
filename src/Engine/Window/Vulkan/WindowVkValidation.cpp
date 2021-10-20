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

using namespace Islands;

#ifdef ISLANDS_DEBUG

const std::vector<const char*> Window::vulkanValidationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

/**
 * Checks validation layer support for all given validation layers
 * @returns Whether or not all validation layers given are supported
 */
bool Window::checkValidationLayerSupport() const {
	auto availableLayers = vk::enumerateInstanceLayerProperties();
	
	for (const std::string& layerName : vulkanValidationLayers) {
		bool layerFound = false;
		for (const auto& layerProperties : availableLayers) {
			if (layerName == layerProperties.layerName) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) return false;
	}

	return true;
}

/**
 * The Vulkan Debug Validation Layer callback function
 * @param severity The severity of the message
 * @param type The type of the message
 * @param callbackData the callback data passed with the message (usually the message itself)
 * @param userData A user pointer - in our app will always be nullptr
 */
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
	VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
	void* userData) {
	
	switch (vk::DebugUtilsMessageSeverityFlagBitsEXT(severity)) {
		#ifdef ISLANDS_VERBOSE
		case vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose:
			console.log(callbackData->pMessage);
			return VK_FALSE;
		#endif
		case vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo:
			console.info(callbackData->pMessage);
			return VK_FALSE;
		case vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning:
			console.warn(callbackData->pMessage);
			return VK_FALSE;
		case vk::DebugUtilsMessageSeverityFlagBitsEXT::eError:
			console.error(callbackData->pMessage);
			return VK_FALSE;
		default:
			return VK_FALSE;
	}
}

/**
 * Sets up the debug message callback
 */
void Window::setupDebugMessageCallback() {
	using MessageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT;
	using MessageType = vk::DebugUtilsMessageTypeFlagBitsEXT;

	vk::DebugUtilsMessengerCreateInfoEXT createInfo;
	#ifdef ISLANDS_VERBOSE
	createInfo.messageSeverity = MessageSeverity::eError | MessageSeverity::eWarning |
		MessageSeverity::eInfo | MessageSeverity::eVerbose;
	#else
	createInfo.messageSeverity = MessageSeverity::eError | MessageSeverity::eWarning |
		MessageSeverity::eInfo;
	#endif
	createInfo.messageType = MessageType::eGeneral | MessageType::ePerformance | MessageType::eValidation;
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = nullptr;

	auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(vulkanInstance, "vkCreateDebugUtilsMessengerEXT"));
	
	if (func == nullptr) {
		console.error("Unable to load Debug Utils Extension");
		return;
	}

	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo = &createInfo.operator const VkDebugUtilsMessengerCreateInfoEXT&();
	if (func(vulkanInstance, pCreateInfo, nullptr, &vulkanDebugMessenger) != VK_SUCCESS) {
		console.error("Failed to setup Debug Messenger");
		return;
	}
}

/**
 * Destroys the debug message callback
 */
void Window::destroyDebugMessageCallback() {
	auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(vulkanInstance, "vkDestroyDebugUtilsMessengerEXT"));

	if (func == nullptr) {
		console.error("Failed to load Debug Utils Deletion Extension");
		return;
	}

	func(vulkanInstance, vulkanDebugMessenger, nullptr);
}

#endif