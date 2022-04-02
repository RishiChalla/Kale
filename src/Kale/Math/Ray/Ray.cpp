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
#include <Kale/Math/Path/Path.hpp>
#include <Kale/Math/Circle/Circle.hpp>
#include <Kale/Math/Utils/Utils.hpp>

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
 * @param deg Whether or not the angle is in degrees, true means it is in degrees, false means radians
 */
Ray::Ray(const Vector2f& origin, float angle, bool deg) : origin(origin) {
	direction.x = std::cos(deg ? degToRad(angle) : angle);
	direction.y = std::sin(deg ? degToRad(angle) : angle);
}

/**
 * Gets the angle of the direction in degrees
 * @returns the angle of the direction in degrees
 */
float Ray::getAngleDeg() const {
	return radToDeg(std::tan(direction.y / direction.x));
}

/**
 * Gets The angle of the direction in radians
 * @returns The angle of the direction in radians
 */
float Ray::getAngleRad() const {
	return std::tan(direction.y / direction.x);
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
 * Checks for collision with a path
 * @param path The path to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Ray::pathCollision(const Path& path) const {
    throw std::runtime_error("Unimplemented method");
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

/**
 * Checks for collision with a rectangle
 * @param rect The rectangle to check collision for
 * @returns The points of collision on the colliding edges of the geometry
 */
std::vector<Vector2f> Ray::rectCollisionPoints(RotatedRect rect) const {
	throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a rectangle
 * @param rect The rectangle to check collision for
 * @returns The points of collision on the colliding edges of the geometry
 */
std::vector<Vector2f> Ray::rectCollisionPoints(Rect rect) const {
	throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a circle
 * @param circle The circle to check collision for
 * @returns The points of collision on the colliding edges of the geometry
 */
std::vector<Vector2f> Ray::circleCollisionPoints(Circle circle) const {
	throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a ray
 * @param ray The ray to check collision for
 * @returns The points of collision on the colliding edges of the geometry
 */
std::vector<Vector2f> Ray::rayCollisionPoints(Ray ray) const {
	throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a path
 * @param path The path to check collision for
 * @returns The points of collision on the colliding edges of the geometry
 */
std::vector<Vector2f> Ray::pathCollisionPoints(const Path& path) const {
	throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a line
 * @param line The line to check collision for
 * @returns The points of collision on the colliding edges of the geometry
 */
std::vector<Vector2f> Ray::lineCollisionPoints(Line line) const {
	throw std::runtime_error("Unimplemented method");
}
