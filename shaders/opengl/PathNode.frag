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

#version 410

#define MAX_BEZIERS 128
#define PI 3.1415926538

uniform vec4 vertexColor;
uniform vec4 strokeColor;
uniform vec2[MAX_BEZIERS * 4] beziers;
uniform float strokeRadius;
uniform int numBeziers;
uniform int fill;
uniform int stroke; // Neither = 0, Both = 1, Inside = 2, Outside = 3

in vec2 fragPos;

out vec4 outColor;

/**
 * Calculates the bezier output at a given position
 * @param t The time at the bezier
 * @param start The bezier start
 * @param controlPoint1 The bezier control point 1
 * @param controlPoint2 The bezier control point 2
 * @param end The bezier end
 * @returns The position at the given time
 */
vec2 calcBezier(float t, vec2 start, vec2 controlPoint1, vec2 controlPoint2, vec2 end) {
	float a = 1.0 - t;
	return a*a*a*start + 3.0*a*a*t*controlPoint1 + 3.0*a*t*t*controlPoint2 + t*t*t*end;
}

/**
 * Sorts out special roots
 * NOTE - Translated from https://www.particleincell.com/2013/cubic-line-intersection/
 * based on http://mysite.verizon.net/res148h4j/javascript/script_exact_cubic.html#the%20source%20code
 * @param roots The roots in a vector
 * @returns The sorted roots
 */
vec3 sortSpecial(vec3 roots) {
	bool flip;
	float temp;
	float a[3] = float[](roots.x, roots.y, roots.z);
	
	do {
		flip = false;
		for (int i = 0; i < 2; i++) {
			if (!(a[i+1] >= 0.0 && a[i] > a[i+1]) && !(a[i] < 0.0 && a[i+1] >= 0.0)) continue;
			flip = true;
			temp = a[i];
			a[i] = a[i+1];
			a[i+1] = temp;
		}
	} while (flip);

	return vec3(a[0], a[1], a[2]);
}

/**
 * Finds the cubic roots of a cubic polynomial given the coefficients
 * NOTE - Translated from https://www.particleincell.com/2013/cubic-line-intersection/
 * based on http://mysite.verizon.net/res148h4j/javascript/script_exact_cubic.html#the%20source%20code
 * @param coefficients The coefficients in a vector
 * @returns The roots in a vector
 */
vec3 cubicRoots(vec4 coefficients) {
	float a = coefficients.x;
	float b = coefficients.y;
	float c = coefficients.z;
	float d = coefficients.w;

	vec3 t;

	// Quadratic case
	if (abs(a) < 0.00001) {
		float root = sqrt(c * c - 4.0 * b * d);
		t.x = (-c + root) / (2.0 * b);
		t.y = (-c - root) / (2.0 * b);
		t.z = -1.0;
	}
	
	// Cubic case
	else {
		float A = b / a;
		float B = c / a;
		float C = d / a;

		float Q = (3.0 * B - A * A) / 9.0;
		float R = (9.0 * A * B - 27.0 * C - 2.0 * A * A * A) / 54.0;
		float D = Q * Q * Q + R * R;
		
		// complex or duplicate roots
		if (D >= 0) {
			float S = sign(R + sqrt(D)) * pow(abs(R + sqrt(D)), (1.0 / 3.0));
			float T = sign(R - sqrt(D)) * pow(abs(R - sqrt(D)), (1.0 / 3.0));

			t.x = -A / 3.0 + (S + T); // real root
			t.y = -A / 3.0 - (S + T) / 2.0; // real part of complex root
			t.z = -A / 3.0 - (S + T) / 2.0; // real part of complex root
			float Im = abs(sqrt(3.0) * (S - T) / 2.0); // complex part of root pair
			
			// discard complex roots
			if (Im != 0) {
				t.y = -1;
				t.z = -1;
			}
		}
		// distinct real roots 
		else {
			float th = acos(R / sqrt(-Q*Q*Q));
			t.x = 2.0 * sqrt(-Q) * cos(th / 3.0) - A / 3.0;
			t.y = 2.0 * sqrt(-Q) * cos((th + 2.0 * PI) / 3.0) - A / 3.0;
			t.z = 2.0 * sqrt(-Q) * cos((th + 4.0 * PI) / 3.0) - A / 3.0;
			float Im = 0.0;
		}
	}
	
	// discard out of spec roots
	if (t.x < 0.0 || t.x > 1.0) t.x = -1.0;
	if (t.y < 0.0 || t.y > 1.0) t.y = -1.0;
	if (t.z < 0.0 || t.z > 1.0) t.z = -1.0;
				
	// sort but place -1 at the end
	t = sortSpecial(t);
	return t;
}

