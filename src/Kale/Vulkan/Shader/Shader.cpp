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

#include "Shader.hpp"
#include <Kale/Application/Application.hpp>
#include <Kale/Vulkan/Device/Device.hpp>
#include <fstream>
#include <stdexcept>

using namespace Kale;
using namespace Kale::Vulkan;

/**
 * Creates an uninitialized object
 */
Shader::Shader() {
	// Empty Body
}

/**
 * Creates and initializes the object
 * @param filename The filename of the shader to create from
 * @param type The type of shader
 * @param device The device to link the shader to
 * @throws If unable to open the file
 */
Shader::Shader(const std::string& filename, ShaderType type, Device& device) :
	ChildResource(dynamic_cast<ParentResource&>(device)), type(type) {
	std::vector<char> code = readFile(filename);
	createShaderModule(code);
}

/**
 * Initializes this object
 * @param filename The shader file to initialize from
 * @param type The type of shader this is
 * @param device The device to link the shader to
 * @throws If unable to open the file
 */
void Shader::init(const std::string filename, ShaderType type, Device& device) {
	ChildResource::init(dynamic_cast<ParentResource&>(device));
	this->type = type;

	std::vector<char> code = readFile(filename);
	createShaderModule(code);
}

/**
 * Reads a shader file
 * @param filename The file name
 * @returns the code
 * @throws If unable to open the file
 */
std::vector<char> Shader::readFile(const std::string& filename) const {
	std::ifstream file("." + mainApp->applicationName + "/assets/shaders/" + filename, std::ios::ate | std::ios::binary);
	if (!file.is_open()) throw std::runtime_error("Unable to open the file "+filename);
	size_t fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> code(fileSize);
	file.seekg(0);
	file.read(code.data(), fileSize);
	file.close();
	return code;
}

/**
 * Creates the shader module given the byte code
 * @param code The byte code
 */
void Shader::createShaderModule(const std::vector<char>& code) {
	vk::ShaderModuleCreateInfo createInfo(vk::ShaderModuleCreateFlags(), code.size(),
		reinterpret_cast<const uint32_t*>(code.data()));
	shader = dynamic_cast<Device*>(parentPtr)->logicalDevice.createShaderModule(createInfo);
}

/**
 * Moves a shader and steals its resources
 * @param other The shader to move from
 */
Shader::Shader(Shader&& other) : type(other.type), shader(other.shader) {
	if (other.parentPtr != nullptr)
		ChildResource::init(dynamic_cast<ParentResource&>(*other.parentPtr));
	other.parentPtr = nullptr;
}

/**
 * Move assignment
 * @param other The shader to move from
 */
void Shader::operator=(Shader&& other) {
	freeResources();
	if (other.parentPtr != nullptr)
		ChildResource::init(dynamic_cast<ParentResource&>(*other.parentPtr));
	type = other.type;
	shader = other.shader;
	parentPtr = other.parentPtr;
	other.parentPtr = nullptr;
}

/**
 * Frees resources if not already freed
 */
Shader::~Shader() {
	freeResources();
}

/**
 * Frees resources for this shader
 */
void Shader::freeResources(bool remove) {
	if (parentPtr == nullptr) return;
	dynamic_cast<Device*>(parentPtr)->logicalDevice.destroyShaderModule(shader);
	ChildResource::freeResources(remove);
	parentPtr = nullptr;
}

/**
 * Gets the shader stage from the type
 * @returns the shader stage
 */
vk::ShaderStageFlagBits Shader::getShaderStage() const {
	switch (type) {
		case ShaderType::Vertex:
			return vk::ShaderStageFlagBits::eVertex;
			break;
		case ShaderType::Fragment:
			return vk::ShaderStageFlagBits::eFragment;
			break;
		default:
			throw std::runtime_error("Invalid Shader Type");
	}
}

/**
 * Gets the shader pipeline create information from this shader module
 * @returns The shader pipeline create information
 */
vk::PipelineShaderStageCreateInfo Shader::getShaderPipelineInfo() const {
	vk::PipelineShaderStageCreateInfo createInfo;
	createInfo.stage = getShaderStage();
	createInfo.module = shader;
	createInfo.pName = "main"; // All kale shaders are REQUIRED to have a main function
	
	return createInfo;
}