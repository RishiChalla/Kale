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

#include <vulkan/vulkan.hpp>
#include <string>
#include <vector>

namespace Kale::Vulkan {

	/**
	 * Represents the type of shader being used
	 */
	enum class ShaderType {
		Fragment, Vertex
	};

	/**
	 * Forward declaration of device
	 */
	class Device;

	/**
	 * Represents a simple Spir-V shader
	 */
	class Shader {

		/**
		 * Reads a shader file
		 * @param filename The file name
		 * @returns the code
		 * @throws If unable to open the file
		 */
		std::vector<char> readFile(const std::string& filename) const;

		/**
		 * Creates the shader module given the byte code
		 * @param code The byte code
		 */
		void createShaderModule(const std::vector<char>& code);

	public:

		/**
		 * The type of shader
		 */
		ShaderType type;

		/**
		 * The shader module
		 */
		vk::ShaderModule shader;

		/**
		 * Pointer to the device this shader is held on
		 */
		const Device* devicePtr = nullptr;

		/**
		 * Creates an unitialized object
		 */
		Shader();

		/**
		 * Creates and initializes the object
		 * @param filename The filename of the shader to create from
		 * @param type The type of shader
		 * @param device The device to link the shader to
		 * @throws If unable to open the file
		 */
		Shader(const std::string& filename, ShaderType type, const Device& device);

		/**
		 * Shaders don't support copying
		 */
		Shader(const Shader& other) = delete;

		/**
		 * Moves a shader and steals its resources
		 * @param other The shader to move from
		 */
		Shader(Shader&& other);

		/**
		 * Frees resources if not already freed
		 */
		~Shader();

		/**
		 * Shaders don't support copying
		 */
		void operator=(const Shader& other) = delete;

		/**
		 * Move assignment
		 * @param other The shader to move from
		 */
		void operator=(Shader&& other);

		/**
		 * Initializes this object
		 * @param filename The shader file to initialize from
		 * @param type The type of shader this is
		 * @param device The device to link the shader to
		 * @throws If unable to open the file
		 */
		void init(const std::string filename, ShaderType type, const Device& device);

		/**
		 * Frees resources for this shader
		 */
		void freeResources();

		/**
		 * Gets the shader stage from the type
		 * @returns the shader stage
		 */
		vk::ShaderStageFlagBits getShaderStage() const;

		/**
		 * Gets the shader pipeline create information from this shader module
		 * @returns The shader pipeline create information
		 */
		vk::PipelineShaderStageCreateInfo getShaderPipelineInfo() const;
	};
}