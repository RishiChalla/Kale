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

#include <Kale/Math/Geometry/Geometry.hpp>

namespace Kale {

	/**
	 * Represents a circle
	 */
	struct Circle : public Geometry {

		/**
		 * The center of the circle
		 */
		Vector2f center;

		/**
		 * The radius of the circle
		 */
		float radius;

		/**
		 * Creates a circle at 0, 0 with a radius of 1
		 */
		Circle();

		/**
		 * Creates a circle given the center and radius
		 * @param center The center
		 * @param radius The radius
		 */
		Circle(const Vector2f& center, float radius);

		/**
		 * Checks for collision with a point
		 * @param point The point to check collision for
		 * @returns True if there is a collision, false for no collision
		 */
		bool pointCollision(Vector2f point) const override;

		/**
		 * Checks for collision with a rectangle
		 * @param rect The rectangle to check collision for
		 * @returns True if there is a collision, false for no collision
		 */
		bool rectCollision(RotatedRect rect) const override;

		/**
		 * Checks for collision with a rectangle
		 * @param rect The rectangle to check collision for
		 * @returns True if there is a collision, false for no collision
		 */
		bool rectCollision(Rect rect) const override;

		/**
		 * Checks for collision with a circle
		 * @param circle The circle to check collision for
		 * @returns True if there is a collision, false for no collision
		 */
		bool circleCollision(Circle circle) const override;

		/**
		 * Checks for collision with a ray
		 * @param ray The ray to check collision for
		 * @returns True if there is a collision, false for no collision
		 */
		bool rayCollision(Ray ray) const override;

		/**
		 * Checks for collision with a path
		 * @param path The path to check collision for
		 * @returns True if there is a collision, false for no collision
		 */
		bool pathCollision(const Path& path) const override;

		/**
		 * Checks for collision with a line
		 * @param line The line to check collision for
		 * @returns True if there is a collision, false for no collision
		 */
		bool lineCollision(Line line) const override;

		/**
		 * Checks for collision with a rectangle
		 * @param rect The rectangle to check collision for
		 * @returns The points of collision on the colliding edges of the geometry
		 */
		std::vector<Vector2f> rectCollisionPoints(RotatedRect rect) const override;

		/**
		 * Checks for collision with a rectangle
		 * @param rect The rectangle to check collision for
		 * @returns The points of collision on the colliding edges of the geometry
		 */
		std::vector<Vector2f> rectCollisionPoints(Rect rect) const override;

		/**
		 * Checks for collision with a circle
		 * @param circle The circle to check collision for
		 * @returns The points of collision on the colliding edges of the geometry
		 */
		std::vector<Vector2f> circleCollisionPoints(Circle circle) const override;

		/**
		 * Checks for collision with a ray
		 * @param ray The ray to check collision for
		 * @returns The points of collision on the colliding edges of the geometry
		 */
		std::vector<Vector2f> rayCollisionPoints(Ray ray) const override;

		/**
		 * Checks for collision with a path
		 * @param path The path to check collision for
		 * @returns The points of collision on the colliding edges of the geometry
		 */
		std::vector<Vector2f> pathCollisionPoints(const Path& path) const override;

		/**
		 * Checks for collision with a line
		 * @param line The line to check collision for
		 * @returns The points of collision on the colliding edges of the geometry
		 */
		std::vector<Vector2f> lineCollisionPoints(Line line) const override;

	};
}
