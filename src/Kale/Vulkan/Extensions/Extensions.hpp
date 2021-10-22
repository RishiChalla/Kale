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

#pragma once

#include <string>
#include <vector>

namespace Kale::Vulkan {

	/**
	 * Vulkan Instance extensions which are required, if these extensions are not found then the program will exit
	 */
	const std::vector<std::string> vulkanRequiredInstanceExtensions = {
		// Empty for now
	};

	/**
	 * Vulkan Instance extensions which are not required but will be used if present/available
	 */
	const std::vector<std::string> vulkanRequestedInstanceExtensions = {
		// Empty for now
	};

	/**
	 * Vulkan Device extensions which are required, if these extensions are not found then the program will
	 * not consider the physical device for usage
	 */
	const std::vector<std::string> vulkanRequiredDeviceExtensions = {
		// Empty for now
	};

	/**
	 * Vulkan Device extensions which are not required but will be used if present/available
	 */
	const std::vector<std::string> vulkanRequestedDeviceExtensions = {
		// Empty for now
	};

	/**
	 * Gets the list of vulkan extensions given the available extensions, required extensions, and requested extensions
	 * @throws Throws when a required extension is not found
	 * @param availableExtensions The available extensions directly from a vulkan enumeration
	 * @param requiredExtensions The required extensions, if any of these are missing an exception will be thrown
	 * @param requestedExtensions The requested extensions, these will be added if available
	 * @returns The list of extensions accounting for all of the above
	 */
	std::vector<const char*> getVulkanExtensions(const std::vector<const char*>& availableExtensions,
		const std::vector<std::string>& requiredExtensions, const std::vector<std::string>& requestedExtensions);
}