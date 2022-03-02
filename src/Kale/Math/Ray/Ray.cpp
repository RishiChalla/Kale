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
    throw std::runtime_error("Unimplemented method");
}

/**
 * Gets The angle of the direction in radians
 * @returns The angle of the direction in radians
 */
float Ray::getAngleRad() const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks whether or not the ray is perpendicular with this ray
 * @param ray The ray to check with
 * @returns Whether or not the ray is perpendicular with this ray
 */
bool Ray::isPerpendicular(Ray ray) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks whether or not the line is perpendicular with this ray
 * @param line The line to check with
 * @returns Whether or not the line is perpendicular with this ray
 */
bool Ray::isPerpendicular(Line line) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks whether or not the ray is parallel with this ray
 * @param ray The ray to check with
 * @returns Whether or not the ray is parallel with this ray
 */
bool Ray::isParallel(Ray ray) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks whether or not the line is parallel with this ray
 * @param line The line to check with
 * @returns Whether or not the line is parallel with this ray
 */
bool Ray::isParallel(Line line) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a point
 * @param point The point to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Ray::pointCollision(Vector2f point) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a rectangle
 * @param rect The rectangle to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Ray::rectCollision(RotatedRect rect) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a rectangle
 * @param rect The rectangle to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Ray::rectCollision(Rect rect) const {
	throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a circle
 * @param circle The circle to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Ray::circleCollision(Circle circle) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a ray
 * @param ray The ray to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Ray::rayCollision(Ray ray) const {
    throw std::runtime_error("Unimplemented method");
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
    throw std::runtime_error("Unimplemented method");
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
