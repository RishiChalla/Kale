/*
   Copyright 2022 Rishi Challa

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

#ifdef KALE_VULKAN

#include "GraphicsPipeline.hpp"

#include <Kale/Vulkan/Core/Core.hpp>
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
 * @param renderer The renderer this graphics pipeline will render to
 */
GraphicsPipeline::GraphicsPipeline(const std::string& vert, const std::string& frag, const Renderer& renderer) {
	init(vert, frag, renderer);
}

/**
 * Creates the pipeline layout
 */
void GraphicsPipeline::createPipelineLayout() {
	vk::PipelineLayoutCreateInfo layoutCreateInfo;
	layout = Core::device.logicalDevice->createPipelineLayoutUnique(layoutCreateInfo);
}

/**
 * Sets up the graphics pipeline
 * @param vert The vertex shader filename (the assets/shaders/ path is prepended automatically)
 * @param frag The fragment shader filename (the assets/shaders/ path is prepended automatically)
 * @param renderer The renderer this graphics pipeline will render to
 */
void GraphicsPipeline::init(const std::string& vert, const std::string& frag, const Renderer& renderer) {
	ChildResource::init(Core::swapchain);

	// Shaders
	Shader vertShader(vert, ShaderType::Vertex, Core::device);
	Shader fragShader(frag, ShaderType::Fragment, Core::device);
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
	vk::Viewport viewport(0.0f, 0.0f, static_cast<float>(Core::swapchain.extent.width),
		static_cast<float>(Core::swapchain.extent.height), 0.0f, 1.0f);
	vk::Rect2D scissors(vk::Offset2D(), Core::swapchain.extent);
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
	vk::PipelineDynamicStateCreateInfo dynamicCreateInfo(vk::PipelineDynamicStateCreateFlags(),
		static_cast<uint32_t>(dynamicStates.size()), dynamicStates.data());

	// Pipeline Layout
	createPipelineLayout();

	// Pipeline Create Info
	vk::GraphicsPipelineCreateInfo createInfo(vk::PipelineCreateFlags(), 2, shaderStage.data(), &vertexCreateInfo,
		&inputAssemblyCreateInfo, nullptr, &viewportCreateInfo, &rasterizerCreateInfo, &multisamplingCreateInfo,
		nullptr, &colorBlendingCreateInfo, &dynamicCreateInfo, layout.get(), renderer.renderPass.get());
	pipeline = Core::device.logicalDevice->createGraphicsPipelineUnique({}, createInfo).value;
}

/**
 * Frees resources if not already freed
 */
void GraphicsPipeline::freeResources(bool remove) {
	ChildResource::freeResources(remove);
	layout.reset();
	pipeline.reset();
}

#endif
