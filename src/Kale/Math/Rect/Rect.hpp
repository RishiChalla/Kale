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
	 * Forward declaration of Path struct
	 */
	struct Path;

	/**
	 * Forward declaration of Line struct
	 */
	struct Line;

	/**
	 * Represents a rectangular structure of float vectors
	 */
	struct Rect {

		/**
		 * The top left corner of the rectangle
		 */
		Vector2f topLeft;

		/**
		 * The bottom right corner of the rectangle
		 */
		Vector2f bottomRight;

		/**
		 * Gets the top right corner of the rectangle
		 * @returns the top right corner of the rectangle
		 */
		Vector2f topRight() const;

		/**
		 * Gets the bottom left corner of the rectangle
		 * @returns the bottom left corner of the rectangle
		 */
		Vector2f bottomLeft() const;

		/**
		 * Gets the center of the rectangle
		 * @returns the center of the rectangle
		 */
		Vector2f center() const;

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
