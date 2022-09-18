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

#include "PathNode.hpp"

#include <Kale/Engine/Utils/Utils.hpp>
#include <Kale/Core/Application/Application.hpp>

#include <algorithm>
#include <array>

using namespace Kale;

/**
 * Creates and compiles shaders if not already compiled/created
 */
void PathNode::setup() {
	if (shader != nullptr) return;

	// Get shader paths
	const std::string folder = "." + mainApp->applicationName;
	const std::string vertShaderPath = folder + "/assets/shaders/PathNode.vert";
	const std::string fragShaderPath = folder + "/assets/shaders/PathNode.frag";

	// Load the shaders
	shader = std::make_unique<const OpenGL::Shader>(vertShaderPath.c_str(), fragShaderPath.c_str());

	// Get the uniform locations
	cameraUniform = static_cast<unsigned int>(shader->getUniformLocation("camera"));
	localUniform = static_cast<unsigned int>(shader->getUniformLocation("local"));
	vertexColorUniform = static_cast<unsigned int>(shader->getUniformLocation("vertexColor"));
	zPositionUniform = static_cast<unsigned int>(shader->getUniformLocation("zPosition"));
	beziersUniform = static_cast<unsigned int>(shader->getUniformLocation("beziers"));
	numBeziersUniform = static_cast<unsigned int>(shader->getUniformLocation("numBeziers"));

	// Get the attribute locations
	posAttribute = static_cast<unsigned int>(shader->getAttributeLocation("pos"));
}

/**
 * Deletes shaders/cleans up
 */
void PathNode::cleanup() {
	shader.reset();
}

/**
 * Called when the node is added to the scene, guaranteed to be called before any updates & renders
 * and from the main thread.
 * @param scene The scene the node has been added to
 */
void PathNode::begin(const Scene& scene) {
	Rect boundingBox = path.getBoundingBox();

	const std::array<Vector2f, 4> verts = {boundingBox.bottomLeft(), boundingBox.topLeft, boundingBox.bottomRight, boundingBox.topRight()};
	const std::array<unsigned int, 6> indices = {0, 1, 2, 1, 3, 2};

	vertexArray = std::make_unique<OpenGL::VertexArray<Vector2f, 2>>(verts, indices, OpenGL::BufferUsage::Static);
	vertexArray->enableAttributePointer({posAttribute});
}

/**
 * Renders the node
 * @param camera The camera to render with
 */
void PathNode::render(const Camera& camera, float deltaTime) const {
	if (vertexArray == nullptr) return;
	shader->useProgram();
	shader->uniform(cameraUniform, camera);
	shader->uniform(localUniform, transform);
	shader->uniform(vertexColorUniform, color);
	shader->uniform(zPosition, zPosition);

	shader->uniform(beziersUniform, reinterpret_cast<const Vector2f*>(path.beziers.data()), path.beziers.size() * 4); 
	shader->uniform(numBeziersUniform, static_cast<int>(path.beziers.size()));

	vertexArray->draw();
}

/**
 * Called when the node is removed from the scene, guaranteed to be called from the main thread
 */
void PathNode::end(const Scene& scene) {
	
}

/**
 * Creates a blank pathnode with nothing to render
 */
PathNode::PathNode() {
	// Empty Body
}

/**
 * Creates a path node given the path to use
 * @param path The path to use
 */
PathNode::PathNode(const Path& path) : path(path) {
	// Empty Body
}

#endif
