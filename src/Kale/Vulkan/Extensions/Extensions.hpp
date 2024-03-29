/*
   Copyright 2022 Rishi Challa

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

#ifdef KALE_VULKAN

#include <string>
#include <vector>
#include <functional>
#include <algorithm>

namespace Kale::Vulkan {

	/**
	 * Gets the list of vulkan extensions given the available extensions, required extensions, and requested extensions
	 * @throws Throws when a required extension is not found
	 * @param availableExtensions The available extensions directly from a vulkan enumeration
	 * @param requiredExtensions The required extensions, if any of these are missing an exception will be thrown
	 * @param requestedExtensions The requested extensions, these will be added if available
	 * @param mappingFn A lambda to map from a vulkan type or any other type to strings
	 * @returns The list of extensions accounting for all of the above
	 */
	template <typename T> std::vector<const char*> getExtensions(const std::vector<T>& availableExtensions,
		const std::vector<std::string>& requiredExtensions, const std::vector<std::string>& requestedExtensions,
		std::function<std::string (const T&)> mappingFn) {

		// Filter the extensions out
		std::vector<const char*> extensions;

		// Find all required extensions
		for (const std::string& requiredExtension : requiredExtensions) {

			// If a required extension isn't found then throw an error
			if (std::find_if(availableExtensions.begin(), availableExtensions.end(), [&](const T& availableExtension) {
				return requiredExtension == mappingFn(availableExtension);
			}) == availableExtensions.end())
				throw std::runtime_error("Unable to find required extension - " + requiredExtension);
			
			// Add the extension
			extensions.push_back(requiredExtension.c_str());
		}

		// Find requested extensions
		for (const std::string& requestedExtension : requestedExtensions) {

			// Skip this extension if it's not found
			if (std::find_if(availableExtensions.begin(), availableExtensions.end(), [&](const T& availableExtension) {
				return requestedExtension == mappingFn(availableExtension);
			}) == availableExtensions.end())
				continue;
			
			// If found add the extension
			extensions.push_back(requestedExtension.c_str());
		}

		// Return our processed extensions
		return extensions;
	}

	/**
	 * Vulkan Instance extensions which are required, if these extensions are not found then the program will exit
	 */
	const inline std::vector<std::string> requiredInstanceExtensions{
		VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME
	};

	/**
	 * Vulkan Instance extensions which are not required but will be used if present/available
	 */
	const inline std::vector<std::string> requestedInstanceExtensions = {
		#ifdef KALE_DEBUG
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME // Debug Utils, not a necessity but is nice in debug mode
		#endif
	};

	/**
	 * Vulkan Device extensions which are required, if these extensions are not found then the program will
	 * not consider the physical device for usage
	 */
	const inline std::vector<std::string> requiredDeviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	/**
	 * Vulkan Device extensions which are not required but will be used if present/available
	 */
	const inline std::vector<std::string> requestedDeviceExtensions = {
		"VK_KHR_portability_subset" // Specification states that if supported must be added
	};

	#ifdef KALE_DEBUG

	/**
	 * All the required validation layers used when debugging
	 */
	const inline std::vector<std::string> requiredValidationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	/**
	 * All the requested validation layers used when debugging
	 */
	const inline std::vector<std::string> requestedValidationLayers = {
		// Empty for now
	};

	#endif
}

#endif
