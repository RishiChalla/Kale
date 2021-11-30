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

#include <Kale/Vulkan/Shader/Shader.hpp>

#include <vulkan/vulkan.hpp>
#include <string>
#include <vector>

namespace Kale::Vulkan {

	/**
	 * Represents the vulkan graphics pipeline
	 */
	class GraphicsPipeline : public ChildResource<Device> {
	private:

		/**
		 * Creates the pipeline layout
		 */
		void createPipelineLayout();

		/**
		 * Creates the render pass object
		 */
		void createRenderPass();

	public:

		/**
		 * The graphics pipeline
		 */
		vk::UniquePipeline pipeline;

		/**
		 * The pipeline layout
		 */
		vk::UniquePipelineLayout layout;

		/**
		 * The render pass object for this graphics pipeline
		 */
		vk::UniqueRenderPass renderPass;

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
		GraphicsPipeline(const std::string& vert, const std::string& frag);

		/**
		 * Sets up the graphics pipeline
		 * @param vert The vertex shader filename (the assets/shaders/ path is prepended automatically)
		 * @param frag The fragment shader filename (the assets/shaders/ path is prepended automatically)
		 * @param device The device to create the graphics pipeline on
		 */
		void init(const std::string& vert, const std::string& frag);

		/**
		 * Frees resources if not already freed
		 */
		void freeResources(bool remove = true) override;

		/**
		 * Binds the graphics pipeline to a command buffer for drawing
		 * @param commandBuffer The command buffer to bind to
		 */
		void bind(const vk::UniqueCommandBuffer& commandBuffer) const;

	};
}
