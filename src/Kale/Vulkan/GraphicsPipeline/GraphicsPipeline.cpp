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
void GraphicsPipeline::freeResources() {
	ChildResource::freeResources();
	if (parentPtr == nullptr) return;
	Device& device = *dynamic_cast<Device*>(parentPtr);
	device.logicalDevice.destroyPipeline(pipeline);
	parentPtr = nullptr;
}