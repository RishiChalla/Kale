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

#ifdef KALE_VULKAN

#include "Renderer.hpp"

#include <Kale/Vulkan/Core/Core.hpp>

using namespace Kale;
using namespace Kale::Vulkan;

/**
 * Creates a new renderer given the device to create it from
 */
Renderer::Renderer() : ChildResource(Core::swapchain) {
	createRenderPass();
}

/**
 * Initializes the object
 */
void Renderer::init() {
	ChildResource::init(Core::swapchain);
	createRenderPass();
}

/**
 * Creates the render pass
 */
void Renderer::createRenderPass() {
	vk::AttachmentDescription colorAttachment(vk::AttachmentDescriptionFlags(), Core::swapchain.format,
		vk::SampleCountFlagBits::e1, vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore,
		vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined,
		vk::ImageLayout::ePresentSrcKHR);
	
	vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);
	vk::SubpassDescription subpass(vk::SubpassDescriptionFlags(), vk::PipelineBindPoint::eGraphics);
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	vk::RenderPassCreateInfo createInfo(vk::RenderPassCreateFlags(), 1, &colorAttachment, 1, &subpass);
	renderPass = Core::device.logicalDevice->createRenderPassUnique(createInfo);
}

/**
 * Frees resources if not already freed
 */
void Renderer::freeResources(bool remove) {
	ChildResource::freeResources(remove);
	ParentResource::freeChildren();
	renderPass.reset();
}

#ifdef KALE_DEBUG

/**
 * Gets the swapchain for debug assertions
 */
const SwapChain* Renderer::getSwapChain() const {
	return parentPtr;
}

#endif

#endif
