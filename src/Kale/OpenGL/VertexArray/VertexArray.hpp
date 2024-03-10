/*
   Copyright 2022 Rishi Challa

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

#ifdef KALE_OPENGL

#include <Kale/OpenGL/Buffer/Buffer.hpp>

#include <array>
#include <vector>
#include <algorithm>

namespace Kale::OpenGL {

	/**
	 * Type of object to draw
	 */
	enum class DrawType : GLenum {
		Triangles = GL_TRIANGLES,
		Points = GL_POINTS,
		Lines = GL_LINES
	};

	/**
	 * Represents an array of vertices, NOT an OpenGL vertex array
	 * (although the class does use OpenGL vertex arrays internally)
	 * @tparam T the struct containing an individual vertex
	 * @tparam NFloats for each vector within the vertex the number of floats in the vector must be given
	 */
	template <typename T, size_t... NFloats> class VertexArray {
	private:

		/**
		 * The OpenGL Vertex Array ID
		 */
		unsigned int vertexArray;

		/**
		 * The total number of floats within a single vertex
		 */
		constexpr size_t numFloatsInVert() {
			return sizeof(T) / sizeof(float);
		}

		/**
		 * Checks if the underlying floats are equal between a float pointer and a vertex
		 * @param floatPtr The float pointer to the first float within the vertex
		 * @param vert The vertex to compare to
		 * @returns Whether or not they are equal
		 */
		inline bool vertFloatPtrEqual(const float* floatPtr, const T& vert) {
			return std::equal(floatPtr, floatPtr + numFloatsInVert(), reinterpret_cast<const float*>(&vert));
		}

		/**
		 * Condenses vertices from a container into the vertices and elements buffers and syncs them with the GPU
		 * @param verts The vertices
		 * @param usage The usage of the vertex array
		 */
		template <typename Container> void condenseVertices(const Container& verts, BufferUsage usage) {
			bind();

			vertices.data.reserve(verts.size());
			elements.data.reserve(verts.size());

			size_t vertIndex = 0;

			for (const T& vert : verts) {

				// Check for existing vertex in memory
				bool existingVertFound = false;
				size_t existingVertIndex;

				// Reverse iteration as same vertices are more likely to be near eachother in the array
				for (int i = static_cast<int>(elements.data.size()) - 1; i >= 0; i--) {

					// Check if the vert in memory/the vert are equal
					if (vertFloatPtrEqual(&vertices.data[numFloatsInVert() * elements.data[i]], vert)) {
						existingVertIndex = static_cast<size_t>(elements.data[i]);
						existingVertFound = true;
						break;
					}
				}

				// An existing vertex was found, just add it's index to the elements and continue
				if (existingVertFound) {
					elements.data.push_back(static_cast<unsigned int>(existingVertIndex));
					continue;
				}

				// No existing vertex was found, add both the vertex to memory and the index
				elements.data.push_back(static_cast<unsigned int>(vertIndex));
				vertIndex++;

				const float* vertLoc = reinterpret_cast<const float*>(&vert);
				vertices.data.insert(vertices.data.end(), vertLoc, vertLoc + numFloatsInVert());
			}

			vertices.allocBuffer(usage);
			elements.allocBuffer(usage);
			vertices.data.shrink_to_fit();
			elements.data.shrink_to_fit();
		}

		/**
		 * Creates the vertices and elements given the vertices container and a function to create the
		 * elements
		 * @param verts The vertices
		 * @param func The function for setting the elements
		 * @param usage The usage of the vertex array
		 */
		template <typename Verts, typename Func>
		void createVerticesAndElements(const Verts& verts, const Func& func, BufferUsage usage) {
			glGenVertexArrays(1, &vertexArray);
			bind();
			vertices.data.insert(vertices.data.begin(), reinterpret_cast<const float*>(verts.data()),
				reinterpret_cast<const float*>(verts.data() + verts.size()));
			func();
			vertices.allocBuffer(usage);
			elements.allocBuffer(usage);
		}

	public:

		/**
		 * The vertex data itself
		 */
		Buffer<float> vertices;

		/**
		 * The element buffer/index buffer
		 */
		Buffer<unsigned int> elements;
		
		/**
		 * Creates an empty vertex array
		 */
		VertexArray() : vertices(BufferType::VertexBuffer), elements(BufferType::ElementBuffer) {
			glGenVertexArrays(1, &vertexArray);
			bind();
			vertices.bind();
			elements.bind();
		}
		
		/**
		 * Creates a vertex array given the vertices
		 * @param verts The vertices
		 * @param usage The usage of the vertex array
		 * @param condense Whether or not to remove the duplicates of vertices given and condense it into elements
		 */
		template <size_t N>
		VertexArray(const std::array<T, N>& verts, BufferUsage usage, bool condense = true) : vertices(BufferType::VertexBuffer),
			elements(BufferType::ElementBuffer) {
			
			glGenVertexArrays(1, &vertexArray);
			if (condense) condenseVertices(verts, usage);
			else {
				bind();
				vertices.data.insert(vertices.data.begin(), reinterpret_cast<const float*>(verts.data()),
					reinterpret_cast<const float*>(verts.data() + verts.size()));
				vertices.allocBuffer(usage);
			}
		}
		
		/**
		 * Creates a vertex array given the vertices
		 * @param verts The vertices
		 * @param usage The usage of the vertex array
		 * @param condense Whether or not to remove the duplicates of vertices given and condense it into elements
		 */
		VertexArray(const std::vector<T>& verts, BufferUsage usage, bool condense = true) : vertices(BufferType::VertexBuffer),
			elements(BufferType::ElementBuffer) {
			
			glGenVertexArrays(1, &vertexArray);
			if (condense) condenseVertices(verts, usage);
			else {
				bind();
				vertices.data.insert(vertices.data.begin(), reinterpret_cast<const float*>(verts.data()),
					reinterpret_cast<const float*>(verts.data() + verts.size()));
				vertices.allocBuffer(usage);
			}
		}
		
		/**
		 * Creates a vertex array given the vertices and elements/indices
		 * @tparam N1 Size of vertices array
		 * @tparam N2 Size of indices array
		 * @tparam index_type Type of indices (should be numeric)
		 * @param verts The vertices
		 * @param indices The indices
		 * @param usage The usage of the vertex array
		 */
		template <size_t N1, size_t N2, typename index_type = unsigned int>
		VertexArray(const std::array<T, N1>& verts, const std::array<index_type, N2>& indices, BufferUsage usage) :
			vertices(BufferType::VertexBuffer), elements(BufferType::ElementBuffer) {

			createVerticesAndElements(verts, [&]() -> void {
				elements.data.reserve(indices.size());
				for (const index_type& i : indices) elements.data.push_back(static_cast<unsigned int>(i));
			}, usage);
		}
		
		/**
		 * Creates a vertex array given the vertices and elements/indices
		 * @tparam index_type The type of indices, should be numeric
		 * @param verts The vertices
		 * @param indices The indices
		 * @param usage The usage of the vertex array
		 */
		template <typename index_type = unsigned int>
		VertexArray(const std::vector<T>& verts, const std::vector<index_type>& indices, BufferUsage usage) :
			vertices(BufferType::VertexBuffer), elements(BufferType::ElementBuffer) {

			createVerticesAndElements(verts, [&]() -> void {
				elements.data.reserve(indices.size());
				for (const index_type& i : indices) elements.data.push_back(static_cast<unsigned int>(i));
			}, usage);
		}
		
		/**
		 * Creates a vertex array given the vertices and elements/indices
		 * @param verts The vertices
		 * @param indices The indices
		 * @param usage The usage of the vertex array
		 */
		VertexArray(const std::vector<T>& verts, std::vector<unsigned int>&& indices, BufferUsage usage) :
			vertices(BufferType::VertexBuffer), elements(BufferType::ElementBuffer) {

			createVerticesAndElements(verts, [&]() -> void {
				elements.data = std::move(indices);
			}, usage);
		}

		/**
		 * Frees resources
		 */
		~VertexArray() {
			glDeleteVertexArrays(1, &vertexArray);
		}

		/**
		 * Links the vertex array to the attributes of shaders. All shaders using this vertex array must use
		 * the correct attribute layouts.
		 * @param attributes An array of the attribute locations for each vertex component
		 */
		void enableAttributePointer(const std::array<unsigned int, sizeof...(NFloats)>& attributes) const {
			bind();
			const std::array<size_t, sizeof...(NFloats)> nFloatsArr = {NFloats...};
			const float* offset = nullptr;
			for (size_t i = 0; i < attributes.size(); i++) {
				glVertexAttribPointer(attributes[i], static_cast<GLint>(nFloatsArr[i]), GL_FLOAT, GL_FALSE,
					sizeof(T), static_cast<const void*>(offset));
				glEnableVertexAttribArray(attributes[i]);
				offset += nFloatsArr[i];
			}
		}

		/**
		 * Binds this vertex array for use externally
		 */
		void bind() const {
			glBindVertexArray(vertexArray);
		}

		/**
		 * Updates the vertices and condenses them
		 * NOTE - This condenses again and updates both elements and vertices. If you're only updating vertices
		 * and not elements it is highly recommended to directly update the buffers
		 * @param verts The vertices
		 * @param usage The usage of the vertex array
		 */
		template <size_t N> void updateVerticesCondense(const std::array<T, N>& verts, BufferUsage usage) {
			vertices.data.clear();
			elements.data.clear();
			condenseVertices(verts, usage);
		}

		/**
		 * Updates the vertices and condenses them
		 * NOTE - This condenses again and updates both elements and vertices. If you're only updating vertices
		 * and not elements it is highly recommended to directly update the buffers
		 * @param verts The vertices
		 * @param usage The usage of the vertex array
		 */
		void updateVerticesCondense(const std::vector<T>& verts, BufferUsage usage) {
			vertices.data.clear();
			elements.data.clear();
			condenseVertices(verts, usage);
		}

		/**
		 * Draws the vertex array as triangles
		 */
		void draw() const {
			bind();
			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(elements.data.size()), GL_UNSIGNED_INT, nullptr);
		}

		/**
		 * Draws the vertex array as triangles directly using the vertex buffer
		 */
		void drawNoElements() const {
			bind();
			glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.data.size()));
		}

		/**
		 * Draws the vertex array
		 * @param type The type of object to draw
		 */
		void draw(DrawType type) const {
			bind();
			glDrawElements(getEnumValue(type), static_cast<GLsizei>(elements.data.size()), GL_UNSIGNED_INT, nullptr);
		}

		/**
		 * Draws the vertex array directly using the vertex buffer
		 * @param type The type of object to draw
		 */
		void drawNoElements(DrawType type) const {
			bind();
			glDrawArrays(getEnumValue(type), 0, static_cast<GLsizei>(vertices.data.size()));
		}

	};

}

#endif
