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
GraphicsPipeline::GraphicsPipeline(GraphicsPipeline&& other) : pipeline(other.pipeline) {
	if (other.parentPtr != nullptr)
		ChildResource::init(dynamic_cast<ParentResource&>(*other.parentPtr));
	other.parentPtr = nullptr;
}

/**
 * Moves the graphics pipeline
 * @param other Object to move from
 */
void GraphicsPipeline::operator=(GraphicsPipeline&& other) {
	freeResources();
	parentPtr = other.parentPtr;
	pipeline = other.pipeline;
	if (other.parentPtr != nullptr)
		ChildResource::init(dynamic_cast<ParentResource&>(*other.parentPtr));
	other.parentPtr = nullptr;
}

/**
 * Sets up the pipeline layout
 */
vk::PipelineLayout GraphicsPipeline::setupPipelineLayout() {

	// Vertex Input - Choose defaults
	vk::PipelineVertexInputStateCreateInfo vertexCreateInfo;

	// Input Assembly
	vk::PipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo(vk::PipelineInputAssemblyStateCreateFlags(),
		vk::PrimitiveTopology::eTriangleList, VK_FALSE);

	// Viewports & Scissors
	std::vector<vk::Viewport> viewports;
	viewports.emplace_back(0.0f, 0.0f, renderer.swapchain.extent.width, renderer.swapchain.extent.height, 0.0f, 1.0f);
	std::vector<vk::Rect2D> scissors;
	scissors.emplace_back(vk::Offset2D(), renderer.swapchain.extent);
	vk::PipelineViewportStateCreateInfo viewportCreateInfo(vk::PipelineViewportStateCreateFlags(),
		viewports, scissors);

	// Rasterizer
	vk::PipelineRasterizationStateCreateInfo rasterizerCreateInfo;
	rasterizerCreateInfo.polygonMode = vk::PolygonMode::eFill;
	rasterizerCreateInfo.lineWidth = 1.0f;
	rasterizerCreateInfo.cullMode = vk::CullModeFlagBits::eBack;
	rasterizerCreateInfo.frontFace = vk::FrontFace::eClockwise;

	// Multisampling
	vk::PipelineMultisampleStateCreateInfo multisamplingCreateInfo;
	multisamplingCreateInfo.minSampleShading = 1.0f;

	// Depth & Stencil Testing - Not using

	// Color Blending
	vk::PipelineColorBlendAttachmentState attachmentState;
	attachmentState.srcColorBlendFactor = vk::BlendFactor::eOne;
	attachmentState.srcAlphaBlendFactor = vk::BlendFactor::eOne;
	attachmentState.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
		vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;

	/*
	vk::PipelineColorBlendStateCreateFlags flags_
	vk::Bool32 logicOpEnable_
	vk::LogicOp logicOp_
	const vk::ArrayProxyNoTemporaries<const vk::PipelineColorBlendAttachmentState> &attachments_
	const std::__1::array<float
	4UL> &blendConstants_ = {}
	*/
	// vk::PipelineColorBlendStateCreateInfo colorBlendingCreateInfo(vk::PipelineColorBlendStateCreateFlags(), );

	// Dynamic State

	// Pipeline Layout

	return vk::PipelineLayout();
}

/**
 * Sets up the graphics pipeline
 * @param vert The vertex shader filename (the assets/shaders/ path is prepended automatically)
 * @param frag The fragment shader filename (the assets/shaders/ path is prepended automatically)
 * @param device The device to create the graphics pipeline on
 */
void GraphicsPipeline::init(const std::string& vert, const std::string& frag, Device& device) {
	ChildResource::init(dynamic_cast<ParentResource&>(device));
	Shader vertShader(vert, ShaderType::Vertex, device);
	Shader fragShader(vert, ShaderType::Fragment, device);
	std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStage = {
		vertShader.getShaderPipelineInfo(),
		fragShader.getShaderPipelineInfo()
	};

	vk::PipelineLayout layout(setupPipelineLayout());
	
	// Pipeline Creation Code

	device.logicalDevice.destroyPipelineLayout(layout);
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
	Device& device = *dynamic_cast<Device*>(parentPtr);
	device.logicalDevice.destroyPipeline(pipeline);
	ChildResource::freeResources(remove);
	parentPtr = nullptr;
}