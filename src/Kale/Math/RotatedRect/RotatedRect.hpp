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
	 * Represents a rectangular structure of float vectors
	 */
	struct RotatedRect : public Geometry {

		/**
		 * The first point in the rectangle
		 */
		Vector2f point1;

		/**
		 * The second point in the rectangle
		 */
		Vector2f point2;

		/**
		 * The third point in the rectangle
		 */
		Vector2f point3;

		/**
		 * The fourth point in the rectangle
		 */
		Vector2f point4;

		/**
		 * Creates a rectangle with all points at 0, 0
		 */
		RotatedRect();

        /**
         * Creates a new rectangle given four points. Points must be specified in clockwise order.
         * @param point1 The first point
         * @param point2 The second point
         * @param point3 The third point
         * @param point4 The fourth point
         */
        RotatedRect(const Vector2f& point1, const Vector2f& point2, const Vector2f& point3, const Vector2f& point4);

        /**
         * Creates a new rectangle from the topLeft and bottomRight points
         * @param topLeft The top left point
         * @param bottomRight The bottom right point
         */
        RotatedRect(const Vector2f& topLeft, const Vector2f& bottomRight);

        /**
         * Gets the center of the rectangle
         * @returns the center of the rectangle
         */
		Vector2f center() const;

		/**
		 * Gets a bounding box for this geometry to check for quick and inaccurate collisions
		 * @return The bounding box
		 */
		Rect getBoundingBox() const override;

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
