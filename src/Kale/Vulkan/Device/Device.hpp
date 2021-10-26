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

#include <Kale/Vulkan/QueueFamilyIndices/QueueFamilyIndices.hpp>
#include <vulkan/vulkan.hpp>
#include <map>

namespace Kale::Vulkan {

	/**
	 * All the types of queues used for the engine
	 */
	enum class QueueType {
		Graphics, Presentation
	};
	
	/**
	 * A simple Kale abstraction over the Vulkan Physical Device and Logical Devices
	 */
	struct Device {
		vk::Device logicalDevice;
		vk::PhysicalDeviceProperties physicalDeviceProprties;
		vk::PhysicalDevice physicalDevice;
		std::map<QueueType, vk::Queue> queueMap;
		QueueFamilyIndices queueIndices;
		
		static std::vector<vk::Device> availableDevices();
		vk::SwapchainKHR createSwapchain(const vk::SurfaceKHR& surface) const;
		
	};
}