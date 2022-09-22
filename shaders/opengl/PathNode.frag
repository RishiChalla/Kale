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

float j, k, l, m, n, o;

float d(float x) {
	return 6.0*j*(x*x*x*x*x) + 5.0*k*(x*x*x*x) + 4.0*l*(x*x*x) + 3.0*m*(x*x) + 2.0*n*(x) + o;
}

float d2(float x) {
	return 30.0*j*(x*x*x*x) + 20.0*k*(x*x*x) + 12.0*l*(x*x) + 6.0*m*(x) + 2.0*n;
}

/**
 * Uses the newton raphson method to find an approximation for the roots of function f with derivative d
 * @param x The initial guess of the root
 * @returns The root or -1 if a root is not found in 10 iterations
 */
float newtonApproximation(float x) {
	const float EPSILON = 0.001;
	float h = d(x) / d2(x);
	int count = 0;
	while (abs(h) >= EPSILON) {
		h = d(x) / d2(x);
		x = x - h;
		count++;
		if (count >= 10) return -1;
	}

	return x;
}

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
	
	float A = b / a;
	float B = c / a;
	float C = d / a;

	float Q = (3.0 * B - A * A) / 9.0;
	float R = (9.0 * A * B - 27.0 * C - 2.0 * A * A * A) / 54.0;
	float D = Q * Q * Q + R * R;

	vec3 t;
	
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
	
	// discard out of spec roots
	if (t.x < 0.0 || t.x > 1.0) t.x = -1.0;
	if (t.y < 0.0 || t.y > 1.0) t.y = -1.0;
	if (t.z < 0.0 || t.z > 1.0) t.z = -1.0;
				
	// sort but place -1 at the end
	t = sortSpecial(t);
	return t;
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
	float C = lineStart.x * (lineStart.y - lineEnd.y) +  lineStart.y * (lineEnd.x - lineStart.x);

	vec2 bezierCoefficients[4];
	bezierCoefficients[0] = -bezierStart + 3.0 * bezierControlPoint1 + -3.0 * bezierControlPoint2 + bezierEnd; 
	bezierCoefficients[1] = 3.0 * bezierStart - 6.0 * bezierControlPoint1 + 3.0 * bezierControlPoint2;
	bezierCoefficients[2] = -3.0 * bezierStart + 3.0 * bezierControlPoint1;
	bezierCoefficients[3] = bezierStart;

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
 * Checks whether or not this fragment should be stroked relative to a certain bezier
 * @param p0 The bezier start
 * @param p1 The bezier control point 1
 * @param p2 The bezier control point 2
 * @param p3 The bezier end
 * @param p The frag coord
 * @returns Whether or not this fragment should be stroked relative to the bezier
 */
