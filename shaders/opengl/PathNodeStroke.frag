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

uniform vec4 vertexColor;
uniform vec2[MAX_BEZIERS * 4] beziers;
uniform int numBeziers;
uniform float strokeRadius;

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
 * Entry point
 */
void main() {

	// Using an algorithm to minimize the distance to the bezier curve and see if we're close enough to stroke
	vec2 p = fragPos;

	// Iterate through all the beziers
	for (int i = 0; i < numBeziers; i++) {
		
		// Get the bezier points
		vec2 p0 = beziers[4*i];
		vec2 p1 = beziers[4*i + 1];
		vec2 p2 = beziers[4*i + 2];
		vec2 p3 = beziers[4*i + 3];

		// Conducting the below calculations can be expensive, let's use bounding boxes to avoid it if possible
		if (p.x > max4(p0.x, p1.x, p2.x, p3.x)+strokeRadius || p.x < min4(p0.x, p1.x, p2.x, p3.x)-strokeRadius ||
			p.y > max4(p0.y, p1.y, p2.y, p3.y)+strokeRadius || p.y < min4(p0.y, p1.y, p2.y, p3.y)-strokeRadius) continue;
		
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
		if (root > 1.0 || root < 0.0 || d2(root) < 0.0) continue; // No local minima found

		vec2 d = calcBezier(root, p0, p1, p2, p3) - p;
		if (dot(d, d) < strokeRadius * strokeRadius) {
			outColor = vertexColor;
			return;
		}
	}

	// This fragment isn't in the stroke radius
	outColor = vec4(0.0, 0.0, 0.0, 0.0);
}
