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

#include <vector>
#include <vulkan/vulkan.hpp>

namespace Kale::Vulkan {
	
	/**
	 * Holds information regarding the swap chain support details for a physical device
	 */
	struct SwapChainSupportDetails {

		/**
		 * The id of the physical device this swap chain support details is linked to
		 */
		const uint32_t deviceId;

		/**
		 * The capabilities of the surface
		 */
		vk::SurfaceCapabilitiesKHR capabilities;

		/**
		 * The supported formats by this physical device and surface
		 */
		std::vector<vk::SurfaceFormatKHR> formats;

		/**
		 * The present modes supported by this physical device and surface
		 */
		std::vector<vk::PresentModeKHR> presentModes;

		/**
		 * Populates the struct given the physical device and the surface
		 * @param physicalDevice The physical device to check for
		 * @param surface The surface to check for
		 */
		SwapChainSupportDetails(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface);

		/**
		 * Checks whether or not the device is adequate for our purposes (Must have at least 1 format & present mode)
		 * @returns True when there is at least one format and present mode
		 */
		bool deviceIsAdequate() const;

		/**
		 * Chooses a surface format which passes all required checks for use for rendering
		 * @returns The chosen surface format
		 */
		const vk::SurfaceFormatKHR& chooseFormat() const;
	};
}