bool shouldStrokeBezier(vec2 p0, vec2 p1, vec2 p2, vec2 p3, vec2 p) {
	// B(t) = (1-t)^3*p0 + 3t(1-t)^2*p1 + 3t^2(1-t)*p2 + t^3*p3 // Bezier formula
	// B'(t) = 3(1-t)^2(p1 - p0) + 6(1-t)t(p2 - p1) + 3t^2(p3 - p2) // Bezier derivative
	
	// D(t) = (x - B(t)x)^2 + (y - B(t)y)^2 // Squared distance between position and B(t)

	// Substitute
	// (x - ((1-t)^3*p0x + 3t(1-t)^2*p1x + 3t^2(1-t)*p2x + t^3*p3x))^2 + (y - ((1-t)^3*p0y + 3t(1-t)^2*p1y + 3t^2(1-t)*p2y + t^3*p3y))^2

	// Map p0x = a, p0y = b, p1x = c, p1y = d, p2x = f, p2y = g, p3x = h, p3y = i ->
	// (x - ((1-t)^3*a + 3t(1-t)^2*c + 3t^2(1-t)*f + t^3*h))^2 + (y - ((1-t)^3*b + 3t(1-t)^2*d + 3t^2(1-t)*g + t^3*i))^2

	// Simplify ->
	// jt^6 + kt^5 + lt^4 + mt^3 + nt^2 + ot + q = 0
	// j = a^2 - 6ac + 6af - 2ah + b^2 - 6bd + 6bg - 2bi + 9c^2 - 18cf + 6ch + 9d^2 - 18dg + 6di + 9f^2 - 6fh + 9g^2 - 6gi + h^2 + i^2
	// k = -6a^2 + 30ac - 24af + 6ah - 6b^2 + 30bd - 24bg + 6bi - 36c^2 + 54cf - 12ch - 36d^2 + 54dg - 12di - 18f^2 + 6fh - 18g^2 + 6gi
	// l = 15a^2 - 60ac + 36af - 6ah + 15b^2 - 60bd + 36bg - 6bi + 54c^2 - 54cf + 6ch + 54d^2 - 54dg + 6di + 9f^2 + 9g^2
	// m = -20a^2 + 60ac - 24af + 2ah + 2ax - 20b^2 + 60bd - 24bg + 2bi + 2by - 36c^2 + 18cf - 6cx - 36d^2 + 18dg - 6dy + 6fx + 6gy - 2hx - 2iy
	// n = 15a^2 - 30ac + 6af - 6ax + 15b^2 - 30bd + 6bg - 6by + 9c^2 + 12cx + 9d^2 + 12dy - 6fx - 6gy
	// o = -6a^2 + 6ac + 6ax - 6b^2 + 6bd + 6by - 6cx - 6dy
	// p = a^2 - 2ax + b^2 - 2by + x^2 + y^2

	j = p0.x*p0.x - 6.0*p0.x*p1.x + 6.0*p0.x*p2.x - 2.0*p0.x*p3.x + p0.y*p0.y - 6.0*p0.y*p1.y + 6.0*p0.y*p2.y - 2.0*p0.y*p3.y +
		9*p1.x*p1.x - 18.0*p1.x*p2.x + 6.0*p1.x*p3.x + 9.0*p1.y*p1.y - 18.0*p1.y*p2.y + 6.0*p1.y*p3.y + 9.0*p2.x*p2.x -
		6*p2.x*p3.x + 9.0*p2.y*p2.y - 6.0*p2.y*p3.y + p3.x*p3.x + p3.y*p3.y;
	k = -6*p0.x*p0.x + 30.0*p0.x*p1.x - 24.0*p0.x*p2.x + 6.0*p0.x*p3.x - 6.0*p0.y*p0.y + 30.0*p0.y*p1.y - 24.0*p0.y*p2.y + 6.0*p0.y*p3.y -
		36*p1.x*p1.x + 54.0*p1.x*p2.x - 12.0*p1.x*p3.x - 36.0*p1.y*p1.y + 54.0*p1.y*p2.y - 12.0*p1.y*p3.y - 18.0*p2.x*p2.x +
		6*p2.x*p3.x - 18.0*p2.y*p2.y + 6.0*p2.y*p3.y;
	l = 15.0*p0.x*p0.x - 60.0*p0.x*p1.x + 36.0*p0.x*p2.x - 6.0*p0.x*p3.x + 15.0*p0.y*p0.y - 60.0*p0.y*p1.y + 36.0*p0.y*p2.y - 6.0*p0.y*p3.y +
		54*p1.x*p1.x - 54.0*p1.x*p2.x + 6.0*p1.x*p3.x + 54.0*p1.y*p1.y - 54.0*p1.y*p2.y + 6.0*p1.y*p3.y + 9.0*p2.x*p2.x + 9.0*p2.y*p2.y;
	m = -20*p0.x*p0.x + 60.0*p0.x*p1.x - 24.0*p0.x*p2.x + 2.0*p0.x*p3.x + 2.0*p0.x*p.x - 20.0*p0.y*p0.y + 60.0*p0.y*p1.y - 24.0*p0.y*p2.y +
		2*p0.y*p3.y + 2.0*p0.y*p.y - 36.0*p1.x*p1.x + 18.0*p1.x*p2.x - 6.0*p1.x*p.x - 36.0*p1.y*p1.y + 18.0*p1.y*p2.y - 6.0*p1.y*p.y + 6.0*p2.x*p.x +
		6*p2.y*p.y - 2.0*p3.x*p.x - 2.0*p3.y*p.y;
	n = 15.0*p0.x*p0.x - 30.0*p0.x*p1.x + 6.0*p0.x*p2.x - 6.0*p0.x*p.x + 15.0*p0.y*p0.y - 30.0*p0.y*p1.y + 6.0*p0.y*p2.y - 6.0*p0.y*p.y +
		9*p1.x*p1.x + 12.0*p1.x*p.x + 9.0*p1.y*p1.y + 12.0*p1.y*p.y - 6.0*p2.x*p.x - 6.0*p2.y*p.y;
	o = -6*p0.x*p0.x + 6.0*p0.x*p1.x + 6.0*p0.x*p.x - 6.0*p0.y*p0.y + 6.0*p0.y*p1.y + 6.0*p0.y*p.y - 6.0*p1.x*p.x - 6.0*p1.y*p.y;

	float root = newtonApproximation(0.5);
	if (root > 1.0 || root < 0.0 || d2(root) < 0.0) return false; // No local minima found

	vec2 d = calcBezier(root, p0, p1, p2, p3) - p;
	if (dot(d, d) < strokeRadius * strokeRadius) return true;

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
		else outColor = strokeColor;
	}
	else outColor = vec4(0.0, 0.0, 0.0, 0.0);
}
