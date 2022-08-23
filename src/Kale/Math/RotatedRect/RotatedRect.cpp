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

#include "RotatedRect.hpp"

#include <Kale/Math/Rect/Rect.hpp>
#include <Kale/Math/Ray/Ray.hpp>
#include <Kale/Math/Line/Line.hpp>
#include <Kale/Math/Circle/Circle.hpp>

#include <stdexcept>

using namespace Kale;

/**
 * Creates a rectangle with all points at 0, 0
 */
RotatedRect::RotatedRect() : point1(0, 0), point2(0, 0), point3(0, 0), point4(0, 0) {
	// Empty Body
}

/**
 * Creates a new rectangle given four points
 * @param point1 The first point
 * @param point2 The second point
 * @param point3 The third point
 * @param point4 The fourth point
 */
RotatedRect::RotatedRect(const Vector2f &point1, const Vector2f &point2, const Vector2f &point3, const Vector2f &point4) :
    point1(point1), point2(point2), point3(point3), point4(point4) {
    // Empty Body
}

/**
 * Creates a new rectangle from the topLeft and bottomRight points
 * @param topLeft The top left point
 * @param bottomRight The bottom right point
 */
RotatedRect::RotatedRect(const Vector2f &point1, const Vector2f &point3) : point1(point1), point2(point3.x, point1.y), point3(point3), point4(point1.x, point3.y) {
    // Empty Body
}

/**
 * Gets the center of the rectangle
 * @returns the center of the rectangle
 */
Vector2f RotatedRect::center() const {
    return (point1 + point3) / 2;
}

/**
 * Gets a bounding box for this geometry to check for quick and inaccurate collisions
 * @returns The bounding box
 */
Rect RotatedRect::getBoundingBox() const {
	return {
		{std::min({point1.x, point2.x, point3.x, point4.x}), std::max({point1.y, point2.y, point3.y, point4.y})},
		{std::max({point1.x, point2.x, point3.x, point4.x}), std::min({point1.y, point2.y, point3.y, point4.y})}
	};
}

/**
 * Checks for collision with a point
 * @param point The point to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool RotatedRect::pointCollision(Vector2f point) const {
	return (point1 - point2).dot(point - point2) > 0.0f &&
		(point3 - point4).dot(point - point4) > 0.0f &&
		(point1 - point4).dot(point - point4) > 0.0f &&
		(point3 - point2).dot(point - point2) > 0.0f;
}

/**
 * Checks for collision with a rectangle
 * @param rect The rectangle to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool RotatedRect::rectCollision(RotatedRect rect) const {
	return (point2 - point1).dot(rect.point3 - point1) > 0.0f &&
		(point4 - point1).dot(rect.point3 - point1) > 0.0f &&
		(rect.point2 - rect.point1).dot(point3 - rect.point1) > 0.0f &&
		(rect.point4 - rect.point1).dot(point3 - rect.point1) > 0.0f;
}

/**
 * Checks for collision with a rectangle
 * @param rect The rectangle to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool RotatedRect::rectCollision(Rect rect) const {
	return rectCollision(RotatedRect{rect.topLeft, rect.bottomRight});
}

/**
 * Checks for collision with a circle
 * @param circle The circle to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool RotatedRect::circleCollision(Circle circle) const {
	return circle.rectCollision(*this);
}

/**
 * Checks for collision with a ray
 * @param ray The ray to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool RotatedRect::rayCollision(Ray ray) const {
	return ray.rectCollision(*this);
}

/**
 * Checks for collision with a line
 * @param line The line to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool RotatedRect::lineCollision(Line line) const {
	return line.rectCollision(*this);
}
