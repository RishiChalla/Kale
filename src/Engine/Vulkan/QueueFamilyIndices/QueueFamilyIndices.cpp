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

using namespace Islands;
using namespace Islands::Vulkan;

/**
 * Gets all the required queue family indices given all the properties
 * @param deviceId the device to link to
 * @param properties All of the properties
 */
VulkanQueueFamilyIndices::VulkanQueueFamilyIndices(uint32_t deviceId, const std::vector<vk::QueueFamilyProperties>& properties) :
	physicalDeviceId(deviceId) {
	
	// Loop through the family properties and set each indices
	uint32_t i = 0;
	for (const vk::QueueFamilyProperties& familyProperties : properties) {
		// Check the queue flags and set the indices based on them
		if (familyProperties.queueFlags & vk::QueueFlagBits::eGraphics) graphicsFamilyIndex = i;

		// Break the loop if we've completed all indices
		if (hasAllIndices()) break;
		i++;
	}
}

/**
 * Returns whether or not this physical device contains all required queue family indices
 * @returns Whether or not this physical device contains all required queue family indices
 */
bool VulkanQueueFamilyIndices::hasAllIndices() const {
	return graphicsFamilyIndex.has_value();
}