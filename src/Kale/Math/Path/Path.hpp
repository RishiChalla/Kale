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

#include <Kale/Math/Vector/Vector.hpp>

#include <core/SkPath.h>

#include <vector>

namespace Kale {

	/**
	 * Forward declaration of Circle struct
	 */
	struct Circle;

	/**
	 * Forward declaration of Ray struct
	 */
	struct Ray;

	/**
	 * Forward declaration of Rect struct
	 */
	struct Rect;

	/**
	 * Forward declaration of Line struct
	 */
	struct Line;

	/**
	 * Represents a single cubic bezier
	 */
	struct CubicBezier {
		Vector2f controlPoint1, controlPoint2, destination;
	};

	/**
	 * Represents a single path of cubic bezier curves
	 */
	struct Path {

		/**
		 * The origin of the path
		 */
		Vector2f origin;

		/**
		 * A vector of tuples of cubic beziers. Each tuple contains 2 control points and the destination point
		 */
		std::vector<CubicBezier> beziers;

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
		 * Checks for collision with a point
		 * @param point The point to check collision for
		 * @returns True if there is a collision, false for no collision
		 */
		bool pointCollision(Vector2f point) const;

		/**
		 * Checks for collision with a rectangle
		 * @param rect The rectangle to check collision for
		 * @returns True if there is a collision, false for no collision
		 */
		bool rectCollision(Rect rect) const;

		/**
		 * Checks for collision with a circle
		 * @param circle The circle to check collision for
		 * @returns True if there is a collision, false for no collision
		 */
		bool circleCollision(Circle circle) const;

		/**
		 * Checks for collision with a ray
		 * @param ray The ray to check collision for
		 * @returns True if there is a collision, false for no collision
		 */
		bool rayCollision(Ray ray) const;

		/**
		 * Checks for collision with a path
		 * @param path The path to check collision for
		 * @returns True if there is a collision, false for no collision
		 */
		bool pathCollision(Path path) const;

		/**
		 * Checks for collision with a line
		 * @param line The line to check collision for
		 * @returns True if there is a collision, false for no collision
		 */
		bool lineCollision(Line line) const;

	};
}
