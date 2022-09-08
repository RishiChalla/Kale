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

#include <Kale/Math/Math.hpp>

namespace Kale::OpenGL {

	/**
	 * Represents a single shader for use by nodes
	 */
	class Shader {
	private:

		/**
		 * The OpenGL id of the shader program
		 */
		unsigned int program;

		/**
		 * Creates a shader, compiles it, and throws if unable to compile.
		 * @param type The type of shader
		 * @param filePath the path to the source of the shader
		 * @returns The shader
		 * @throws If unable to compile
		 */
		unsigned int createShader(unsigned int type, const char* filePath);

	public:

		/**
		 * Creates, loads, and compiles a new shader program.
		 * @param vertShaderFile The file path of the vertex shader source
		 * @param fragShaderFile The file path of the fragment shader source
		 * @throws If unable to compile
		 */
		Shader(const char* vertShaderFile, const char* fragShaderFile);

		/**
		 * Shaders do not support copying
		 */
		Shader(const Shader& other) = delete;

		/**
		 * Shaders do not support copying
		 */
		void operator=(const Shader& other) = delete;

		/**
		 * Frees resources
		 */
		~Shader();

		/**
		 * Uses this shader program for rendering
		 */
		void useProgram() const;

		/**
		 * Gets the location of an attribute
		 * @param name The name of the attribute
		 * @returns The location of the attribute
		 */
		int getAttributeLocation(const char* name) const;

		/**
		 * Gets the location of an uniform
		 * @param name The name of the uniform
		 * @returns The location of the uniform
		 */
		int getUniformLocation(const char* name) const;

		/**
		 * Passes a uniform at a certain location to the shader
		 * @param location The location of the uniform
		 * @param value The value of the uniform
		 */
		void uniform(unsigned int location, const Vector2f& value) const;

		/**
		 * Passes a uniform at a certain location to the shader
		 * @param location The location of the uniform
		 * @param value The value of the uniform
		 */
		void uniform(unsigned int location, const Vector3f& value) const;

		/**
		 * Passes a uniform at a certain location to the shader
		 * @param location The location of the uniform
		 * @param value The value of the uniform
		 */
		void uniform(unsigned int location, const Vector4f& value) const;

		/**
		 * Passes a uniform at a certain location to the shader
		 * @param location The location of the uniform
		 * @param value The value of the uniform
		 */
		void uniform(unsigned int location, const Matrix2f& value) const;

		/**
		 * Passes a uniform at a certain location to the shader
		 * @param location The location of the uniform
		 * @param value The value of the uniform
		 */
		void uniform(unsigned int location, const Matrix3f& value) const;

		/**
		 * Passes a uniform at a certain location to the shader
		 * @param location The location of the uniform
		 * @param value The value of the uniform
		 */
		void uniform(unsigned int location, const Matrix4f& value) const;

		/**
		 * Passes a uniform at a certain location to the shader
		 * @param location The location of the uniform
		 * @param value The value of the uniform
		 */
		void uniform(unsigned int location, const Transform& value) const;

		/**
		 * Passes a uniform at a certain location to the shader
		 * @param location The location of the uniform
		 * @param value The value of the uniform
		 */
		void uniform(unsigned int location, float value) const;

		/**
		 * Passes a uniform at a certain location to the shader
		 * @param location The location of the uniform
		 * @param value The value of the uniform
		 */
		void uniform(unsigned int location, const std::vector<Vector2f>& value) const;

		/**
		 * Passes a uniform at a certain location to the shader
		 * @param location The location of the uniform
		 * @param value The value of the uniform
		 */
		void uniform(unsigned int location, const std::vector<Vector3f>& value) const;

		/**
		 * Passes a uniform at a certain location to the shader
		 * @param location The location of the uniform
		 * @param value The value of the uniform
		 */
		void uniform(unsigned int location, const std::vector<Vector4f>& value) const;

		/**
		 * Passes a uniform at a certain location to the shader
		 * @param location The location of the uniform
		 * @param value The value of the uniform
		 */
		void uniform(unsigned int location, const std::vector<Matrix2f>& value) const;

		/**
		 * Passes a uniform at a certain location to the shader
		 * @param location The location of the uniform
		 * @param value The value of the uniform
		 */
		void uniform(unsigned int location, const std::vector<Matrix3f>& value) const;

		/**
		 * Passes a uniform at a certain location to the shader
		 * @param location The location of the uniform
		 * @param value The value of the uniform
		 */
		void uniform(unsigned int location, const std::vector<Matrix4f>& value) const;

		/**
		 * Passes a uniform at a certain location to the shader
		 * @param location The location of the uniform
		 * @param value The value of the uniform
		 */
		void uniform(unsigned int location, const std::vector<Transform>& value) const;

		/**
		 * Passes a uniform at a certain location to the shader
		 * @param location The location of the uniform
		 * @param value The value of the uniform
		 */
		void uniform(unsigned int location, const std::vector<float>& value) const;

	};
}

#endif
