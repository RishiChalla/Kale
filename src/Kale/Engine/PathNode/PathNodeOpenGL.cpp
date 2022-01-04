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

#ifdef KALE_OPENGL

#include <Kale/Core/Application/Application.hpp>

#include "PathNode.hpp"

using namespace Kale;

/**
 * The shader program used for rendering path nodes
 */
std::unique_ptr<const OpenGL::Shader> PathNode::shader = nullptr;

/**
 * Renders the node
 * @param renderer The renderer to render to
 */
void PathNode::render() {
	shader->useProgram();
	shader->uniform(1, color);
	shader->uniform(2, zPosition);
	vertexArray.draw();
}

/**
 * Updates the node
 * @param threadNum the index of this thread, ranged 0 - numUpdateThreads
 * @param ups The amount of updates the current thread is doing in a second
 */
void PathNode::update(size_t threadNum, float ups) {
	// Do nothing
}

void PathNode::createShaders() {
	if (shader == nullptr) {
		const std::string folder = "." + mainApp->applicationName;
		const std::string vertShaderPath = folder + "/assets/shaders/shader.vert";
		const std::string fragShaderPath = folder + "/assets/shaders/shader.frag";
		shader = std::make_unique<const OpenGL::Shader>(vertShaderPath.c_str(), fragShaderPath.c_str());
	}
	
	vertexArray.enableAttributePointer({0});
}

/**
 * Creates an empty path node
 */
PathNode::PathNode() {
	createShaders();
}

/**
 * Creates a path node given the path
 * @param path The path
 */
PathNode::PathNode(const std::vector<Vector2f>& path) : vertexArray(path, OpenGL::BufferUsage::Static, true),
	color(1.0f, 1.0f, 1.0f, 0.0f) {
	createShaders();
}

/**
 * Creates a path node given the path and color
 * @param path The path
 * @param color The color
 */
PathNode::PathNode(const std::vector<Vector2f>& path, const Vector4f& color) : vertexArray(path, OpenGL::BufferUsage::Static, true),
	color(color) {
	createShaders();
}

/**
 * Updates the path to a new one
 * @param newPath the path to update to
 */
void PathNode::updatePath(const std::vector<Vector2f>& newPath) {
	std::tuple<std::vector<float>, std::vector<unsigned int>> bufferInfo = triangulatePathFloat(newPath);
	vertexArray.vertices.data.clear();
	vertexArray.vertices.data = std::move(std::get<0>(bufferInfo));
	vertexArray.vertices.updateBuffer(OpenGL::BufferUsage::Static);

	vertexArray.elements.data.clear();
	vertexArray.elements.data = std::move(std::get<1>(bufferInfo));
	vertexArray.elements.updateBuffer(OpenGL::BufferUsage::Static);
}

#endif
