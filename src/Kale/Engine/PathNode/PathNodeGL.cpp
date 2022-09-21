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
	// Get shader paths
	const std::string folder = "." + mainApp->applicationName;
	const std::string vertShaderPath = folder + "/assets/shaders/PathNode.vert";
	const std::string fragFillShaderPath = folder + "/assets/shaders/PathNodeFill.frag";
	const std::string fragStrokeShaderPath = folder + "/assets/shaders/PathNodeStroke.frag";

	// Load the shaders
	fillShader = std::make_unique<const OpenGL::Shader>(vertShaderPath.c_str(), fragFillShaderPath.c_str());
	strokeShader = std::make_unique<const OpenGL::Shader>(vertShaderPath.c_str(), fragStrokeShaderPath.c_str());

	// Get the uniform locations
	fillCameraUniform = static_cast<unsigned int>(fillShader->getUniformLocation("camera"));
	fillLocalUniform = static_cast<unsigned int>(fillShader->getUniformLocation("local"));
	fillVertexColorUniform = static_cast<unsigned int>(fillShader->getUniformLocation("vertexColor"));
	fillZPositionUniform = static_cast<unsigned int>(fillShader->getUniformLocation("zPosition"));
	fillBeziersUniform = static_cast<unsigned int>(fillShader->getUniformLocation("beziers"));
	fillNumBeziersUniform = static_cast<unsigned int>(fillShader->getUniformLocation("numBeziers"));

	strokeCameraUniform = static_cast<unsigned int>(strokeShader->getUniformLocation("camera"));
	strokeLocalUniform = static_cast<unsigned int>(strokeShader->getUniformLocation("local"));
	strokeVertexColorUniform = static_cast<unsigned int>(strokeShader->getUniformLocation("vertexColor"));
	strokeZPositionUniform = static_cast<unsigned int>(strokeShader->getUniformLocation("zPosition"));
	strokeBeziersUniform = static_cast<unsigned int>(strokeShader->getUniformLocation("beziers"));
	strokeNumBeziersUniform = static_cast<unsigned int>(strokeShader->getUniformLocation("numBeziers"));
	strokeRadiusUniform = static_cast<unsigned int>(strokeShader->getUniformLocation("strokeRadius"));

	// Get the attribute locations
	fillPosAttribute = static_cast<unsigned int>(fillShader->getAttributeLocation("pos"));
	strokePosAttribute = static_cast<unsigned int>(strokeShader->getAttributeLocation("pos"));
}

/**
 * Deletes shaders/cleans up
 */
void PathNode::cleanup() {
	fillShader.reset();
	strokeShader.reset();
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
}

/**
 * Renders the node
 * @param camera The camera to render with
 */
void PathNode::render(const Camera& camera, float deltaTime) const {
	if (vertexArray == nullptr) return;

	if (fill) {
		fillShader->useProgram();
		fillShader->uniform(fillCameraUniform, camera);
		fillShader->uniform(fillLocalUniform, transform);
		fillShader->uniform(fillVertexColorUniform, color);
		fillShader->uniform(fillZPositionUniform, zPosition);

		fillShader->uniform(fillBeziersUniform, reinterpret_cast<const Vector2f*>(path.beziers.data()), path.beziers.size() * 4); 
		fillShader->uniform(fillNumBeziersUniform, static_cast<int>(path.beziers.size()));

		vertexArray->enableAttributePointer({fillPosAttribute});
		vertexArray->draw();
	}

	if (stroke) {
		strokeShader->useProgram();
		strokeShader->uniform(strokeCameraUniform, camera);
		strokeShader->uniform(strokeLocalUniform, transform);
		strokeShader->uniform(strokeVertexColorUniform, strokeColor);
		strokeShader->uniform(strokeZPositionUniform, zPosition);
		strokeShader->uniform(strokeRadiusUniform, strokeRadius);

		strokeShader->uniform(strokeBeziersUniform, reinterpret_cast<const Vector2f*>(path.beziers.data()), path.beziers.size() * 4); 
		strokeShader->uniform(strokeNumBeziersUniform, static_cast<int>(path.beziers.size()));

		vertexArray->enableAttributePointer({strokePosAttribute});
		vertexArray->draw();
	}
}

/**
 * Called when the node is removed from the scene, guaranteed to be called from the main thread
 */
void PathNode::end(const Scene& scene) {
	vertexArray.reset();
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
 * @param fill Whether or not to fill the node
 * @param stroke Whether or not to stroke the node
 */
PathNode::PathNode(const Path& path, bool fill, bool stroke) : path(path), fill(fill), stroke(stroke) {
	// Empty Body
}

#endif
