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
 * Creates a path given a closed shape made of lines and a corner radius to use to round each corner of the shape
 * @param lines A vector of the lines, each element has a line drawn to the next and the last element has a line drawn to the first
 * @param cornerRadius The corner radius to use to round the corners of the shape
 */
Path::Path(const std::vector<Vector2f>& lines, float cornerRadius) {
	std::vector<std::pair<Vector2f, float>> l(lines.size());
	std::transform(lines.begin(), lines.end(), l.begin(), [&](const Vector2f& pos) -> std::pair<Vector2f, float> {
		return std::make_pair(pos, cornerRadius);
	});
	roundCorners(l);
}

/**
 * Creates a path given a closed shape of lines, and a corner radius to use to round each corner of the shape
 * @param lines A vector of the lines, each element has a line drawn to the next and the last element has a line drawn to the first.
 * The second item in the pair is the corner radius to the corner created by the specific point, you can use a negative number or 0 to
 * denote no corner rounding.
 */
Path::Path(const std::vector<std::pair<Vector2f, float>>& lines) {
	roundCorners(lines);
}

/**
 * Creates a path with rounded corners
 * @param lines The lines to create the path from
 */
void Path::roundCorners(const std::vector<std::pair<Vector2f, float>>& lines) {

	// Loop through all of the corners and attempt to round them
	moveTo(lines[0].first);

	for (size_t i = 0; i < lines.size(); i++) {

		// Get the radius for this corner
		float radius = lines[i].second;

		// Get the corner, next, and previous points
		Vector2f corner = lines[i].first;
		Vector2f prev = i != 0 ? lines[i-1].first : lines.back().first;
		Vector2f next = i+1 < lines.size() ? lines[i+1].first : lines.front().first;

		// Calculate the cross product to see if our angle is acute or obtuse
		float cp = (prev - corner).cross(next - corner);
		if (isFloating0(cp) || radius <= 0.0f) {
			lineTo(corner);
			continue;
		}

		Vector2f prevOffset, nextOffset;
		bool clockwise;

		// Acute angle
		if (cp < 0) {
			clockwise = false;
			prevOffset = (prev - corner).rotateClockwise().normalized() * radius;
			nextOffset = (next - corner).rotateCounterClockwise().normalized() * radius;
		}
		// Obtuse angle
		else {
			clockwise = true;
			prevOffset = (prev - corner).rotateCounterClockwise().normalized() * radius;
			nextOffset = (next - corner).rotateClockwise().normalized() * radius;
		}

		// Create 2 parallel lines next to the previous - corner & next - corner lines
		Vector2f prevLp0 = prev + prevOffset;
		Vector2f prevLp1 = corner + prevOffset;
		Vector2f nextLp0 = next + nextOffset;
		Vector2f nextLp1 = corner + nextOffset;

		Vector2f prevDist = prevLp1 - prevLp0;
		Vector2f nextDist = nextLp1 - nextLp0;

		// Calculate the times of their intersection
		float prevT = (nextLp0 - prevLp0).cross(nextDist / prevDist.cross(nextDist));
		float nextT = (nextLp0 - prevLp0).cross(prevDist / prevDist.cross(nextDist));

		Vector2f center = prevLp0 + prevDist * prevT;
		Vector2f truePrev = prev + prevDist * prevT;
		Vector2f trueNext = next + nextDist * nextT;

		lineTo(truePrev);
		arcTo(trueNext, center, clockwise);
	}

	closePath();
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
 * Moves the starting point of the path to a certain point.
 * @note This function must be used when there are no beziers in the path,
 * and other path modification methods must be used directly after.
 * @param pos The position to move to
 */
void Path::moveTo(Vector2f pos) {
	beziers.clear();
	beziers.push_back(CubicBezier{pos, Vector2f::zero(), Vector2f::zero(), Vector2f::zero()});
}

/**
 * Moves the path to a new position while drawing a line to the new position.
 * @note Do not add beziers manually when using path modification methods
 * @param pos The position to draw a line to
 */
void Path::lineTo(Vector2f pos) {
	beziers.back().controlPoint1 = beziers.back().start;
	beziers.back().controlPoint2 = pos;
	beziers.back().end = pos;
	beziers.push_back(CubicBezier{pos, Vector2f::zero(), Vector2f::zero(), Vector2f::zero()});
}

/**
 * Moves the path to a new position while drawing a cubic bezier curve to the new position.
 * @note Do not add beziers manually when using path modification methods
 * @param controlPoint1 The first control point of the bezier
 * @param controlPoint2 The second control point of the bezier
 * @param end The ending point of the bezier
 */
void Path::bezierTo(Vector2f controlPoint1, Vector2f controlPoint2, Vector2f end) {
	beziers.back().controlPoint1 = controlPoint1;
	beziers.back().controlPoint2 = controlPoint2;
	beziers.back().end = end;
	beziers.push_back(CubicBezier{end, Vector2f::zero(), Vector2f::zero(), Vector2f::zero()});
}

/**
 * Creates an arc (a segment of a circle's circumference) to a point given a center
 * @note Do not add beziers manually when using path modification methods
 * @param end The ending point where the arc ends
 * @param center The center of the circle of which the arc is drawn to
 * @param clockwise Whether or not to draw the arc clockwise or counter clockwise to the ending position
 */
void Path::arcTo(Vector2f end, Vector2f center, bool clockwise) {
	// Calculate the number of segments we need to approximate the curve (1 - 0:90, 2 - 90:180, 3 - 180:270, 4 - 270:360)
	float radius = (end - center).magnitude();
	float angle = (beziers.back().start - center).signedAngle(end - center);
	int numsegments;
	if (angle > 0) numsegments = static_cast<int>(ceil(angle / (0.5f * PI)));
	else numsegments = static_cast<int>(ceil((angle + 2.0f * PI) / (0.5f * PI)));

	if (clockwise) numsegments = std::max(4 - numsegments, 1);

	// Prep for loop
	float a = angle * (1.0f / static_cast<float>(numsegments));
	if (clockwise) a += 2.0f * PI;

	// This is a constant needed to approximate a circle with beziers - 
	// https://stackoverflow.com/questions/1734745/how-to-create-circle-with-bézier-curves
	float bezierArcApprox = (4.0f / 3.0f) * tan(PI / (2.0f * (static_cast<float>(numsegments) * 2.0f * PI / angle)));

	// Loop through the segments
	for (size_t i = 0; i < numsegments; i++) {

		// Rotate the end of the last segment about the center
		CubicBezier& last = beziers.back();
		float c = cos(a);
		float s = sin(a);
		Vector2f n = Vector2f(
			(c * (last.start.x - center.x)) - (s * (last.start.y - center.y)),
			(s * (last.start.x - center.x)) + (c * (last.start.y - center.y))
		) + center;

		// Create the bezier approximation
		last.controlPoint1 = last.start + (center - last.start).rotateClockwise().normalized() * bezierArcApprox * radius;
		last.controlPoint2 = n + (center - n).rotateCounterClockwise().normalized() * bezierArcApprox * radius;
		last.end = n;
		beziers.push_back(CubicBezier{n, Vector2f::zero(), Vector2f::zero(), Vector2f::zero()});
	}
}

/**
 * Closes the path allowing for rendering
 * @note Do not add beziers manually when using path modification methods, this method closes off the path making it suitable for rendering
 */
void Path::closePath() {
	if (beziers.front().controlPoint1 == beziers.front().start) beziers.front().controlPoint1 = beziers.back().start;
	beziers.front().start = beziers.back().start;
	beziers.pop_back();
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
