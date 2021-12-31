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

#include <Kale/OpenGL/Utils/Utils.hpp>

#include <vector>
#include <array>
#include <algorithm>

#include <glad/glad.h>

namespace Kale::OpenGL {

	/**
	 * The type of buffer a buffer can hold
	 */
	enum class BufferType : GLenum {
		ElementBuffer = GL_ELEMENT_ARRAY_BUFFER,
		VertexBuffer = GL_ARRAY_BUFFER,
		TextureBuffer = GL_TEXTURE_BUFFER
	};

	/**
	 * The usage of the buffer, static isn't modified frequently whereas dynamic is
	 */
	enum class BufferUsage : GLenum {
		Static = GL_STATIC_DRAW,
		Dynamic = GL_DYNAMIC_DRAW
	};

	/**
	 * Represents a block of data on the GPU
	 */
	template <typename T>
	class Buffer {
	private:

		/**
		 * The location of the buffer for opengl accessing
		 */
		unsigned int buffer;

		/**
		 * The type of buffer this is
		 */
		BufferType type;
		
	public:

		/**
		 * The data this buffer holds
		 */
		std::vector<T> data;

		/**
		 * Updates the buffer on the GPU to the data within the vector
		 * @param usage The usage of the buffer
		 */
		void updateBuffer(BufferUsage usage) {
			bind();
			glBufferData(getEnumValue(type), sizeof(T) * data.size(), data.data(), getEnumValue(usage));
		}

		/**
		 * Creates an empty buffer
		 * @param type The type of buffer
		 */
		Buffer(BufferType type) : type(type) {
			glGenBuffers(1, &buffer);
			bind();
		}

		/**
		 * Creates a buffer from an array
		 * @param type The type of buffer
		 * @param usage The usage of the buffer
		 * @param data The data to create the buffer from
		 */
		template <size_t N> Buffer(BufferType type, BufferUsage usage, const std::array<T, N>& data) : type(type),
			data(data.begin(), data.end()) {
			glGenBuffers(1, &buffer);
			updateBuffer(usage);
		}

		/**
		 * Creates a buffer from a vector
		 * @param type The type of buffer
		 * @param usage The usage of the buffer
		 * @param dat The data to create the buffer from
		 */
		Buffer(BufferType type, BufferUsage usage, const std::vector<T>& dat) : type(type), data(dat) {
			glGenBuffers(1, &buffer);
			updateBuffer(usage);
		}

		/**
		 * Creates a buffer from a vector
		 * @param type The type of buffer
		 * @param usage The usage of the buffer
		 * @param dat The data to create the buffer from
		 */
		Buffer(BufferType type, BufferUsage usage, std::vector<T>&& dat) : type(type), data(std::move(dat)) {
			glGenBuffers(1, &buffer);
			updateBuffer(usage);
		}

		/**
		 * Creates a buffer from a C array
		 * @param type The type of buffer
		 * @param usage The usage of the buffer
		 * @param arr The data to create the buffer from
		 * @param n The length of the array
		 */
		Buffer(BufferType type, BufferUsage usage, const T* arr, size_t n) : type(type), data(arr, arr + n) {
			glGenBuffers(1, &buffer);
			updateBuffer(usage);
		}

		/**
		 * Buffers do not support copying
		 */
		Buffer(const Buffer& other) = delete;

		/**
		 * Buffers do not support copying
		 */
		void operator=(const Buffer& other) = delete;

		/**
		 * Destroys the buffer and frees resources from the GPU
		 */
		~Buffer() {
			glDeleteBuffers(1, &buffer);
		}

		/**
		 * Returns the length of the data
		 */
		[[nodiscard]] size_t size() const {
			return data.size();
		}

		/**
		 * Retrieves the value in the buffer at a certain index
		 * @param i The index
		 * @returns The value within the buffer
		 */
		[[nodiscard]] const T& operator[](size_t i) const {
			return data[i];
		}

		/**
		 * Binds the buffer for use directly with opengl commands
		 */
		void bind() const {
			glBindBuffer(getEnumValue<BufferType>(type), buffer);
		}

		/**
		 * modifies the buffer
		 * @param i The index to begin modifying at
		 * @param val The data to modify and replace with
		 */
		template <size_t N> void modify(size_t i, const std::array<T, N>& val) {
			std::copy(data.begin() + i, data.begin() + i + N, val.data());
			bind();
			glBufferSubData(getEnumValue<BufferType>(type), sizeof(T) * i, sizeof(T) * N, data.data());
		}

		/**
		 * modifies the buffer
		 * @param i The index to begin modifying at
		 * @param val The data to modify and replace with
		 */
		void modify(size_t i, const std::vector<T>& val) {
			std::copy(data.begin() + i, data.begin() + i + val.size(), val.data());
			bind();
			glBufferSubData(getEnumValue<BufferType>(type), sizeof(T) * i, sizeof(T) * val.size(), data.data());
		}

		/**
		 * modifies the buffer
		 * @param i The index to begin modifying at
		 * @param arr The data to modify and replace with
		 * @param n The length of the array/val data
		 */
		void modify(size_t i, const T* arr, size_t n) {
			std::copy(data.begin() + i, data.begin() + i + n, arr);
			bind();
			glBufferSubData(getEnumValue<BufferType>(type), sizeof(T) * i, sizeof(T) * val.size(), data.data());
		}

		/**
		 * Modifies the buffer
		 * @param i The index to modify
		 * @param val The data to replace the existing data at the index with
		 */
		void modify(size_t i, T val) {
			data[i] = val;
			bind();
			glBufferSubData(getEnumValue<BufferType>(type), sizeof(T) * i, sizeof(T) * val.size(), data.data());
		}

		/**
		 * Resizes the buffer to a given new size
		 * @param usage The usage for the buffer
		 * @param newSize the new size of the buffer
		 */
		void resize(BufferUsage usage, size_t newSize) {
			data.resize(newSize);
			updateBuffer(usage);
		}

		/**
		 * Resizes the buffer to a given array and replaces the data
		 * @param usage The usage for the buffer
		 * @param arr the array to resize to and replace with
		 */
		template <size_t N> void resize(BufferUsage usage, const std::array<T, N>& arr) {
			data.resize(N);
			std::copy(data.begin(), data.end(), arr.data());
			updateBuffer(usage);
		}

		/**
		 * Resizes the buffer to a given vector and replaces the data
		 * @param usage The usage for the buffer
		 * @param vec the vector to resize to and replace with
		 */
		void resize(BufferUsage usage, const std::vector<T>& vec) {
			data = vec;
			updateBuffer(usage);
		}

		/**
		 * Resizes the buffer to a given vector and replaces the data
		 * @param usage The usage for the buffer
		 * @param vec the vector to resize to and replace with
		 */
		void resize(BufferUsage usage, std::vector<T>&& vec) {
			data = std::move(vec);
			updateBuffer(usage);
		}

		/**
		 * Resizes the buffer to a given c array and replaces the data
		 * @param usage The usage for the buffer
		 * @param arr the c array to resize to and replace with
		 * @param n The length of the c array
		 */
		void resize(BufferUsage usage, const T* arr, size_t n) {
			data.clear();
			data.insert(data.begin(), arr, arr + n);
			updateBuffer(usage);
		}

	};
}

#endif
