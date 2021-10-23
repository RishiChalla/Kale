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

#include "Extensions.hpp"
#include <vulkan/vulkan.hpp>

using namespace Kale;
using namespace Kale::Vulkan;

/**
 * Vulkan Instance extensions which are required, if these extensions are not found then the program will exit
 */
const std::vector<std::string> Kale::Vulkan::requiredInstanceExtensions = {
	#ifdef KALE_DEBUG
	VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
	#endif
	VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME
};

/**
 * Vulkan Instance extensions which are not required but will be used if present/available
 */
const std::vector<std::string> Kale::Vulkan::requestedInstanceExtensions = {
	// Empty for now
};

/**
 * Vulkan Device extensions which are required, if these extensions are not found then the program will
 * not consider the physical device for usage
 */
const std::vector<std::string> Kale::Vulkan::requiredDeviceExtensions = {
	// Empty for now
};

/**
 * Vulkan Device extensions which are not required but will be used if present/available
 */
const std::vector<std::string> Kale::Vulkan::requestedDeviceExtensions = {
	"VK_KHR_portability_subset" // Specification states that if supported must be added
};

#ifdef KALE_DEBUG

/**
 * All the required validation layers used when debugging
 */
const std::vector<std::string> Kale::Vulkan::requiredValidationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

/**
 * All the requested validation layers used when debugging
 */
const std::vector<std::string> Kale::Vulkan::requestedValidationLayers = {
	// Empty for now
};

#endif