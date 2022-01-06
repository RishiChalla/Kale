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

#include "PathNode.hpp"

using namespace Kale;

/**
 * The shader program used for rendering path nodes
 */
std::unique_ptr<const OpenGL::Shader> PathNode::shader = nullptr;

/**
 * Renders the node
 * @param camera The camera to render with
 */
void PathNode::render(const Camera& camera) {
	shader->useProgram();

	Transform local;
	local.setTranslation(center * -1.0f);
	local.scale(scale);
	local.rotateDeg(rotation);
	local.translate(center);

	shader->uniform(localUniform, local);
	shader->uniform(cameraUniform, camera);
	shader->uniform(vertexColorUniform, color);
	shader->uniform(zPositionUniform, zPosition);
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
		cameraUniform = static_cast<unsigned int>(shader->getUniformLocation("camera"));
		localUniform = static_cast<unsigned int>(shader->getUniformLocation("local"));
		vertexColorUniform = static_cast<unsigned int>(shader->getUniformLocation("vertexColor"));
		zPositionUniform = static_cast<unsigned int>(shader->getUniformLocation("zPosition"));
		vertexPositionAttribute = static_cast<unsigned int>(shader->getAttributeLocation("vertexPosition"));
	}
	
	vertexArray.enableAttributePointer({vertexPositionAttribute});
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
PathNode::PathNode(const std::vector<Vector2f>& path) : color(1.0f, 1.0f, 1.0f, 0.0f) {
	createShaders();
	createPath(path);
}

/**
 * Creates a path node given the path and color
 * @param path The path
 * @param color The color
 */
PathNode::PathNode(const std::vector<Vector2f>& path, const Vector4f& color) : color(color) {
	createShaders();
	createPath(path);
}

/**
 * Updates the path to a new one
 * @param newPath the path to update to
 */
void PathNode::updatePath(const std::vector<Vector2f>& newPath) {
	createPath(newPath);
}

#endif
