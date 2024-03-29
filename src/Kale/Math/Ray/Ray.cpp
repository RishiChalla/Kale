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

#include "Ray.hpp"

#include <Kale/Math/RotatedRect/RotatedRect.hpp>
#include <Kale/Math/Rect/Rect.hpp>
#include <Kale/Math/Line/Line.hpp>
#include <Kale/Math/Circle/Circle.hpp>

#include <stdexcept>
#include <cmath>

using namespace Kale;

/**
 * Creates a ray originating from 0, 0 and pointing right
 */
Ray::Ray() : origin(0, 0), direction(1, 0) {
	// Empty Body
}

/**
 * Creates a new ray given the origin and direction unit vector
 * @param origin The origin of the ray
 * @param direction The direction unit vector, must have a magnitude of 1.
 */
Ray::Ray(const Vector2f& origin, const Vector2f& direction) : origin(origin), direction(direction) {
	// Empty Body
}

/**
 * Creates a new ray from a line with the first point as the origin pointing towards the second point
 * @param line The line to create from
 */
Ray::Ray(const Line& line) : origin(line.point1), direction((line.point2 - line.point1).unit()) {
	// Empty Body
}

/**
 * Creates a new ray given the origin and angle of casting
 * @param origin The origin of the ray
 * @param angle The angle of casting
 * @param unit The type of angle unit given
 */
Ray::Ray(const Vector2f& origin, float angle, AngleUnit unit) : origin(origin) {
	direction.x = std::cos(unit == AngleUnit::Degree ? degToRad(angle) : angle);
	direction.y = std::sin(unit == AngleUnit::Degree ? degToRad(angle) : angle);
}

/**
 * Gets the angle of the direction
 * @param unit The type of unit to return the angle in
 * @returns the angle of the direction
 */
float Ray::getAngle(AngleUnit unit) const {
	float angle = std::tan(direction.y / direction.x);
	if (unit == AngleUnit::Degree) angle = radToDeg(angle);
	return angle;
}

/**
 * Checks whether or not the ray is perpendicular with this ray
 * @param ray The ray to check with
 * @returns Whether or not the ray is perpendicular with this ray
 */
bool Ray::isPerpendicular(Ray ray) const {
	return isFloating0(ray.direction.dot(direction));
}

/**
 * Checks whether or not the line is perpendicular with this ray
 * @param line The line to check with
 * @returns Whether or not the line is perpendicular with this ray
 */
bool Ray::isPerpendicular(Line line) const {
    return isFloating0(direction.dot(line.point2 - line.point1));
}

/**
 * Checks whether or not the ray is parallel with this ray
 * @param ray The ray to check with
 * @returns Whether or not the ray is parallel with this ray
 */
bool Ray::isParallel(Ray ray) const {
	return ray.direction == direction;
}

/**
 * Checks whether or not the line is parallel with this ray
 * @param line The line to check with
 * @returns Whether or not the line is parallel with this ray
 */
bool Ray::isParallel(Line line) const {
	return isFloating0(direction.cross(line.point1 - line.point2));
}

/**
 * Gets a bounding box for this geometry to check for quick and inaccurate collisions
 * @returns The bounding box
 */
Rect Ray::getBoundingBox() const {
#ifdef KALE_DEBUG
	throw std::runtime_error("Cannot find bounding box of a ray - rays are infinite in length");
#else
	return Rect();
#endif
}

/**
 * Checks for collision with a point
 * @param point The point to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Ray::pointCollision(Vector2f point) const {
	Vector2f po = point - origin;
    return isFloating0(po.cross(direction)) && po.dot(direction) > 0.0f;
}

/**
 * Checks for collision with a rectangle
 * @param rect The rectangle to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Ray::rectCollision(RotatedRect rect) const {
	return lineCollision({rect.point1, rect.point2}) || lineCollision({rect.point2, rect.point3}) ||
		lineCollision({rect.point3, rect.point4}) || lineCollision({rect.point4, rect.point1});
}

/**
 * Checks for collision with a rectangle
 * @param rect The rectangle to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Ray::rectCollision(Rect rect) const {
	return lineCollision({rect.topLeft, rect.topRight()}) || lineCollision({rect.topRight(), rect.bottomRight}) ||
		lineCollision({rect.bottomRight, rect.bottomLeft()}) || lineCollision({rect.bottomLeft(), rect.topLeft});
}

/**
 * Checks for collision with a circle
 * @param circle The circle to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Ray::circleCollision(Circle circle) const {
	return circle.rayCollision(*this);
}

/**
 * Checks for collision with a ray
 * @param ray The ray to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Ray::rayCollision(Ray ray) const {
	// Rays are parallel in the same direction
	if (ray.direction == direction) return isFloating0(direction.cross(ray.origin - origin));

	// Rays are parallel in the opposite direction
	if (-ray.direction == direction)
		return isFloating0(direction.cross(ray.origin - origin)) && (direction.x > 0.0f ? ray.origin.x >= origin.x : ray.origin.x <= origin.x);

	// Convert both rays to y = ax + b form
	float a1 = direction.y / direction.x;
	float a2 = ray.direction.y / ray.direction.x;
	float b1 = origin.y - a1 * origin.x;
	float b2 = ray.origin.y - a2 * ray.origin.x;

	// Find collision x
	float colX = (b2 - b1) / (a1 - a2);

	// Ensure that the x faces in the ray's direction for both rays
	return sign(colX - origin.x) == sign(direction.x) && sign(colX - ray.origin.x) == sign(ray.direction.x);
}

/**
 * Checks for collision with a line
 * @param line The line to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Ray::lineCollision(Line line) const {
	// Convert ray & line to y = ax + b form
	float a1 = direction.y / direction.x;
	float a2 = (line.point2.y - line.point1.y) / (line.point2.x - line.point2.x);

	// Check for parallel lines
	float den = a1 - a2;
	if (isFloating0(den))
		return isFloating0(direction.cross(line.point1 - origin)) &&
			(sign(line.point1.x - origin.x) == sign(direction.x) || sign(line.point2.x - origin.x) == sign(direction.x));

	float b1 = origin.y - a1 * origin.x;
	float b2 = line.point1.y - a2 * line.point1.x;

	// Find collision x
	float colX = (b2 - b1) / den;

	// Ensure that the collision x faces the ray's direction and is in line's bounds
	return colX >= line.point1.x && colX <= line.point2.x && sign(colX - origin.x) == sign(direction.x);
}
