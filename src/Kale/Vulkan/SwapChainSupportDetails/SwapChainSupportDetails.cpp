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

#include <Kale/Core/Application/Application.hpp>
#include <Kale/Vulkan/Core/Core.hpp>

#include <limits>
#include <algorithm>

using namespace Kale;
using namespace Kale::Vulkan;

/**
 * Creates an unitialized object
 */
SwapChainSupportDetails::SwapChainSupportDetails() {
	// Empty Body
}

/**
 * Populates the struct given the physical device and the surface
 * @param physicalDevice The physical device to check for
 */
SwapChainSupportDetails::SwapChainSupportDetails(const vk::PhysicalDevice& physicalDevice) :
	capabilities(physicalDevice.getSurfaceCapabilitiesKHR(Core::surface.get())),
	formats(physicalDevice.getSurfaceFormatsKHR(Core::surface.get())),
	presentModes(physicalDevice.getSurfacePresentModesKHR(Core::surface.get())) {
	
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
vk::SurfaceFormatKHR SwapChainSupportDetails::chooseFormat() const {
	for (const vk::SurfaceFormatKHR& format : formats)
		if (format.format == vk::Format::eR8G8B8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
			return format;
	
	return formats[0];
}

/**
 * Chooses a presentation mode for the application to use
 * @returns The chosen presentation mode
 */
vk::PresentModeKHR SwapChainSupportDetails::choosePresentMode() const {
	// TODO - Add configuration settings for high/medium/low graphics
	if (std::find(presentModes.begin(), presentModes.end(), vk::PresentModeKHR::eMailbox) != presentModes.end())
		return vk::PresentModeKHR::eMailbox;
	else
		return vk::PresentModeKHR::eFifo;
}

/**
 * Chooses The resolution of the swap chain images
 * @returns The resolution of the swap chain images
 */
vk::Extent2D SwapChainSupportDetails::chooseSwapExtent() const {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		// We're required to use the vulkan/window currnet extent
		return capabilities.currentExtent;
	}
	else {
		// Get the window size and clamp it then return
		Vector2ui32 winSize = mainApp->getWindow().getSize().cast<uint32_t>();
		winSize.clampTo(capabilities.minImageExtent.width, capabilities.maxImageExtent.width,
			capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
		return vk::Extent2D{winSize.x, winSize.y};
	}
}