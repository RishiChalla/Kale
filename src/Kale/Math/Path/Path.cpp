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

#include "Path.hpp"

#include <Kale/Math/RotatedRect/RotatedRect.hpp>
#include <Kale/Math/Rect/Rect.hpp>
#include <Kale/Math/Ray/Ray.hpp>
#include <Kale/Math/Line/Line.hpp>
#include <Kale/Math/Circle/Circle.hpp>

#include <stdexcept>

using namespace Kale;

/**
 * Creates an empty path with origin 0 0
 */
Path::Path() : origin(0, 0) {
	// Empty Body
}

/**
 * Creates an empty path given an origin
 * @param origin The origin of the path
 */
Path::Path(Vector2f origin) : origin(origin) {
	// Empty Body
}

/**
 * Converts the path to a skia path
 * @returns The skia path
 */
Path::operator SkPath() const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Adds a cubic bezier to the path
 * @param bezier The bezier curve to use
 */
void Path::cubicBezierTo(CubicBezier bezier) {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Adds a cubic bezier to the path
 * @param control1 The first control point
 * @param control2 The second control point
 * @param destination The destination
 */
void Path::cubicBezierTo(Vector2f control1, Vector2f control2, Vector2f destination) {
	throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a point
 * @param point The point to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Path::pointCollision(Vector2f point) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a rectangle
 * @param rect The rectangle to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Path::rectCollision(RotatedRect rect) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a rectangle
 * @param rect The rectangle to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Path::rectCollision(Rect rect) const {
	throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a circle
 * @param circle The circle to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Path::circleCollision(Circle circle) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a ray
 * @param ray The ray to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Path::rayCollision(Ray ray) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a path
 * @param path The path to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Path::pathCollision(const Path& path) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a line
 * @param line The line to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Path::lineCollision(Line line) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a rectangle
 * @param rect The rectangle to check collision for
 * @returns The points of collision on the colliding edges of the geometry
 */
std::vector<Vector2f> Path::rectCollisionPoints(RotatedRect rect) const {
	throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a rectangle
 * @param rect The rectangle to check collision for
 * @returns The points of collision on the colliding edges of the geometry
 */
std::vector<Vector2f> Path::rectCollisionPoints(Rect rect) const {
	throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a circle
 * @param circle The circle to check collision for
 * @returns The points of collision on the colliding edges of the geometry
 */
std::vector<Vector2f> Path::circleCollisionPoints(Circle circle) const {
	throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a ray
 * @param ray The ray to check collision for
 * @returns The points of collision on the colliding edges of the geometry
 */
std::vector<Vector2f> Path::rayCollisionPoints(Ray ray) const {
	throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a path
 * @param path The path to check collision for
 * @returns The points of collision on the colliding edges of the geometry
 */
std::vector<Vector2f> Path::pathCollisionPoints(const Path& path) const {
	throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a line
 * @param line The line to check collision for
 * @returns The points of collision on the colliding edges of the geometry
 */
std::vector<Vector2f> Path::lineCollisionPoints(Line line) const {
	throw std::runtime_error("Unimplemented method");
}

