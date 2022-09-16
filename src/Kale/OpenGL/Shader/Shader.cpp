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

#ifdef KALE_OPENGL

#include "Shader.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <memory>

#include <glad/glad.h>

using namespace Kale;
using namespace Kale::OpenGL;

/**
 * Creates a shader, compiles it, and throws if unable to compile.
 * @param type The type of shader
 * @param filePath the path to the source of the shader
 * @returns The shader
 * @throws If unable to compile
 */
unsigned int Shader::createShader(unsigned int type, const char* filePath) {
	using namespace std::string_literals;

	unsigned int shader = glCreateShader(type);

	// Read in the file source
	std::string src;
	{
		std::ifstream file(filePath);
		std::ostringstream stream;
		stream << file.rdbuf();
		src = stream.str();
	}

	// Pass the file source to opengl
	const char* cStrSrc = src.c_str();
	int strLen = static_cast<int>(src.size());
	glShaderSource(shader, 1, &cStrSrc, &strLen);

	// compile the shader and deal with errors
	glCompileShader(shader);
	int successful;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &successful);

	// deal with errors
	if (!successful) {
		int logLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		std::unique_ptr<char*> infoLog = std::make_unique<char*>(new char[logLen]);
		glGetShaderInfoLog(shader, logLen, nullptr, *infoLog.get());
		std::string strInfoLog(*infoLog.get(), logLen);
		glDeleteShader(shader);
		throw std::runtime_error("Unable to compile shader ("s + filePath + ") - \n" + strInfoLog);
	}

	return shader;
}

/**
 * Creates, loads, and compiles a new shader program.
 * @param vertShaderFile The file path of the vertex shader source
 * @param fragShaderFile The file path of the fragment shader source
 * @throws if unable to compile
 */