/**
 * When dealing with a larger coordinate system such as 1080p in a fragment shader, there is a larger epsilon. When multiplying by
 * extremely large values, this epsilon may cause visual errors, this function rounds off a vector2 to zero if it is within a certain
 * threshold.
 * @param input The input vector
 * @returns The rounded vector
 */
vec2 removeEpsilon(vec2 vec) {
	const float EPSILON = 0.0001;
	return vec2(abs(vec.x) < EPSILON ? 0.0 : vec.x, abs(vec.y) < EPSILON ? 0.0 : vec.y);
}

/**
 * Finds the number of intersections between a cubic bezier curve and a line defined by two points
 * NOTE - Translated from https://www.particleincell.com/2013/cubic-line-intersection/
 * @param bezierStart The start of the bezier curve
 * @param bezierControlPoint1 The first control point of the bezier curve
 * @param bezierControlPoint2 The second control point of the bezier curve
 * @param bezierEnd The end of the bezier curve
 * @param lineStart The start of the line as a vector
 * @param lineEnd The end of the line as a vector
 * @returns The number of intersections between the curve and the line
 */
int computeNumIntersections(vec2 bezierStart, vec2 bezierControlPoint1, vec2 bezierControlPoint2, vec2 bezierEnd, vec2 lineStart, vec2 lineEnd) {
	// A = y2 - y1
	float A = lineEnd.y - lineStart.y;
	// B = x1 - x2
	float B = lineStart.x - lineEnd.x;
	// C = x1 * (y1 - y2) + y1 * (x2 - x1)
	float C = lineStart.x * (lineStart.y - lineEnd.y) + lineStart.y * (lineEnd.x - lineStart.x);

	vec2 bezierCoefficients[4];
	bezierCoefficients[0] = removeEpsilon(-bezierStart + 3.0 * bezierControlPoint1 + -3.0 * bezierControlPoint2 + bezierEnd);
	bezierCoefficients[1] = removeEpsilon(3.0 * bezierStart - 6.0 * bezierControlPoint1 + 3.0 * bezierControlPoint2);
	bezierCoefficients[2] = removeEpsilon(-3.0 * bezierStart + 3.0 * bezierControlPoint1);
	bezierCoefficients[3] = removeEpsilon(bezierStart);

	vec4 CubicCoefficients;
	CubicCoefficients.x = A * bezierCoefficients[0].x + B * bezierCoefficients[0].y;     // t^3
	CubicCoefficients.y = A * bezierCoefficients[1].x + B * bezierCoefficients[1].y;     // t^2
	CubicCoefficients.z = A * bezierCoefficients[2].x + B * bezierCoefficients[2].y;     // t
	CubicCoefficients.w = A * bezierCoefficients[3].x + B * bezierCoefficients[3].y + C; // 1

	vec3 roots = cubicRoots(CubicCoefficients);
	float r[3] = float[](roots.x, roots.y, roots.z);

	int intersections = 0;

	// verify the roots are in bounds of the linear segment
	for (int i = 0; i < 3; i++) {

		// The roots here are the time points of the bezier curve function
		float t = r[i];
		// Plug the time value into the bezier formula to get the collision point
		vec2 collision = bezierCoefficients[0] * t * t * t + bezierCoefficients[1] * t * t + bezierCoefficients[2] * t + bezierCoefficients[3];

		// The above is the intersection point assuming an infinitely long line segment, we need to make sure we are also in the bounds of the line
		float s;
		// Ensure that the line is not pure vertical
		if (lineEnd.x - lineStart.x != 0) s = (collision.x - lineStart.x) / (lineEnd.x - lineStart.x);
		else s = (collision.y - lineStart.y) / (lineEnd.y - lineStart.y);

		// Check that the collision is in the correct bounds
		if (t < 0.0 || t > 1.0 || s < 0.0 || s > 1.0) continue;

		// We found a collision
		intersections++;
	}

	return intersections;
}

