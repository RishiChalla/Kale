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

#include <algorithm>

using namespace Kale;

/**
 * Called when the node is added to the scene, guaranteed to be called before any updates & renders
 * and from the main thread.
 * @param scene The scene the node has been added to
 */
void PathNode::begin(const Scene& scene) {

	// To render a bezier curve based path we will:
	// - Loop through all of the beziers, construct a quad using the four control points of the bezier
	// - Triangulate those quads and render them with a shader which uses the bezier as an attribute to shade in the correct regions
	// - Triangulate the path constructed from the interiors of the beziers, and render those

	std::vector<InnerVertex> innerVerts;
	std::vector<OuterVertex> outerVerts;
	std::vector<unsigned int> outerIndices;

	std::vector<Vector2f> innerPath;

	for (const CubicBezier& bezier : path.beziers) {
		std::array<Vector2f, 4> quad = {bezier.start, bezier.controlPoint1, bezier.controlPoint2, bezier.end};
		Vector2f center = (quad[0] + quad[1] + quad[2] + quad[3]) / 4.0f;
		std::sort(quad.begin(), quad.end(), [=](Vector2f a, Vector2f b) -> bool { return (a - center).cross(b - center) > 0.0f; });
		// outerVerts.push_back(OuterVertex{bezier.start, bezier, 0.0f, 0.0f, 0.0f});
	}

	std::tuple<std::vector<Vector2f>, std::vector<unsigned int>> triangulatedInnerPath = triangulatePath(innerPath);

	// outerTriangles = std::make_unique<OpenGL::VertexArray<OuterVertex, 2, 2, 2, 2, 2, 1, 1, 1>>();
	// innerTriangles = std::make_unique<OpenGL::VertexArray<InnerVertex, 2, 1, 1, 1>>();
}

/**
 * Renders the node
 * @param camera The camera to render with
 */
void PathNode::render(const Camera& camera, float deltaTime) const {
	if (innerTriangles == nullptr || outerTriangles == nullptr) return;
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
