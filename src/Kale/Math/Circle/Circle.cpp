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

#include "Circle.hpp"

#include <Kale/Math/RotatedRect/RotatedRect.hpp>
#include <Kale/Math/Rect/Rect.hpp>
#include <Kale/Math/Ray/Ray.hpp>
#include <Kale/Math/Line/Line.hpp>

#include <stdexcept>

using namespace Kale;

/**
 * Creates a circle at 0, 0 with a radius of 1
 */
Circle::Circle() : center(0, 0), radius(1) {
	// Empty Body
}

/**
 * Creates a circle given the center and radius
 * @param center The center
 * @param radius The radius
 */
Circle::Circle(const Vector2f& center, float radius) : center(center), radius(radius) {
	// Empty Body
}

/**
 * Gets a bounding box for this geometry to check for quick and inaccurate collisions
 * @return The bounding box
 */
Rect Circle::getBoundingBox() const {
	return {center + Vector2f{-radius, radius}, center + Vector2f{radius, -radius}};
}

/**
 * Checks for collision with a point
 * @param point The point to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Circle::pointCollision(Vector2f point) const {
	Vector2f tmp = point - center;
	return tmp.dot(tmp) <= radius * radius;
}

/**
 * Checks for collision with a rectangle
 * @param rect The rectangle to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Circle::rectCollision(RotatedRect rect) const {
	if (rect.pointCollision(center)) return true;
	return lineCollision({rect.point1, rect.point2}) || lineCollision({rect.point2, rect.point3}) ||
		lineCollision({rect.point3, rect.point4}) || lineCollision({rect.point4, rect.point1});
}

/**
 * Checks for collision with a rectangle
 * @param rect The rectangle to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Circle::rectCollision(Rect rect) const {
	return rect.circleCollision(*this);
}

/**
 * Checks for collision with a circle
 * @param circle The circle to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Circle::circleCollision(Circle circle) const {
	Vector2f tmp = circle.center - center;
	return tmp.dot(tmp) <= pow(radius + circle.radius, 2.0f);
}

/**
 * Checks for collision with a ray
 * @param ray The ray to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Circle::rayCollision(Ray ray) const {
	Vector2f proj = (center - ray.origin).project(ray.direction);
	if (sign(proj.x) != sign(ray.direction.x)) proj = ray.origin;
	else proj += ray.origin;
	return pointCollision(proj);
}

/**
 * Checks for collision with a line
 * @param line The line to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Circle::lineCollision(Line line) const {
	Vector2f proj = line.point1 + (center - line.point1).project(line.point2 - line.point1);
	if (proj.x > line.point2.x) proj = line.point2;
	else if (proj.x < line.point1.x) proj = line.point1;
	return pointCollision(proj);
}
