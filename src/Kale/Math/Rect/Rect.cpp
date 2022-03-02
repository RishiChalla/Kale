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

#include "Rect.hpp"

#include <Kale/Math/RotatedRect/RotatedRect.hpp>
#include <Kale/Math/Ray/Ray.hpp>
#include <Kale/Math/Line/Line.hpp>
#include <Kale/Math/Path/Path.hpp>
#include <Kale/Math/Circle/Circle.hpp>

#include <stdexcept>

using namespace Kale;

/**
 * Creates a rectangle with all points at 0, 0
 */
Rect::Rect() : topLeft(0, 0), bottomRight(0, 0) {
	// Empty Body
}

/**
 * Creates a new rectangle from the topLeft and bottomRight points
 * @param topLeft The top left point
 * @param bottomRight The bottom right point
 */
Rect::Rect(const Vector2f &topLeft, const Vector2f &bottomRight) : topLeft(topLeft), bottomRight(bottomRight) {
	// Empty Body
}

/**
 * Gets the center of the rectangle
 * @returns the center of the rectangle
 */
Vector2f Rect::center() const {
	return (topLeft + bottomRight) / 2;
}

/**
 * Gets the top right point of the rectangle
 * @return the top right point of the rectangle
 */
Vector2f Rect::topRight() const {
	return {bottomRight.x, topLeft.y};
}

/**
 * Gets the bottom left point of the rectangle
 * @return the bottom left point of the rectangle
 */
Vector2f Rect::bottomLeft() const {
	return {topLeft.x, bottomRight.y};
}

/**
 * Checks for collision with a point
 * @param point The point to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Rect::pointCollision(Vector2f point) const {
	return point >= topLeft && point <= bottomRight;
}

/**
 * Checks for collision with a rectangle
 * @param rect The rectangle to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Rect::rectCollision(RotatedRect rect) const {
	return rect.rectCollision(RotatedRect{topLeft, bottomRight});
}

/**
 * Checks for collision with a rectangle
 * @param rect The rectangle to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Rect::rectCollision(Rect rect) const {
	return topLeft <= rect.bottomRight && bottomRight >= rect.topLeft;
}

/**
 * Checks for collision with a circle
 * @param circle The circle to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Rect::circleCollision(Circle circle) const {
	Vector2f closest = circle.center.clamp(topLeft, bottomRight);
	return (circle.center - closest).magnitude() <= circle.radius;
}

/**
 * Checks for collision with a ray
 * @param ray The ray to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Rect::rayCollision(Ray ray) const {
	throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a path
 * @param path The path to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Rect::pathCollision(const Path& path) const {
	throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a line
 * @param line The line to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Rect::lineCollision(Line line) const {
	// Check if line cap points are in rectangle
	if (pointCollision(line.point1) || pointCollision(line.point2)) return true;

	// Calculate x location of line at rectangle top y and bottom y
	Vector2f dist = line.point1 - line.point2;
	float col1x = dist.x * (topLeft.y - line.point1.y) / dist.y;
	float col2x = dist.x * (bottomRight.y - line.point1.y) / dist.y;

	// Check if the x location of the line at the top/bottom y of rect is within rect x bounds
	return (topLeft.x >= col1x && col1x <= bottomRight.x) || (topLeft.x >= col2x && col2x <= bottomRight.x);
}

/**
 * Checks for collision with a rectangle
 * @param rect The rectangle to check collision for
 * @returns The points of collision on the colliding edges of the geometry
 */
std::vector<Vector2f> Rect::rectCollisionPoints(RotatedRect rect) const {
	throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a rectangle
 * @param rect The rectangle to check collision for
 * @returns The points of collision on the colliding edges of the geometry
 */
std::vector<Vector2f> Rect::rectCollisionPoints(Rect rect) const {
	throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a circle
 * @param circle The circle to check collision for
 * @returns The points of collision on the colliding edges of the geometry
 */
std::vector<Vector2f> Rect::circleCollisionPoints(Circle circle) const {
	throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a ray
 * @param ray The ray to check collision for
 * @returns The points of collision on the colliding edges of the geometry
 */
std::vector<Vector2f> Rect::rayCollisionPoints(Ray ray) const {
	throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a path
 * @param path The path to check collision for
 * @returns The points of collision on the colliding edges of the geometry
 */
std::vector<Vector2f> Rect::pathCollisionPoints(const Path& path) const {
	throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a line
 * @param line The line to check collision for
 * @returns The points of collision on the colliding edges of the geometry
 */
std::vector<Vector2f> Rect::lineCollisionPoints(Line line) const {
	throw std::runtime_error("Unimplemented method");
}
