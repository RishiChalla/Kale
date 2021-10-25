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

#include "SwapChainSupportDetails.hpp"

using namespace Kale;
using namespace Kale::Vulkan;

/**
 * Populates the struct given the physical device and the surface
 * @param physicalDevice The physical device to check for
 * @param surface The surface to check for
 */
SwapChainSupportDetails::SwapChainSupportDetails(const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface) :
	deviceId(physicalDevice.getProperties().deviceID), capabilities(physicalDevice.getSurfaceCapabilitiesKHR(surface)),
	formats(physicalDevice.getSurfaceFormatsKHR(surface)), presentModes(physicalDevice.getSurfacePresentModesKHR(surface)) {
	
	// Empty Body
}

/**
 * Checks whether or not the device is adequate for our purposes (Must have at least 1 format & present mode)
 * @returns True when there is at least one format and present mode
 */
bool SwapChainSupportDetails::deviceIsAdequate() const {
	return !formats.empty() && !presentModes.empty();
}

/**
 * Chooses a surface format which passes all required checks for use for rendering
 * @returns The chosen surface format
 */
const vk::SurfaceFormatKHR& SwapChainSupportDetails::chooseFormat() const {
	for (const vk::SurfaceFormatKHR& format : formats)
		if (format.format == vk::Format::eR8G8B8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
			return format;
	
	return formats[0];
}