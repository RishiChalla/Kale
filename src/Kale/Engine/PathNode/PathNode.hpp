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

#include <Kale/Math/Vector/Vector.hpp>
#include <Kale/Math/Rect/Rect.hpp>
#include <Kale/Engine/Node/Node.hpp>

#include "include/core/SkPath.h"

#include <vector>

namespace Kale {

	/**
	 * Represents a single cubic bezier
	 */
	struct CubicBezier {
		Vector2f controlPoint1, controlPoint2, destination;
	};

	/**
	 * Represents a single path of cubic bezier curves
	 */
	class PathNode : public Node {
	public:

		/**
		 * The origin of the path
		 */
		Vector2f origin;

		/**
		 * A vector of tuples of cubic beziers. Each tuple contains 2 control points and the destination point
		 */
		std::vector<CubicBezier> beziers;

		/**
		 * Creates an empty path with origin 0 0
		 */
		PathNode();

		/**
		 * Creates an empty path given an origin
		 * @param origin The origin of the path
		 */
		PathNode(Vector2f origin);

		/**
		 * Converts the path to a skia path
		 * @returns The skia path
		 */
		operator SkPath() const;

		/**
		 * Adds a cubic bezier to the path
		 * @param bezier The bezier curve to use
		 */
		void cubicBezierTo(CubicBezier bezier);

		/**
		 * Adds a cubic bezier to the path
		 * @param control1 The first control point
		 * @param control2 The second control point
		 * @param destination The destination
		 */
		void cubicBezierTo(Vector2f control1, Vector2f control2, Vector2f destination);

		/**
		 * Gets a bounding box for this geometry to check for quick and inaccurate collisions
		 * @return The bounding box
		 */
		Rect getBoundingBox() const override;

	};
}
