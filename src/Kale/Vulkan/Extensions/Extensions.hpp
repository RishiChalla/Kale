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
	extern const std::vector<std::string> requiredInstanceExtensions;

	/**
	 * Vulkan Instance extensions which are not required but will be used if present/available
	 */
	extern const std::vector<std::string> requestedInstanceExtensions;

	/**
	 * Vulkan Device extensions which are required, if these extensions are not found then the program will
	 * not consider the physical device for usage
	 */
	extern const std::vector<std::string> requiredDeviceExtensions;

	/**
	 * Vulkan Device extensions which are not required but will be used if present/available
	 */
	extern const std::vector<std::string> requestedDeviceExtensions;

	#ifdef KALE_DEBUG

	/**
	 * All the required validation layers used when debugging
	 */
	extern const std::vector<std::string> requiredValidationLayers;

	/**
	 * All the requested validation layers used when debugging
	 */
	extern const std::vector<std::string> requestedValidationLayers;

	#endif
}