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

#ifdef KALE_OPENGL

#include <Kale/OpenGL/Buffer/Buffer.hpp>

#include <array>
#include <vector>
#include <algorithm>

namespace Kale::OpenGL {

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
		 * The vertex data itself
		 */
		Buffer<float> vertices;

		/**
		 * The element buffer/index buffer
		 */
		Buffer<unsigned int> elements;

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
		 */
		template <typename Container> void condenseVertices(const Container& verts) {
			glGenVertexArrays(1, &vertexArray);
			bind();

			vertices.data.reserve(verts.size());
			elements.data.reserve(verts.size());

			size_t vertIndex = 0;

			for (const T& vert : verts) {

				// Check for existing vertex in memory
				bool existingVertFound = false;
				size_t existingVertIndex;

				// Reverse iteration as same vertices are more likely to be near eachother in the array
				for (size_t i = elements.data.size() - 1; i >= 0; i--) {

					// Check if the vert in memory/the vert are equal
					if (vertFloatPtrEqual(vertices.data[numFloatsInVert() * elements.data[i]], vert)) {
						existingVertIndex = elements.data[i];
						existingVertFound = true;
						break;
					}
				}

				// An existing vertex was found, just add it's index to the elements and continue
				if (existingVertFound) {
					elements.data.push_back(existingVertIndex);
					continue;
				}

				// No existing vertex was found, add both the vertex to memory and the index
				elements.data.push_back(vertIndex);
				vertIndex++;

				const float* vertLoc = reinterpret_cast<const float*>(&vert);
				vertices.data.insert(vertices.data.end(), vertLoc, vertLoc + numFloatsInVert());
			}

			vertices.updateBuffer();
			elements.updateBuffer();
			vertices.data.shrink_to_fit();
			elements.data.shrink_to_fit();
		}

		/**
		 * Creates the vertices and elements given the vertices container and a function to create the
		 * elements
		 * @param verts The vertices
		 * @param func The function for setting the elements
		 */
		template <typename Verts, typename Func> void createVerticesAndElements(const Verts& verts, const Func& func) {
			glGenVertexArrays(1, &vertexArray);
			bind();
			vertices.data.insert(vertices.data.begin(), reinterpret_cast<const float*>(verts.data()),
				reinterpret_cast<const float*>(verts.data() + verts.size()));
			func()
			vertices.updateBuffer();
			elements.updateBuffer();
		}

	public:
		
		/**
		 * Creates an empty vertex array
		 */
		VertexArray() : vertices(BufferType::VertexBuffer), elements(BufferType::ElementBuffer) {
			glGenVertexArrays(1, &vertexArray);
			bind();
		}
		
		/**
		 * Creates a vertex array given the vertices (condenses them into elements)
		 * @param verts The vertices
		 */
		template <size_t N>
		VertexArray(const std::array<T, N>& verts) : vertices(BufferType::VertexBuffer), elements(BufferType::ElementBuffer) {
			condenseVertices(verts);
		}
		
		/**
		 * Creates a vertex array given the vertices (condenses them into elements)
		 * @param verts The vertices
		 */
		VertexArray(const std::vector<T>& verts) : vertices(BufferType::VertexBuffer), elements(BufferType::ElementBuffer) {
			condenseVertices(verts);
		}
		
		/**
		 * Creates a vertex array given the vertices and elements/indices
		 * @param verts The vertices
		 * @param indices The indices
		 */
		template <size_t N1, size_t N2> VertexArray(const std::array<T, N1>& verts, const std::array<unsigned int, N2>& indices) :
			vertices(BufferType::VertexBuffer), elements(BufferType::ElementBuffer) {

			createVerticesAndElements(verts, [&]() -> void {
				elements.data.insert(elements.data.begin(), indices.begin(), indices.end());
			});
		}
		
		/**
		 * Creates a vertex array given the vertices and elements/indices
		 * @param verts The vertices
		 * @param indices The indices
		 */
		template <size_t N1, size_t N2> VertexArray(const std::array<T, N1>& verts, const std::array<size_t, N2>& indices) :
			vertices(BufferType::VertexBuffer), elements(BufferType::ElementBuffer) {

			createVerticesAndElements(verts, [&]() -> void {
				elements.reserve(indices.size());
				for (const size_t& i : indices) elements.data.push_back(static_cast<unsigned int>(i));
			});
		}
		
