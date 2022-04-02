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

#include <core/SkPath.h>

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
	struct Path : public Geometry {

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
		Path();

		/**
		 * Creates an empty path given an origin
		 * @param origin The origin of the path
		 */
		Path(Vector2f origin);

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
