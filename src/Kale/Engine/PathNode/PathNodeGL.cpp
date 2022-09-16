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

	// Get the attribute locations
	posAttribute = static_cast<unsigned int>(shader->getAttributeLocation("pos"));
	bezierAttribute = static_cast<unsigned int>(shader->getAttributeLocation("bezier"));
	// skeletonTransformWeightAttribute = static_cast<unsigned int>(shader->getAttributeLocation("skeletonTransformWeight"));
	// skeletonTransform1Attribute = static_cast<unsigned int>(shader->getAttributeLocation("skeletonTransform1"));
	// skeletonTransform2Attribute = static_cast<unsigned int>(shader->getAttributeLocation("skeletonTransform2"));
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
	// To render a bezier curve based path we will:
	// - Loop through all of the beziers, construct a quad using the four control points of the bezier
	// - Triangulate those quads and render them with a shader which uses the bezier as an uniform to shade in the correct regions
	// - Triangulate the path constructed from the interiors of the beziers, and render those

	// Final verts/indices
	std::vector<Vertex> verts;
	std::vector<unsigned int> indices;

	// The inner path which will be triangulated after a loop
	std::vector<Vector2f> innerPath;

	// Loop through all the beziers
	for (size_t i = 0; i < path.beziers.size(); i++) {
		const CubicBezier& bezier = path.beziers[i];

		// Create a quad from the four control points
		std::array<Vector2f, 4> quad = {bezier.start, bezier.controlPoint1, bezier.controlPoint2, bezier.end};

		// Construct the quad correctly by sorting in clockwise order
		Vector2f center = (quad[0] + quad[1] + quad[2] + quad[3]) / 4.0f;
		std::sort(quad.begin(), quad.end(), [=](Vector2f a, Vector2f b) -> bool { return (a - center).cross(b - center) > 0.0f; });

		// Triangulate the quad and add it to our vertices/indices
		std::pair<std::vector<Vector2f>, std::vector<unsigned int>> triangulatedQuad;
		try {
			triangulatedQuad = triangulatePath(quad.cbegin(), quad.cend());
		}
		catch (const std::exception& e) {
			klPrint(bezier);
			// This bezier forms a line of some sort, triangulation isn't strictly necessary but we'll do it artificially so
			// there's more vertices to work with in the event of skeletal rigging
		}
		for (const Vector2f& vert : triangulatedQuad.first) verts.push_back(Vertex{vert, static_cast<float>(i), -1.0f, -1.0f, -1.0f});
		indices.insert(indices.end(), triangulatedQuad.second.begin(), triangulatedQuad.second.end());

		// Add the correct vertices to the inner path for triangulation later
		innerPath.push_back(bezier.start);
		if ((bezier.end - bezier.start).cross(bezier.controlPoint1 - bezier.start) > 0.0f) innerPath.push_back(bezier.controlPoint1);
		if ((bezier.end - bezier.start).cross(bezier.controlPoint2 - bezier.start) > 0.0f) innerPath.push_back(bezier.controlPoint2);
	}

	// Triangulate the inner path and add it to the verts/indices
	std::pair<std::vector<Vector2f>, std::vector<unsigned int>> triangulatedInnerPath = triangulatePath(&innerPath[0], &innerPath[0] + innerPath.size());
	for (const Vector2f& vert : triangulatedInnerPath.first) verts.push_back(Vertex{vert, -1.0f, -1.0f, -1.0f, -1.0f});
	indices.insert(indices.end(), triangulatedInnerPath.second.begin(), triangulatedInnerPath.second.end());

	// Create a vertex array from our vertices and indices
	vertexArray = std::make_unique<OpenGL::VertexArray<Vertex, 2, 1>>(verts, indices, OpenGL::BufferUsage::Static);
	vertexArray->enableAttributePointer({posAttribute, bezierAttribute});
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
