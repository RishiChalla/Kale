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

#include "QueueFamilyIndices.hpp"

using namespace Kale;
using namespace Kale::Vulkan;

/**
 * Gets all the required queue family indices
 * @param device The physical device to check for
 * @param surface The surface for rendering
 */
QueueFamilyIndices::QueueFamilyIndices(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface) :
	physicalDeviceId(device.getProperties().deviceID) {
	
	// Loop through the family properties and set each indices
	uint32_t i = 0;
	for (const vk::QueueFamilyProperties& familyProperties : device.getQueueFamilyProperties()) {
		// Check the queue flags and set the indices based on them
		if (familyProperties.queueFlags & vk::QueueFlagBits::eGraphics) graphicsFamilyIndex = i;
		if (device.getSurfaceSupportKHR(i, surface)) presentFamilyIndex = i;

		// Break the loop if we've completed all indices
		if (hasAllIndices()) break;
		i++;
	}
}

/**
 * Returns whether or not this physical device contains all required queue family indices
 * @returns Whether or not this physical device contains all required queue family indices
 */
bool QueueFamilyIndices::hasAllIndices() const {
	return graphicsFamilyIndex.has_value() && presentFamilyIndex.has_value();
}

/**
 * Gets an unordered set of all the unique indices
 * @returns All the unique indices
 */
std::unordered_set<uint32_t> QueueFamilyIndices::getUniqueIndices() const {
	std::unordered_set<uint32_t> set;
	if (graphicsFamilyIndex.has_value()) set.insert(graphicsFamilyIndex.value());
	if (presentFamilyIndex.has_value()) set.insert(presentFamilyIndex.value());
	return set;
}