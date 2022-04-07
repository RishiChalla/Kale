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
	 * Represents a line formed by two points
	 */
	struct Line : public Geometry {

		/**
		 * The first point of the line
		 */
		Vector2f point1;

		/**
		 * The second point of the line
		 */
		Vector2f point2;

		/**
		 * Initializes the line from 0, 0 to 0, 0
		 */
		Line();

        /**
         * Creates a new line given the two endpoints
         * @param point1 The first endpoint
         * @param point2 The second endpoint
         */
        Line(const Vector2f& point1, const Vector2f& point2);

		/**
		 * Returns the normal unit vector of the direction of this line
		 * @returns The normal unit vector
		 */
		Vector2f getNormal() const;

		/**
		 * Checks whether or not the ray is perpendicular with this line
		 * @param ray The ray to check with
		 * @returns Whether or not the ray is perpendicular with this line
		 */
		bool isPerpendicular(Ray ray) const;
		
		/**
		 * Checks whether or not the line is perpendicular with this line
		 * @param line The line to check with
		 * @returns Whether or not the line is perpendicular with this line
		 */
		bool isPerpendicular(Line line) const;
		
		/**
		 * Checks whether or not the ray is parallel with this line
		 * @param ray The ray to check with
		 * @returns Whether or not the ray is parallel with this line
		 */
		bool isParallel(Ray ray) const;
		
		/**
		 * Checks whether or not the line is parallel with this line
		 * @param line The line to check with
		 * @returns Whether or not the line is parallel with this line
		 */
		bool isParallel(Line line) const;

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
		 * Checks for collision with a line
		 * @param line The line to check collision for
		 * @returns True if there is a collision, false for no collision
		 */
		bool lineCollision(Line line) const override;

	};
}
