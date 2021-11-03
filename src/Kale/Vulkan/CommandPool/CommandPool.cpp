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

#include "CommandPool.hpp"
#include <Kale/Vulkan/Device/Device.hpp>

using namespace Kale;
using namespace Kale::Vulkan;

/**
 * Creates an uninstantiated object
 */
CommandPool::CommandPool() {
	// Empty Body
}

/**
 * Creates a new command pool given the device
 * @param device The device to linkt o
 */
CommandPool::CommandPool(Device& device) : ChildResource(device) {
	
}

/**
 * Instantiates this command pool given the device
 * @param device The device to link to
 */
void CommandPool::init(Device& device) {
	freeResources();
	ChildResource::init(device);
}

/**
 * Moves the other command pool to a new object
 * @param other The command pool to move from
 */
CommandPool::CommandPool(CommandPool&& other) : ChildResource(dynamic_cast<ChildResource&&>(other)),
	commandPool(other.commandPool), commandBuffers(other.commandBuffers) {
	other.parentPtr = nullptr;
}

/**
 * Moves the other command pool to this object
 * @param other The command pool to move from
 */
void CommandPool::operator=(CommandPool&& other) {
	freeResources();
	ChildResource::operator=(dynamic_cast<ChildResource&&>(other));
	commandPool = other.commandPool;
	commandBuffers = other.commandBuffers;
	other.parentPtr = nullptr;
}

/**
 * Frees resources if not already freed
 */
CommandPool::~CommandPool() {
	freeResources();
}

/**
 * Frees resources
 */
void CommandPool::freeResources(bool remove) {
	if (parentPtr == nullptr) return;
	parentPtr->logicalDevice.destroyCommandPool(commandPool);
}