Shader::Shader(const char* vertShaderFile, const char* fragShaderFile) {

	// Create the shaders
	unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertShaderFile);
	unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragShaderFile);

	// Create the program and link it with the shaders
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	// Check if linking was successful
	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	// Deal with errors
	if (!success) {
		int logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		std::unique_ptr<char*> infoLog = std::make_unique<char*>(new char[logLen]);
		glGetProgramInfoLog(program, logLen, nullptr, *infoLog.get());
		std::string strInfoLog(*infoLog.get(), logLen);

		glDeleteProgram(program);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		throw std::runtime_error("Unable to link shaders to program - " + strInfoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glUseProgram(program);
}

/**
 * Frees resources
 */
Shader::~Shader() {
	glDeleteProgram(program);
}

/**
 * Uses this shader program for rendering
 */
void Shader::useProgram() const {
	glUseProgram(program);
}

/**
 * Gets the location of an attribute
 * @param name The name of the attribute
 * @returns The location of the attribute
 */
int Shader::getAttributeLocation(const char* name) const {
	return glGetAttribLocation(program, name);
}

/**
 * Gets the location of an uniform
 * @param name The name of the uniform
 * @returns The location of the uniform
 */
int Shader::getUniformLocation(const char* name) const {
	return glGetUniformLocation(program, name);
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param value The value of the uniform
 */	
void Shader::uniform(unsigned int location, const Vector2f& value) const {
	useProgram();
	glUniform2f(location, value.x, value.y);
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param value The value of the uniform
 */	
void Shader::uniform(unsigned int location, const Vector3f& value) const {
	useProgram();
	glUniform3f(location, value.x, value.y, value.z);
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param value The value of the uniform
 */	
void Shader::uniform(unsigned int location, const Vector4f& value) const {
	useProgram();
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param value The value of the uniform
 */	
void Shader::uniform(unsigned int location, const Matrix2f& value) const {
	useProgram();
	glUniformMatrix2fv(location, 1, GL_FALSE, value.data.data());
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param value The value of the uniform
 */	
void Shader::uniform(unsigned int location, const Matrix3f& value) const {
	useProgram();
	glUniformMatrix3fv(location, 1, GL_FALSE, value.data.data());
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param value The value of the uniform
 */	
void Shader::uniform(unsigned int location, const Matrix4f& value) const {
	useProgram();
	glUniformMatrix4fv(location, 1, GL_FALSE, value.data.data());
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param value The value of the uniform
 */	
void Shader::uniform(unsigned int location, const Transform& value) const {
	useProgram();
	glUniformMatrix3fv(location, 1, GL_FALSE, value.data.data());
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param value The value of the uniform
 */	
void Shader::uniform(unsigned int location, float value) const {
	useProgram();
	glUniform1f(location, value);
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param value The value of the uniform
 */
void Shader::uniform(unsigned int location, const std::vector<Vector2f>& value) const {
	useProgram();
	glUniform2fv(location, static_cast<GLsizei>(value.size()), reinterpret_cast<const float*>(value.data()));
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param value The value of the uniform
 */
void Shader::uniform(unsigned int location, const std::vector<Vector3f>& value) const {
	useProgram();
	glUniform3fv(location, static_cast<GLsizei>(value.size()), reinterpret_cast<const float*>(value.data()));
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param value The value of the uniform
 */
void Shader::uniform(unsigned int location, const std::vector<Vector4f>& value) const {
	useProgram();
	glUniform4fv(location, static_cast<GLsizei>(value.size()), reinterpret_cast<const float*>(value.data()));
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param value The value of the uniform
 */
void Shader::uniform(unsigned int location, const std::vector<Matrix2f>& value) const {
	useProgram();
	glUniformMatrix2fv(location, static_cast<GLsizei>(value.size()), GL_FALSE, value[0].data.data());
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param value The value of the uniform
 */
void Shader::uniform(unsigned int location, const std::vector<Matrix3f>& value) const {
	useProgram();
	glUniformMatrix3fv(location, static_cast<GLsizei>(value.size()), GL_FALSE, value[0].data.data());
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param value The value of the uniform
 */
void Shader::uniform(unsigned int location, const std::vector<Matrix4f>& value) const {
	useProgram();
	glUniformMatrix4fv(location, static_cast<GLsizei>(value.size()), GL_FALSE, value[0].data.data());
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param value The value of the uniform
 */
void Shader::uniform(unsigned int location, const std::vector<Transform>& value) const {
	useProgram();
	glUniformMatrix3fv(location, static_cast<GLsizei>(value.size()), GL_FALSE, value[0].data.data());
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param value The value of the uniform
 */
void Shader::uniform(unsigned int location, const std::vector<float>& value) const {
	useProgram();
	glUniform1fv(location, static_cast<GLsizei>(value.size()), value.data());
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param ptr The beginning pointer of the uniform values
 * @param size The count of uniform values
 */
void Shader::uniform(unsigned int location, const Vector2f* ptr, size_t size) const {
	useProgram();
	glUniform2fv(location, static_cast<GLsizei>(size), reinterpret_cast<const float*>(ptr));
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param ptr The beginning pointer of the uniform values
 * @param size The count of uniform values
 */
void Shader::uniform(unsigned int location, const Vector3f* ptr, size_t size) const {
	useProgram();
	glUniform3fv(location, static_cast<GLsizei>(size), reinterpret_cast<const float*>(ptr));
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param ptr The beginning pointer of the uniform values
 * @param size The count of uniform values
 */
void Shader::uniform(unsigned int location, const Vector4f* ptr, size_t size) const {
	useProgram();
	glUniform4fv(location, static_cast<GLsizei>(size), reinterpret_cast<const float*>(ptr));
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param ptr The beginning pointer of the uniform values
 * @param size The count of uniform values
 */
void Shader::uniform(unsigned int location, const Matrix2f* ptr, size_t size) const {
	useProgram();
	glUniformMatrix2fv(location, static_cast<GLsizei>(size), GL_FALSE, ptr->data.data());
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param ptr The beginning pointer of the uniform values
 * @param size The count of uniform values
 */
void Shader::uniform(unsigned int location, const Matrix3f* ptr, size_t size) const {
	useProgram();
	glUniformMatrix3fv(location, static_cast<GLsizei>(size), GL_FALSE, ptr->data.data());
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param ptr The beginning pointer of the uniform values
 * @param size The count of uniform values
 */
void Shader::uniform(unsigned int location, const Matrix4f* ptr, size_t size) const {
	useProgram();
	glUniformMatrix4fv(location, static_cast<GLsizei>(size), GL_FALSE, ptr->data.data());
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param ptr The beginning pointer of the uniform values
 * @param size The count of uniform values
 */
void Shader::uniform(unsigned int location, const Transform* ptr, size_t size) const {
	useProgram();
	glUniformMatrix3fv(location, static_cast<GLsizei>(size), GL_FALSE, ptr->data.data());
}

/**
 * Passes a uniform at a certain location to the shader
 * @param location The location of the uniform
 * @param ptr The beginning pointer of the uniform values
 * @param size The count of uniform values
 */
void Shader::uniform(unsigned int location, const float* ptr, size_t size) const {
	useProgram();
	glUniform1fv(location, static_cast<GLsizei>(size), ptr);
}

#endif
