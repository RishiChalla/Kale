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
	const std::string vertShaderPath = mainApp->getAssetFolderPath() + "shaders/PathNode.vert";
	const std::string fragShaderPath = mainApp->getAssetFolderPath() + "shaders/PathNode.frag";

	// Load the shaders
	shader = std::make_unique<const OpenGL::Shader>(vertShaderPath.c_str(), fragShaderPath.c_str());

	// Get the uniform locations
	cameraUniform = static_cast<unsigned int>(shader->getUniformLocation("camera"));
	localUniform = static_cast<unsigned int>(shader->getUniformLocation("local"));
	vertexColorUniform = static_cast<unsigned int>(shader->getUniformLocation("vertexColor"));
	strokeColorUniform = static_cast<unsigned int>(shader->getUniformLocation("strokeColor"));
	zPositionUniform = static_cast<unsigned int>(shader->getUniformLocation("zPosition"));
	beziersUniform = static_cast<unsigned int>(shader->getUniformLocation("beziers"));
	numBeziersUniform = static_cast<unsigned int>(shader->getUniformLocation("numBeziers"));
	fillUniform = static_cast<unsigned int>(shader->getUniformLocation("fill"));
	strokeUniform = static_cast<unsigned int>(shader->getUniformLocation("stroke"));
	strokeRadiusUniform = static_cast<unsigned int>(shader->getUniformLocation("strokeRadius"));

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
 * Updates the bounding box accounting for stroke
 */
void PathNode::updateBoundingBox() {
	Collidable::boundingBox = path.getBoundingBox();
	
	if (stroke == StrokeStyle::Outside || stroke == StrokeStyle::Both) {
		Collidable::boundingBox.topLeft -= strokeRadius;
		Collidable::boundingBox.bottomRight += strokeRadius;
	}

	const std::array<Vector2f, 4> verts = {
		Collidable::boundingBox.bottomLeft(),
		Collidable::boundingBox.topLeft,
		Collidable::boundingBox.bottomRight,
		Collidable::boundingBox.topRight()
	};

	std::copy(reinterpret_cast<const float*>(verts.cbegin()), reinterpret_cast<const float*>(verts.end()), vertexArray->vertices.data.begin());
	
	// OpenGL commands must be run on the main thread - add a task for it. 
	mainApp->runTaskOnMainThread([&]() { vertexArray->vertices.updateBuffer(); });
}

/**
 * Called when the node is added to the scene, guaranteed to be called before any updates & renders
 * and from the main thread.
 * @param scene The scene the node has been added to
 */
void PathNode::begin(const Scene& scene) {
	Collidable::boundingBox = path.getBoundingBox();
	
	if (stroke == StrokeStyle::Outside || stroke == StrokeStyle::Both) {
		Collidable::boundingBox.topLeft -= strokeRadius;
		Collidable::boundingBox.bottomRight += strokeRadius;
	}

	const std::array<Vector2f, 4> verts = {
		Collidable::boundingBox.bottomLeft(),
		Collidable::boundingBox.topLeft,
		Collidable::boundingBox.bottomRight,
		Collidable::boundingBox.topRight()
	};

	const std::array<unsigned int, 6> indices = {0, 1, 2, 1, 3, 2};

	OpenGL::BufferUsage usage = pathFSM.has_value() ? OpenGL::BufferUsage::Dynamic : OpenGL::BufferUsage::Static;
	vertexArray = std::make_unique<OpenGL::VertexArray<Vector2f, 2>>(verts, indices, usage);
	vertexArray->enableAttributePointer({posAttribute});
}

/**
 * Called prior to update, perfect place to do things such as updating the bounding box, etc
 * @param threadNum the index of the thread this update is called on
 * @param scene The scene being updated to
 * @param deltaTime The duration of the last frame in microseconds
 */
void PathNode::preUpdate(size_t threadNum, const Scene& scene, float deltaTime) {
	// Call transformable update
	Transformable::updateTransform(deltaTime);

	// Update the Path based on the FSM if applicable
	if (pathFSM.has_value()) {
		
		// Update the state
		pathFSM->updateState(deltaTime);

		// Clear the path
		std::fill(path.beziers.begin(), path.beziers.end(), CubicBezier{Vector2f::zero(), Vector2f::zero(), Vector2f::zero(), Vector2f::zero()});

		// Loop through the composition & lerp between states as applicable
		for (std::pair<int, float> composition : pathFSM->getStateComposition<int>()) {
			path += pathFSM->getStructure<int>(composition.first) * composition.second;
		}

		// Update the bounding box
		updateBoundingBox();
	}
}

/**
 * Renders the node
 * @param camera The camera to render with
 */
void PathNode::render(const Camera& camera, float deltaTime) const {
	// There is no vertex array setup - nothing to render
	if (vertexArray == nullptr) return;

	// Use the shader & provide uniforms
	shader->useProgram();
	shader->uniform(cameraUniform, camera);
	shader->uniform(localUniform, getFullTransform()); 
	shader->uniform(vertexColorUniform, color);
	shader->uniform(strokeColorUniform, strokeColor);
	shader->uniform(zPosition, zPosition);
	shader->uniform(fillUniform, fill ? 1 : 0);
	shader->uniform(strokeUniform, static_cast<int>(stroke));
	shader->uniform(strokeRadiusUniform, strokeRadius);

	shader->uniform(beziersUniform, reinterpret_cast<const Vector2f*>(path.beziers.data()), path.beziers.size() * 4); 
	shader->uniform(numBeziersUniform, static_cast<int>(path.beziers.size()));

	// Draw, fragment shaders will do the rest of the work for us
	vertexArray->draw();
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
PathNode::PathNode(const Path& path, bool fill, StrokeStyle stroke) : path(path), fill(fill), stroke(stroke) {
	// Empty Body
}

#endif
