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

#include <optional>
#include <unordered_set>
#include <vulkan/vulkan.hpp>

namespace Kale::Vulkan {

	/**
	 * The indices for each queue family of a physical device which we need
	 */
	class QueueFamilyIndices {
	public:

		/**
		 * The graphics queue family index of the device if it exists
		 */
		std::optional<uint32_t> graphicsFamilyIndex;

		/**
		 * The presentation family queue index
		 */
		std::optional<uint32_t> presentFamilyIndex;

		/**
		 * Creates an unitialized object
		 */
		QueueFamilyIndices();

		/**
		 * Gets all the required queue family indices
		 * @param device The physical device to check for
		 */
		QueueFamilyIndices(const vk::PhysicalDevice& device);

		/**
		 * Returns whether or not this physical device contains all required queue family indices
		 * @returns Whether or not this physical device contains all required queue family indices
		 */
		bool hasAllIndices() const;

		/**
		 * Gets an unordered set of all the unique indices
		 * @returns All the unique indices
		 */
		std::unordered_set<uint32_t> getUniqueIndices() const;
	};
}