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

#include "PathNode.hpp"

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
		bezier.controlPoint1 *= value;
		bezier.end *= value;
	}
	return path;
}

/**
 * Converts the path to a skia path
 * @param camera The camera to transform with
 * @returns The skia path
 */
SkPath Path::toSkia(const Camera& camera) const {
	SkPath path;
	if (beziers.size() == 0) return path;
	path.moveTo(camera.transform(beziers[0].start));
	for (const CubicBezier& bezier : beziers) {
		path.cubicTo(
			camera.transform(bezier.controlPoint1),
			camera.transform(bezier.controlPoint2),
			camera.transform(bezier.end)
		);
	}
	return path;
}
