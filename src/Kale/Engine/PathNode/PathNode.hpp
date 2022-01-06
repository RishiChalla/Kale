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

#include <Kale/Core/Application/Application.hpp>
#include <Kale/Core/Node/Node.hpp>
#include <Kale/Math/Vector/Vector.hpp>
#include <Kale/Engine/Utils/Utils.hpp>

#ifdef KALE_OPENGL

#include <Kale/OpenGL/VertexArray/VertexArray.hpp>
#include <Kale/OpenGL/Shader/Shader.hpp>

#endif

#include <vector>
#include <limits>
#include <array>
#include <memory>
#include <algorithm>

namespace Kale {

	class PathNode : public Node {
	protected:

		/**
		 * The vertex type for path nodes.
		 * Path nodes only have positions, the color is uniform for the entire shape
		 */
		using Vertex = Vector2f;

		/**
		 * The location of the uniform within the shader for rendering this node
		 */
		inline static unsigned int cameraUniform = 0;
		
		/**
		 * The location of the uniform within the shader for rendering this node
		 */
		inline static unsigned int localUniform = 0;
		
		/**
		 * The location of the uniform within the shader for rendering this node
		 */
		inline static unsigned int vertexColorUniform = 0;
		
		/**
		 * The location of the uniform within the shader for rendering this node
		 */
		inline static unsigned int zPositionUniform = 0;

		/**
		 * The location of the attribute within the shader for rendering this node
		 */
		inline static unsigned int vertexPositionAttribute = 0;

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
		 * @param camera The camera to render with
		 */
		void render(const Camera& camera) override;

		/**
		 * Updates the node
		 * @param threadNum the index of this thread, ranged 0 - numUpdateThreads
		 * @param ups The amount of updates the current thread is doing in a second
		 */
		void update(size_t threadNum, float ups) override;

		/**
		 * Recalculates the center of the node based on the min and max averages
		 * @param path The path, can be arrays/vectors of Vector2f
		 */
		template <typename T>
		void recalculateCenter(const T& path) {
			Vector2f sum;
			std::for_each(path.begin(), path.end(), [&](const Vector2f& vec) { sum += vec; });
			center = sum / static_cast<float>(path.size());
		}

		/**
		 * Creates the path given the path
		 * @param path The path, can be arrays/vectors of Vector2f
		 */
		template <typename T>
		void createPath(const T& path) {
			recalculateCenter(path);

			std::tuple<std::vector<float>, std::vector<unsigned int>> bufferInfo = triangulatePathFloat(path);
			vertexArray.vertices.data.clear();
			vertexArray.vertices.data = std::move(std::get<0>(bufferInfo));
			vertexArray.vertices.updateBuffer(OpenGL::BufferUsage::Static);

			vertexArray.elements.data.clear();
			vertexArray.elements.data = std::move(std::get<1>(bufferInfo));
			vertexArray.elements.updateBuffer(OpenGL::BufferUsage::Static);
		}

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
		 * The center of the path (calculated by average of points).
		 * Recalculated on path updates, inherited nodes must call the method to calculate this manually
		 */
		Vector2f center;

		/**
		 * The scale of the path, can be used for shrinking or expanding the path as a whole without updating points
		 */
		Vector2f scale = Vector2f(1.0f, 1.0f);

		/**
		 * The rotation of the path in DEGREES, can be used for rotating the path without updating points
		 */
		float rotation = 0.0f;

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
		PathNode(const std::array<Vector2f, N>& path) : color(1.0f, 1.0f, 1.0f, 0.0f) {
			createShaders();
			createPath(path);
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
		PathNode(const std::array<Vector2f, N>& path, const Vector4f& color) : color(color) {
			createShaders();
			createPath(path);
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
			createPath(newPath);
		}
		
	};
}