		/**
		 * Creates a vertex array given the vertices and elements/indices
		 * @param verts The vertices
		 * @param indices The indices
		 */
		VertexArray(const std::vector<T>& verts, const std::vector<unsigned int>& indices) :
			vertices(BufferType::VertexBuffer), elements(BufferType::ElementBuffer) {

			createVerticesAndElements(verts, [&]() -> void {
				elements.data.insert(elements.data.begin(), indices.begin(), indices.end());
			});
		}
		
		/**
		 * Creates a vertex array given the vertices and elements/indices
		 * @param verts The vertices
		 * @param indices The indices
		 */
		VertexArray(const std::vector<T>& verts, std::vector<unsigned int>&& indices) : vertices(BufferType::VertexBuffer),
			elements(BufferType::ElementBuffer) {

			createVerticesAndElements(verts, [&]() -> void {
				elements.data = std::move(indices);
			});
		}
		
		/**
		 * Creates a vertex array given the vertices and elements/indices
		 * @param verts The vertices
		 * @param indices The indices
		 */
		VertexArray(const std::vector<T>& verts, const std::vector<size_t>& indices) : vertices(BufferType::VertexBuffer),
			elements(BufferType::ElementBuffer) {

			createVerticesAndElements(verts, [&]() -> void {
				elements.reserve(indices.size());
				for (const size_t& i : indices) elements.data.push_back(static_cast<unsigned int>(i));
			});
		}

		/**
		 * Frees resources
		 */
		~VertexArray() {
			glDeleteVertexArrays(1, vertexArray);
		}

		/**
		 * Links the vertex array to the attributes of shaders. All shaders using this vertex array must use
		 * the correct attribute layouts.
		 * @param attributes An array of the attribute locations for each vertex component
		 */
		void enableAttributePointer(const std::array<unsigned int, sizeof...(NFloats)>& attributes) const {
			bind();
			const std::array<size_t, sizeof...(NFloats)> nFloatsArr = {NFloats...};
			size_t offset = 0;
			for (size_t i = 0; i < attributes.size(); i++) {
				glVertexAttribPointer(attributes[i], nFloatsArr[i], GL_FLOAT, GL_FALSE, sizeof(T),
					static_cast<GLVoid*>(offset * sizeof(float)));
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
		 * Modifies an individual vertex
		 * @param index The index of the vertex to modify
		 * @param elementIndex whether or not the index given refers to an element
		 * (this should be true if a constructor without indices was used in most cases)
		 * @param newVertex The vertex to set at the index
		 */
		void modifyVertex(size_t index, bool elementIndex, const T& newVertex) {
			if (elementIndex) index = elements.data[index];
			const float* newVertPtr = reinterpret_cast<const float*>(&newVertex);
			std::copy(newVertPtr, newVertPtr + numFloatsInVert(), &vertices.data[index]);
		}

		/**
		 * Modifies multiple vertices at once
		 * @param index The index of the vertex to modify
		 * @param elementIndex whether or not the index given refers to an element
		 * (this should be true if a constructor without indices was used in most cases)
		 * @param newVertices The new vertices to replace with
		 */
		template <size_t N> void modifyVertices(size_t index, bool elementIndex, const std::array<T, N>& newVertices) {
			if (elementIndex) {
				for (size_t i = 0; i < newVertices.size(); i++) {
					size_t index = elements.data[index + i];
					const float* newVertPtr = reinterpret_cast<const float*>(&newVertices[i]);
					std::copy(newVertPtr, newVertPtr + numFloatsInVert(), &vertices.data[index]);
				}
			}
			else {
				const float* newVertPtr = reinterpret_cast<const float*>(&newVertices[0]);
				std::copy(newVertPtr, newVertPtr + numFloatsInVert() * newVertices.size(), &vertices.data[index]);
			}
		}

		/**
		 * Modifies multiple vertices at once
		 * @param index The index of the vertex to modify
		 * @param elementIndex whether or not the index given refers to an element
		 * (this should be true if a constructor without indices was used in most cases)
		 * @param newVertices The new vertices to replace with
		 */
		void modifyVertices(size_t index, bool elementIndex, const std::vector<T>& newVertices) {
			if (elementIndex) {
				for (size_t i = 0; i < newVertices.size(); i++) {
					size_t index = elements.data[index + i];
					const float* newVertPtr = reinterpret_cast<const float*>(&newVertices[i]);
					std::copy(newVertPtr, newVertPtr + numFloatsInVert(), &vertices.data[index]);
				}
			}
			else {
				const float* newVertPtr = reinterpret_cast<const float*>(&newVertices[0]);
				std::copy(newVertPtr, newVertPtr + numFloatsInVert() * newVertices.size(), &vertices.data[index]);
			}
		}

	};

}

#endif
