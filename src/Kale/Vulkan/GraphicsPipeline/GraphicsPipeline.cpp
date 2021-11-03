/*
   Copyright 2021 Rishi Challa

   Licensed under the Apache License, Version 2.0 (the "License") {
	
}
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "GraphicsPipeline.hpp"
#include <Kale/Vulkan/Renderer/Renderer.hpp>
#include <Kale/Vulkan/Device/Device.hpp>
#include <array>

using namespace Kale;
using namespace Kale::Vulkan;

/**
 * Creates an uninitialized graphics pipeline
 */
GraphicsPipeline::GraphicsPipeline() {
	// Empty Body
}

/**
 * Sets up the graphics pipeline
 * @param vert The vertex shader filename (the assets/shaders/ path is prepended automatically)
 * @param frag The fragment shader filename (the assets/shaders/ path is prepended automatically)
 * @param device The device to create the graphics pipeline on
 */
GraphicsPipeline::GraphicsPipeline(const std::string& vert, const std::string& frag, Device& device) {
	init(vert, frag, device);
}

/**
 * Moves the graphics pipeline
 * @param other Object to move from
 */
GraphicsPipeline::GraphicsPipeline(GraphicsPipeline&& other) : ChildResource(dynamic_cast<ChildResource&&>(other)),
	pipeline(other.pipeline) {
	other.parentPtr = nullptr;
}

/**
 * Moves the graphics pipeline
 * @param other Object to move from
 */
void GraphicsPipeline::operator=(GraphicsPipeline&& other) {
	freeResources();
	ChildResource::operator=(dynamic_cast<ChildResource&&>(other));
	pipeline = other.pipeline;
	other.parentPtr = nullptr;
}

/**
 * Creates the pipeline layout
 */
void GraphicsPipeline::createPipelineLayout() {
	vk::PipelineLayoutCreateInfo layoutCreateInfo;
	layout = parentPtr->logicalDevice.createPipelineLayout(layoutCreateInfo);
}

/**
 * Creates the render pass object
 */
void GraphicsPipeline::createRenderPass() {
	vk::AttachmentDescription colorAttachment(vk::AttachmentDescriptionFlags(), renderer.swapchain.format,
		vk::SampleCountFlagBits::e1, vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore,
		vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined,
		vk::ImageLayout::ePresentSrcKHR);
	
	vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eAttachmentOptimalKHR);
	vk::SubpassDescription subpass(vk::SubpassDescriptionFlags(), vk::PipelineBindPoint::eGraphics);
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	vk::RenderPassCreateInfo createInfo(vk::RenderPassCreateFlags(), 1, &colorAttachment, 1, &subpass);
	renderPass = parentPtr->logicalDevice.createRenderPass(createInfo);
}

/**
 * Sets up the graphics pipeline
 * @param vert The vertex shader filename (the assets/shaders/ path is prepended automatically)
 * @param frag The fragment shader filename (the assets/shaders/ path is prepended automatically)
 * @param device The device to create the graphics pipeline on
 */
void GraphicsPipeline::init(const std::string& vert, const std::string& frag, Device& device) {
	ChildResource::init(device);

	// Shaders
	Shader vertShader(vert, ShaderType::Vertex, device);
	Shader fragShader(frag, ShaderType::Fragment, device);
	std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStage = {
		vertShader.getShaderPipelineInfo(),
		fragShader.getShaderPipelineInfo()
	};

	// Vertex Input - Choose defaults
	vk::PipelineVertexInputStateCreateInfo vertexCreateInfo;

	// Input Assembly
	vk::PipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo(vk::PipelineInputAssemblyStateCreateFlags(),
		vk::PrimitiveTopology::eTriangleList, VK_FALSE);

	// Viewports & Scissors
	vk::Viewport viewport(0.0f, 0.0f, renderer.swapchain.extent.width, renderer.swapchain.extent.height, 0.0f, 1.0f);
	vk::Rect2D scissors(vk::Offset2D(), renderer.swapchain.extent);
	vk::PipelineViewportStateCreateInfo viewportCreateInfo(vk::PipelineViewportStateCreateFlags(),
		1, &viewport, 1, &scissors);

	// Rasterizer
	vk::PipelineRasterizationStateCreateInfo rasterizerCreateInfo;
	rasterizerCreateInfo.polygonMode = vk::PolygonMode::eFill;
	rasterizerCreateInfo.cullMode = vk::CullModeFlagBits::eBack;
	rasterizerCreateInfo.lineWidth = 1.0f;
	rasterizerCreateInfo.frontFace = vk::FrontFace::eClockwise;

	// Multisampling
	vk::PipelineMultisampleStateCreateInfo multisamplingCreateInfo;
	multisamplingCreateInfo.minSampleShading = 1.0f;

	// Color Blending
	vk::PipelineColorBlendAttachmentState attachmentState(VK_TRUE, vk::BlendFactor::eSrcAlpha,
		vk::BlendFactor::eOneMinusSrcAlpha, vk::BlendOp::eAdd, vk::BlendFactor::eOne,
		vk::BlendFactor::eZero, vk::BlendOp::eAdd, vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
		vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);

	vk::PipelineColorBlendStateCreateInfo colorBlendingCreateInfo(vk::PipelineColorBlendStateCreateFlags(),
		VK_FALSE, vk::LogicOp::eCopy, 1, &attachmentState);

	// Dynamic State - Viewport will not change on android/ios
	#if defined(KALE_IOS) || defined(KALE_ANDROID)
	std::vector<vk::DynamicState> dynamicStates;
	#else
	std::vector<vk::DynamicState> dynamicStates = {vk::DynamicState::eViewport};
	#endif
	vk::PipelineDynamicStateCreateInfo dynamicCreateInfo(vk::PipelineDynamicStateCreateFlags(), dynamicStates);

	// Pipeline Layout/Render Pass
	createPipelineLayout();
	createRenderPass();

	// Pipeline Create Info
	vk::GraphicsPipelineCreateInfo createInfo(vk::PipelineCreateFlags(), shaderStage, &vertexCreateInfo,
		&inputAssemblyCreateInfo, nullptr, &viewportCreateInfo, &rasterizerCreateInfo, &multisamplingCreateInfo,
		nullptr, &colorBlendingCreateInfo, &dynamicCreateInfo, layout, renderPass);
	pipeline = device.logicalDevice.createGraphicsPipeline(VK_NULL_HANDLE, createInfo).value;
}

/**
 * Frees resources if not already freed
 */
GraphicsPipeline::~GraphicsPipeline() {
	freeResources();
}

/**
 * Frees resources if not already freed
 */
void GraphicsPipeline::freeResources(bool remove) {
	if (parentPtr == nullptr) return;
	parentPtr->logicalDevice.destroyPipelineLayout(layout);
	parentPtr->logicalDevice.destroyRenderPass(renderPass);
	parentPtr->logicalDevice.destroyPipeline(pipeline);
	ChildResource::freeResources(remove);
	parentPtr = nullptr;
}