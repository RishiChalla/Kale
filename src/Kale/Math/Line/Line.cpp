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

#include "Line.hpp"

#include <Kale/Math/Rect/Rect.hpp>
#include <Kale/Math/Ray/Ray.hpp>
#include <Kale/Math/Path/Path.hpp>
#include <Kale/Math/Circle/Circle.hpp>

#include <stdexcept>

using namespace Kale;

/**
 * Initializes the line from 0, 0 to 0, 0
 */
Line::Line() : point1(0, 0), point2(0, 0) {
	// Empty Body
}

/**
 * Creates a new line given the two endpoints
 * @param point1 The first endpoint
 * @param point2 The second endpoint
 */
Line::Line(const Vector2f &point1, const Vector2f &point2) : point1(point1), point2(point2) {
    // Empty Body
}

/**
 * Gets the angle of the direction in degrees
 * @returns the angle of the direction in degrees
 */
float Line::getAngleDeg() const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Gets The angle of the direction in radians
 * @returns The angle of the direction in radians
 */
float Line::getAngleRad() const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Returns the normal unit vector of the direction of this line
 * @returns The normal unit vector
 */
Vector2f Line::getNormal() const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks whether or not the ray is perpendicular with this line
 * @param ray The ray to check with
 * @returns Whether or not the ray is perpendicular with this line
 */
bool Line::isPerpendicular(Ray ray) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks whether or not the line is perpendicular with this line
 * @param line The line to check with
 * @returns Whether or not the line is perpendicular with this line
 */
bool Line::isPerpendicular(Line line) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks whether or not the ray is parallel with this line
 * @param ray The ray to check with
 * @returns Whether or not the ray is parallel with this line
 */
bool Line::isParallel(Ray ray) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks whether or not the line is parallel with this line
 * @param line The line to check with
 * @returns Whether or not the line is parallel with this line
 */
bool Line::isParallel(Line line) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a point
 * @param point The point to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Line::pointCollision(Vector2f point) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a rectangle
 * @param rect The rectangle to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Line::rectCollision(Rect rect) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a circle
 * @param circle The circle to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Line::circleCollision(Circle circle) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a ray
 * @param ray The ray to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Line::rayCollision(Ray ray) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a path
 * @param path The path to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Line::pathCollision(const Path& path) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a line
 * @param line The line to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Line::lineCollision(Line line) const {
    throw std::runtime_error("Unimplemented method");
}
