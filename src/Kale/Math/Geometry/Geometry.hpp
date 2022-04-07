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

namespace Kale {

	/**
	 * Forward declarations of structs
	 */
	struct RotatedRect; struct Rect; struct Circle; struct Ray; struct Line;

	/**
	 * All geometries (rect/path/line/etc) must inherit from this struct.
	 * This struct contains function pointers to all collision methods
	 */
	struct Geometry {

		/**
		 * Gets a bounding box for this geometry to check for quick and inaccurate collisions
		 * @return The bounding box
		 */
		virtual Rect getBoundingBox() const = 0;

		/**
		 * Checks for collision with a point
		 * @param point The point to check collision for
		 * @returns True if there is a collision, false for no collision
		 */
		virtual bool pointCollision(Vector2f point) const = 0;

		/**
		 * Checks for collision with a rectangle
		 * @param rect The rectangle to check collision for
		 * @returns True if there is a collision, false for no collision
		 */
		virtual bool rectCollision(RotatedRect rect) const = 0;

		/**
		 * Checks for collision with a rectangle
		 * @param rect The rectangle to check collision for
		 * @returns True if there is a collision, false for no collision
		 */
		virtual bool rectCollision(Rect rect) const = 0;

		/**
		 * Checks for collision with a circle
		 * @param circle The circle to check collision for
		 * @returns True if there is a collision, false for no collision
		 */
		virtual bool circleCollision(Circle circle) const = 0;

		/**
		 * Checks for collision with a ray
		 * @param ray The ray to check collision for
		 * @returns True if there is a collision, false for no collision
		 */
		virtual bool rayCollision(Ray ray) const = 0;

		/**
		 * Checks for collision with a line
		 * @param line The line to check collision for
		 * @returns True if there is a collision, false for no collision
		 */
		virtual bool lineCollision(Line line) const = 0;
	};
}
