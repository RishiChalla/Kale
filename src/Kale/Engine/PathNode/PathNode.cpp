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

#include "PathNode.hpp"

#include <stdexcept>

using namespace Kale;

/**
 * Creates an empty path with origin 0 0
 */
PathNode::PathNode() : origin(0, 0), beziers() {
	// Empty Body
}

/**
 * Creates an empty path given an origin
 * @param origin The origin of the path
 */
PathNode::PathNode(Vector2f origin) : origin(origin) {
	// Empty Body
}

/**
 * Converts the path to a skia path
 * @returns The skia path
 */
PathNode::operator SkPath() const {
	SkPath path;
	path.moveTo(origin.x, origin.y);
	for (const CubicBezier& bezier : beziers)
		path.cubicTo(bezier.controlPoint1.x, bezier.controlPoint1.y, bezier.controlPoint2.x, bezier.controlPoint2.y, bezier.destination.x,
			bezier.destination.y);
	return path;
}

/**
 * Adds a cubic bezier to the path
 * @param bezier The bezier curve to use
 */
void PathNode::cubicBezierTo(CubicBezier bezier) {
	beziers.push_back(bezier);
}

/**
 * Adds a cubic bezier to the path
 * @param control1 The first control point
 * @param control2 The second control point
 * @param destination The destination
 */
void PathNode::cubicBezierTo(Vector2f control1, Vector2f control2, Vector2f destination) {
	beziers.push_back({control1, control2, destination});
}

/**
 * Gets a bounding box for this geometry to check for quick and inaccurate collisions
 * @return The bounding box
 */
Rect PathNode::getBoundingBox() const {
	throw std::runtime_error("Unimplemented method");
}
