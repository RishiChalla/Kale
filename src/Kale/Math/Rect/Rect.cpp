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

#include <Kale/Math/Ray/Ray.hpp>
#include <Kale/Math/Line/Line.hpp>
#include <Kale/Math/Path/Path.hpp>
#include <Kale/Math/Circle/Circle.hpp>

#include <stdexcept>

using namespace Kale;

/**
 * Gets the center of the rectangle
 * @returns the center of the rectangle
 */
Vector2f Rect::center() const {
    return (point1 + point4) / 2;
}

/**
 * Checks for collision with a point
 * @param point The point to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Rect::pointCollision(Vector2f point) const {
    // https://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection
    // 0 ≤ AP·AB ≤ AB·AB and 0 ≤ AP·AD ≤ AD·AD;
    // 0 <= point1 * p · point1 * point2 <= point1 * point2 · point1 * point2 AND
    // 0 <= point1 * p · point1 * point4 <= point1 * point4 · point1 * point4
    // Vector2f p1c = point1 * point;
    // Vector2f p12 = point1 * point2;
    // Vector2f p14 = point1 * point4;
    // float d1 = p1c.dot(p12);
    // float d2 = p1c.dot(p14);
    // return 0 <= d1 && d1 <= p12.dot(p12) && 0 <= d2 && d2 <= p14.dot(p14);

    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a rectangle
 * @param rect The rectangle to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Rect::rectCollision(Rect rect) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a circle
 * @param circle The circle to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Rect::circleCollision(Circle circle) const {
    throw std::runtime_error("Unimplemented method");
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
bool Rect::pathCollision(Path path) const {
    throw std::runtime_error("Unimplemented method");
}

/**
 * Checks for collision with a line
 * @param line The line to check collision for
 * @returns True if there is a collision, false for no collision
 */
bool Rect::lineCollision(Line line) const {
    throw std::runtime_error("Unimplemented method");
}
