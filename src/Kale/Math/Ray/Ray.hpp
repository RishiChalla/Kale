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
#include <Kale/Math/Geometry/Geometry.hpp>

namespace Kale {

	/**
	 * Represents a ray coming from a point
	 */
	struct Ray : public Geometry {

		/**
		 * The origin of the ray
		 */
		Vector2f origin;

		/**
		 * The unit vector representing the direction of the ray
		 */
		Vector2f direction;

		/**
		 * Creates a ray originating from 0, 0 and pointing right
		 */
		Ray();

		/**
		 * Creates a new ray given the origin and direction unit vector
		 * @param origin The origin of the ray
		 * @param direction The direction unit vector, must have a magnitude of 1.
		 */
		Ray(const Vector2f& origin, const Vector2f& direction);

		/**
		 * Creates a new ray given the origin and angle of casting
		 * @param origin The origin of the ray
		 * @param angle The angle of casting
		 * @param deg Whether or not the angle is in degrees, true means it is in degrees, false means radians
		 */
		Ray(const Vector2f& origin, float angle, bool deg = false);

		/**
		 * Creates a new ray from a line with the first point as the origin pointing towards the second point
		 * @param line The line to create from
		 */
		Ray(const Line& line);

		/**
		 * Gets the angle of the direction in degrees
		 * @returns the angle of the direction in degrees
		 */
		float getAngleDeg() const;

		/**
		 * Gets The angle of the direction in radians
		 * @returns The angle of the direction in radians
		 */
		float getAngleRad() const;

		/**
		 * Checks whether or not the ray is perpendicular with this ray
		 * @param ray The ray to check with
		 * @returns Whether or not the ray is perpendicular with this ray
		 */
		bool isPerpendicular(Ray ray) const;
		
		/**
		 * Checks whether or not the line is perpendicular with this ray
		 * @param line The line to check with
		 * @returns Whether or not the line is perpendicular with this ray
		 */
		bool isPerpendicular(Line line) const;
		
		/**
		 * Checks whether or not the ray is parallel with this ray
		 * @param ray The ray to check with
		 * @returns Whether or not the ray is parallel with this ray
		 */
		bool isParallel(Ray ray) const;
		
		/**
		 * Checks whether or not the line is parallel with this ray
		 * @param line The line to check with
		 * @returns Whether or not the line is parallel with this ray
		 */
		bool isParallel(Line line) const;

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
