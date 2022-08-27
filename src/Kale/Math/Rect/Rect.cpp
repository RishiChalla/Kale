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
 * @returns the top right point of the rectangle
 */
Vector2f Rect::topRight() const {
	return {bottomRight.x, topLeft.y};
}

/**
 * Gets the bottom left point of the rectangle
 * @returns the bottom left point of the rectangle
 */
Vector2f Rect::bottomLeft() const {
	return {topLeft.x, bottomRight.y};
}

/**
 * Gets a bounding box for this geometry to check for quick and inaccurate collisions
 * @returns The bounding box
 */
Rect Rect::getBoundingBox() const {
	return *this;
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
	Vector2f closest = circle.center - circle.center.clamp(topLeft, bottomRight);
	return closest.dot(closest) <= pow(circle.radius, 2.0f);
}

/**
 * Checks for collision with a ray
 * @param ray The ray to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Rect::rayCollision(Ray ray) const {
	return ray.rectCollision(*this);
}

/**
 * Checks for collision with a line
 * @param line The line to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Rect::lineCollision(Line line) const {
	return line.rectCollision(*this);
}
