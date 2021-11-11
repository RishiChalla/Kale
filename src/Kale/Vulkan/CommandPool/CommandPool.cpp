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
#include <Kale/Vulkan/Renderer/Renderer.hpp>

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
 * @param device The device to link to
 * @param clearColor The color to use for clearing the screen
 */
CommandPool::CommandPool(Device& device, const Vector4f& clearColor) : ChildResource(device) {
	createCommandPool();
	createCommandBuffers(clearColor);
}

/**
 * Instantiates this command pool given the device
 * @param device The device to link to
 * @param clearColor The color to use for clearing the screen
 */
void CommandPool::init(Device& device, const Vector4f& clearColor) {
	freeResources();
	ChildResource::init(device);

	createCommandPool();
	createCommandBuffers(clearColor);
}

/**
 * Creates the command pool
 */
void CommandPool::createCommandPool() {
	vk::CommandPoolCreateInfo createInfo(vk::CommandPoolCreateFlags(),
		parentPtr->queueIndices.graphicsFamilyIndex.value());
	commandPool = parentPtr->logicalDevice->createCommandPoolUnique(createInfo);
}

/**
 * Creates the command buffers
 * @param clearColor The color to use for clearing the screen
 */
void CommandPool::createCommandBuffers(const Vector4f& clearColor) {
	vk::CommandBufferAllocateInfo allocInfo(*commandPool, vk::CommandBufferLevel::ePrimary,
		static_cast<uint32_t>(renderer.swapchain.frameBuffers.size()));
	commandBuffers = parentPtr->logicalDevice->allocateCommandBuffers(allocInfo);

	// Start Command Buffer Recording
	vk::Rect2D renderArea({0, 0}, renderer.swapchain.extent);
	vk::CommandBufferBeginInfo cmdBeginInfo;
	vk::ClearValue clearValue(vk::ClearColorValue(std::array<float, 4>({clearColor.x, clearColor.y,
		clearColor.z, clearColor.w})));
	vk::Viewport viewport(0.0f, 0.0f, mainApp->getWindow().getSizeF().x,
		mainApp->getWindow().getSizeF().y, 0.0f, 1.0f);
	for (size_t i = 0; i < commandBuffers.size(); i++) {
		const vk::CommandBuffer& commandBuffer = commandBuffers[i];
		commandBuffer.begin(cmdBeginInfo);
		vk::RenderPassBeginInfo renderBeginInfo(renderer.pipeline.renderPass.get(),
			renderer.swapchain.frameBuffers[i].get(), renderArea, 1, &clearValue);
		commandBuffer.beginRenderPass(renderBeginInfo, vk::SubpassContents::eInline);
		renderer.pipeline.bind(commandBuffer);
		commandBuffer.setViewport(0, 1, &viewport);
		commandBuffer.draw(3, 1, 0, 0);
		commandBuffer.endRenderPass();
		commandBuffer.end();
	}
}

/**
 * Moves the other command pool to a new object
 * @param other The command pool to move from
 */
CommandPool::CommandPool(CommandPool&& other) : ChildResource(dynamic_cast<ChildResource&&>(other)),
	commandPool(std::move(other.commandPool)), commandBuffers(std::move(other.commandBuffers)) {
	// Empty Body
}

/**
 * Moves the other command pool to this object
 * @param other The command pool to move from
 */
void CommandPool::operator=(CommandPool&& other) {
	ChildResource::operator=(dynamic_cast<ChildResource&&>(other));
	commandPool = std::move(other.commandPool);
	commandBuffers = std::move(other.commandBuffers);
}

/**
 * Frees resources
 */
void CommandPool::freeResources(bool remove) {
	ChildResource::freeResources(remove);
	commandPool.reset();
}
