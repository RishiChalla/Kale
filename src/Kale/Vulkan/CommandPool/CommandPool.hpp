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

#include <Kale/Vulkan/ChildResource/ChildResource.hpp>
#include <Kale/Math/Vector/Vector.hpp>
#include <vulkan/vulkan.hpp>
#include <vector>

namespace Kale::Vulkan {

	/**
	 * Forward declaration of device class
	 */
	class Device;

	/**
	 * The Kale abstraction over vulkan command pools and command buffers
	 */
	class CommandPool : public ChildResource<Device> {
	private:

		/**
		 * Creates the command pool
		 */
		void createCommandPool();

		/**
		 * Creates the command buffers
		 * @param clearColor The color to use for clearing the screen
		 */
		void createCommandBuffers(const Vector4f& clearColor);

	public:

		/**
		 * The command pool for submitting commands to
		 */
		vk::CommandPool commandPool;

		/**
		 * The command buffers
		 */
		std::vector<vk::CommandBuffer> commandBuffers;

		/**
		 * Creates an uninstantiated object
		 */
		CommandPool();

		/**
		 * Creates a new command pool given the device
		 * @param device The device to link to
		 * @param clearColor The color to use for clearing the screen
		 */
		CommandPool(Device& device, const Vector4f& clearColor);

		/**
		 * Instantiates this command pool given the device
		 * @param device The device to link to
		 * @param clearColor The color to use for clearing the screen
		 */
		void init(Device& device, const Vector4f& clearColor);

		/**
		 * GPU Structures do not support copying
		 */
		CommandPool(const CommandPool& other) = delete;

		/**
		 * GPU Structures do not support copying
		 */
		void operator=(const CommandPool& other) = delete;

		/**
		 * Moves the other command pool to a new object
		 * @param other The command pool to move from
		 */
		CommandPool(CommandPool&& other);

		/**
		 * Moves the other command pool to this object
		 * @param other The command pool to move from
		 */
		void operator=(CommandPool&& other);

		/**
		 * Frees resources if not already freed
		 */
		~CommandPool();

		/**
		 * Frees resources
		 */
		void freeResources(bool remove = true) override;
	};
}