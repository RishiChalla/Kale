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

#include <Kale/Math/Rect/Rect.hpp>
#include <Kale/Math/Line/Line.hpp>
#include <Kale/Math/Path/Path.hpp>
#include <Kale/Math/Circle/Circle.hpp>

#include <stdexcept>

using namespace Kale;

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
bool Ray::pathCollision(Path path) const {
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
