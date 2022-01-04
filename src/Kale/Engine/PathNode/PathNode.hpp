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

#include <Kale/Core/Node/Node.hpp>
#include <Kale/Math/Vector/Vector.hpp>
#include <Kale/Engine/Utils/Utils.hpp>

#ifdef KALE_OPENGL

#include <Kale/OpenGL/VertexArray/VertexArray.hpp>
#include <Kale/OpenGL/Shader/Shader.hpp>

#endif

#include <vector>
#include <array>
#include <memory>

namespace Kale {

	class PathNode : public Node {
	protected:

		/**
		 * The vertex type for path nodes.
		 * Path nodes only have positions, the color is uniform for the entire shape
		 */
		using Vertex = Vector2f;

#ifdef KALE_OPENGL

		/**
		 * The vertex array containing the vertices on both the GPU and CPU
		 */
		OpenGL::VertexArray<Vertex, 2> vertexArray;

		/**
		 * The shader program used for rendering path nodes
		 */
		static std::unique_ptr<const OpenGL::Shader> shader;

		/**
		 * Creates the shaders
		 */
		void createShaders();

#endif

#ifdef KALE_VULKAN

		// TODO - Vulkan Vertex Array & Shader

#endif

		/**
		 * Renders the node
		 * @param renderer The renderer to render to
		 */
		void render() override;

		/**
		 * Updates the node
		 * @param threadNum the index of this thread, ranged 0 - numUpdateThreads
		 * @param ups The amount of updates the current thread is doing in a second
		 */
		void update(size_t threadNum, float ups) override;

	public:

		/**
		 * The color of this pathnode
		 */
		Vector4f color;

		/**
		 * The z position of this node
		 */
		float zPosition = 0.0f;

		/**
		 * Creates an empty path node
		 */
		PathNode();

		/**
		 * Creates a path node given the path
		 * @param path The path
		 */
		PathNode(const std::vector<Vector2f>& path);

		/**
		 * Creates a path node given the path
		 * @param path The path
		 */
		template <size_t N>
		PathNode(const std::array<Vector2f, N>& path) : vertexArray(path, true), color(1.0f, 1.0f, 1.0f, 0.0f) {
			createShaders();
		}

		/**
		 * Creates a path node given the path and color
		 * @param path The path
		 * @param color The color
		 */
		PathNode(const std::vector<Vector2f>& path, const Vector4f& color);

		/**
		 * Creates a path node given the path and color
		 * @param path The path
		 * @param color The color
		 */
		template <size_t N>
		PathNode(const std::array<Vector2f, N>& path, const Vector4f& color) : vertexArray(path, true), color(color) {
			createShaders();
		}

		/**
		 * Updates the path to a new one
		 * @param newPath the path to update to
		 * @param newPath the path to update to
		 */
		void updatePath(const std::vector<Vector2f>& newPath);

		/**
		 * Updates the path to a new one
		 * @param newPath the path to update to
		 * @param newPath the path to update to
		 */
		template <size_t N> void updatePath(const std::array<Vector2f, N>& newPath) {
			std::tuple<std::vector<float>, std::vector<unsigned int>> bufferInfo = triangulatePathFloat(newPath);
			vertexArray.vertices.data.clear();
			vertexArray.vertices.data = std::move(std::get<0>(bufferInfo));
			vertexArray.vertices.updateBuffer(OpenGL::BufferUsage::Static);

			vertexArray.elements.data.clear();
			vertexArray.elements.data = std::move(std::get<1>(bufferInfo));
			vertexArray.elements.updateBuffer(OpenGL::BufferUsage::Static);
		}
		
	};
}
