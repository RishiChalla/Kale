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
#include <Kale/Vulkan/Shader/Shader.hpp>
#include <string>
#include <vector>

namespace Kale::Vulkan {

	/**
	 * Represents the vulkan graphics pipeline
	 */
	class GraphicsPipeline : public ChildResource {
	private:

		/**
		 * Sets up the pipeline layout
		 */
		vk::PipelineLayout setupPipelineLayout();

	public:

		/**
		 * The graphics pipeline
		 */
		vk::Pipeline pipeline;

		/**
		 * Creates an uninitialized graphics pipeline
		 */
		GraphicsPipeline();

		/**
		 * Creates the graphics pipeline given the required shaders
		 * @param vert The filename of the vertex shader (the assets/shaders/ path is prepended automatically)
		 * @param frag The filename of the fragment shader (the assets/shaders/ path is prepended automatically)
		 * @param device The device to create the graphics pipeline on
		 */
		GraphicsPipeline(const std::string& vert, const std::string& frag, Device& device);

		/**
		 * Sets up the graphics pipeline
		 * @param vert The vertex shader filename (the assets/shaders/ path is prepended automatically)
		 * @param frag The fragment shader filename (the assets/shaders/ path is prepended automatically)
		 * @param device The device to create the graphics pipeline on
		 */
		void init(const std::string& vert, const std::string& frag, Device& device);

		/**
		 * Graphics Pipelines do not support copying
		 */
		GraphicsPipeline(const GraphicsPipeline& other) = delete;

		/**
		 * Graphics Pipelines do not support copying
		 */
		void operator=(const GraphicsPipeline& other) = delete;

		/**
		 * Moves the graphics pipeline
		 * @param other Object to move from
		 */
		GraphicsPipeline(GraphicsPipeline&& other);
		
		/**
		 * Moves the graphics pipeline
		 * @param other Object to move from
		 */
		void operator=(GraphicsPipeline&& other);

		/**
		 * Frees resources if not already freed
		 */
		~GraphicsPipeline();

		/**
		 * Frees resources if not already freed
		 */
		void freeResources(bool remove = true) override;

	};
}