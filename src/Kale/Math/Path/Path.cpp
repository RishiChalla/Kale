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

#include <Kale/Core/Logger/Logger.hpp>

#include <limits>

using namespace Kale;

/**
 * Creates a new empty path
 */
Path::Path() {
	// Empty Body
}

/**
 * Creates a path with a size with all points at 0
 * @param n The size
 */
Path::Path(size_t n) : beziers(n) {
	// Empty Body
}

/**
 * Creates a path given beziers
 * @param beziers The beziers
 */
Path::Path(const std::vector<CubicBezier>& beziers) : beziers(beziers) {
	// Empty Body
}

/**
 * Gets the bounding box for this path
 * @returns The bounding box
 */
Rect Path::getBoundingBox() const {
	Vector2f topLeft = Vector2f::max();
	Vector2f bottomRight = Vector2f::min();
	
	for (const CubicBezier& bezier : beziers) {
		for (const Vector2f& point : {bezier.start, bezier.controlPoint1, bezier.controlPoint2, bezier.end}) {
			if (point.x < topLeft.x) topLeft.x = point.x;
			if (point.y < topLeft.y) topLeft.y = point.y;
			if (point.x > bottomRight.x) bottomRight.x = point.x;
			if (point.y > bottomRight.y) bottomRight.y = point.y;
		}
	}

	return {topLeft, bottomRight};
}

/**
 * Adds another path to this
 * @param other The path to add to this
 */
void Path::operator+=(const Path& other) {
	klAssert(other.beziers.size() == beziers.size());
	for (size_t i = 0; i < beziers.size(); i++) {
		beziers[i].start += other.beziers[i].start;
		beziers[i].controlPoint1 += other.beziers[i].controlPoint1;
		beziers[i].controlPoint2 += other.beziers[i].controlPoint2;
		beziers[i].end += other.beziers[i].end;
	}
}

/**
 * Multiplies this path's points by a value
 * @param value The scalar value
 */
Path Path::operator*(float value) const {
	Path path(*this);
	for (CubicBezier& bezier : path.beziers) {
		bezier.start *= value;
		bezier.controlPoint1 *= value;
		bezier.controlPoint2 *= value;
		bezier.end *= value;
	}
	return path;
}

/**
 * Prints a bezier to an output stream
 */
std::ostream& Kale::operator<<(std::ostream& os, const CubicBezier& bezier) {
	os << "CubicBezier(" << bezier.start << ", " << bezier.controlPoint1 << ", " << bezier.controlPoint2 << ", " << bezier.end << ")";
	return os;
}

/**
 * Prints a path to an output stream
 */
std::ostream& Kale::operator<<(std::ostream& os, const Path& path) {
	os << "Path(\n";
	for (const CubicBezier& bezier : path.beziers) os << bezier << "\n";
	os << ")";
	return os;
}