/**
 * Helper function to calculate the maximum of 4 float values
 */
float max4(float a, float b, float c, float d) {
	return max(max(max(a, b), c), d);
}

/**
 * Helper function to calculate the minimum of 4 float values
 */
float min4(float a, float b, float c, float d) {
	return min(min(min(a, b), c), d);
}

/**
 * Helper function to find the squared length between two vectors
 */
float squaredLength(vec2 a, vec2 b) {
	return dot(a - b, a - b);
}

/**
 * Checks whether or not this fragment should be stroked relative to a certain bezier
 * @param p0 The bezier start
 * @param p1 The bezier control point 1
 * @param p2 The bezier control point 2
 * @param p3 The bezier end
 * @param p The frag coord
 * @returns Whether or not this fragment should be stroked relative to the bezier
 */
bool shouldStrokeBezier(vec2 p0, vec2 p1, vec2 p2, vec2 p3, vec2 p) {
	// Create a bezier with n segments and check if the distance to any of those segments are less than radius
	const int numIterations = 50;
	vec2 lineStart = p0, lineEnd;
	float squaredRadius = strokeRadius * strokeRadius;

	for (int i = 1; i <= numIterations; i++) {
		lineEnd = calcBezier(float(i) / float(numIterations), p0, p1, p2, p3);

		float squaredDist;
		if (dot(lineStart - lineEnd, p - lineEnd) < 0.0) squaredDist = squaredLength(lineEnd, p);
		else if (dot(lineEnd - lineStart, p - lineStart) < 0.0) squaredDist = squaredLength(lineStart, p);
		else {
			vec2 a = p - lineEnd;
			vec2 b = lineStart - lineEnd;
			squaredDist = squaredLength(lineEnd + b * dot(a, b) / dot(b, b), p);
		}

		if (squaredDist < squaredRadius) return true;
		lineStart = lineEnd;
	}
	return false;
}

/**
 * Entry point
 */
void main() {

	// Calculate whether or not the fragment is in the shape by using even-odd test
	int numCollisions = 0;
	bool shouldStroke = false;
	vec2 lineStart = fragPos;
	vec2 lineEnd = vec2(fragPos.x + 10000000.0, fragPos.y);

	// Loop through the beziers
	for (int i = 0; i < numBeziers; i++) {

		// Fetch the bezier from the uniforms
		vec2 p0 = beziers[4*i];
		vec2 p1 = beziers[4*i + 1];
		vec2 p2 = beziers[4*i + 2];
		vec2 p3 = beziers[4*i + 3];
		float maxX = max4(p0.x, p1.x, p2.x, p3.x), maxY = max4(p0.y, p1.y, p2.y, p3.y),
			minX = min4(p0.x, p1.x, p2.x, p3.x), minY = min4(p0.y, p1.y, p2.y, p3.y);

		// Avoid unnecessary computation as calculating this in a loop is extremely expensive for a fragment shader
		if (fill == 1 && fragPos.x < maxX && fragPos.y > minY && fragPos.y < maxY) {
			// Add the number of computed collisions
			numCollisions += computeNumIntersections(p0, p1, p2, p3, lineStart, lineEnd);
		}

		// Don't continue if we aren't stroking/already found if we're stroking or not
		if (stroke != 0 && !shouldStroke && fragPos.x < maxX+strokeRadius && fragPos.x > minX-strokeRadius &&
			fragPos.y < maxY+strokeRadius && fragPos.y > minY-strokeRadius) {
			if (!shouldStrokeBezier(p0, p1, p2, p3, fragPos)) continue;
			shouldStroke = true;
			if (fill == 0) break;
		}
	}

	// Check whether or not we should fill based on whether the number of collisions is even or odd
	bool shouldFill = (numCollisions & 1) == 1;

	// Determine color based on stroke mode & fill mode
	if (shouldFill && !shouldStroke) outColor = vertexColor;
	else if (shouldStroke) {
		if (shouldFill && stroke != 3) outColor = strokeColor;
		else if (shouldFill && stroke == 3) outColor = vertexColor;
		else if (stroke != 2) outColor = strokeColor;
	}
	else discard;
}
