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

namespace Kale::Vulkan {

	/**
	 * Represents the vulkan graphics pipeline
	 */
	class GraphicsPipeline {
	private:
		void setupInputAssembler();
		void setupVertexShaders(const std::string& filename);
		void setupTessellation();
		void setupGeometry();
		void setupRasterization();
		void setupFragmentShaders(const std::string& filename);
		void seutpColorBlending();

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
		 */
		GraphicsPipeline(const std::string& vert, const std::string& frag);
	};
}