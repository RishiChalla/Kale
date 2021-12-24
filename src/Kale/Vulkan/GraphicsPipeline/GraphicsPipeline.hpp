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

#ifdef KALE_VULKAN

#include <Kale/Vulkan/Shader/Shader.hpp>
#include <Kale/Vulkan/Renderer/Renderer.hpp>
#include <Kale/Vulkan/SwapChain/SwapChain.hpp>

#include <vulkan/vulkan.hpp>
#include <string>
#include <vector>

namespace Kale::Vulkan {

	/**
	 * Represents the vulkan graphics pipeline
	 */
	class GraphicsPipeline : public ChildResource<SwapChain> {
	private:

		/**
		 * Creates the pipeline layout
		 */
		void createPipelineLayout();

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
		 * Creates an uninitialized graphics pipeline
		 */
		GraphicsPipeline();

		/**
		 * Creates the graphics pipeline given the required shaders
		 * @param vert The filename of the vertex shader (the assets/shaders/ path is prepended automatically)
		 * @param frag The filename of the fragment shader (the assets/shaders/ path is prepended automatically)
		 * @param renderer The renderer this graphics pipeline will render to
		 */
		GraphicsPipeline(const std::string& vert, const std::string& frag, const Renderer& renderer);

		/**
		 * Sets up the graphics pipeline
		 * @param vert The vertex shader filename (the assets/shaders/ path is prepended automatically)
		 * @param frag The fragment shader filename (the assets/shaders/ path is prepended automatically)
		 * @param renderer The renderer this graphics pipeline will render to
		 */
		void init(const std::string& vert, const std::string& frag, const Renderer& renderer);

		/**
		 * Frees resources if not already freed
		 */
		void freeResources(bool remove = true) override;

	};
}

#endif
