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

// Using cubic line intersection algorithm to perform even-odd polygon testing
// Line intersection translated from
// https://www.particleincell.com/2013/cubic-line-intersection/

#define MAX_BEZIERS 128
#define PI 3.1415926538

uniform vec4 vertexColor;
uniform vec2[MAX_BEZIERS * 4] beziers;
uniform int numBeziers;

in vec2 fragPos;

out vec4 outColor;

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
 * Entry point
 */
void main() {

	// Calculate whether or not the fragment is in the shape by using even-odd test
	int numCollisions = 0;
	vec2 lineStart = fragPos;
	vec2 lineEnd = vec2(fragPos.x + 10000000.0, fragPos.y);

	// Loop through the beziers
	for (int i = 0; i < numBeziers; i++) {

		// Fetch the bezier from the uniforms
		vec2 bezierStart = beziers[4*i];
		vec2 bezierControlPoint1 = beziers[4*i + 1];
		vec2 bezierControlPoint2 = beziers[4*i + 2];
		vec2 bezierEnd = beziers[4*i + 3];

		// Avoid unnecessary computation as calculating this in a loop is extremely expensive for a fragment shader
		if (fragPos.x > max4(bezierStart.x, bezierControlPoint1.x, bezierControlPoint2.x, bezierEnd.x) ||
			fragPos.y < min4(bezierStart.y, bezierControlPoint1.y, bezierControlPoint2.y, bezierEnd.y) ||
			fragPos.y > max4(bezierStart.y, bezierControlPoint1.y, bezierControlPoint2.y, bezierEnd.y))
			continue;

		// Add the number of computed collisions
		numCollisions += computeNumIntersections(bezierStart, bezierControlPoint1, bezierControlPoint2, bezierEnd, lineStart, lineEnd);
	}

	// Odd number of collisions, this fragment is inside the path
	if ((numCollisions & 1) == 1) outColor = vertexColor;
	// Even number of collisions, this fragment is outside the path
	else outColor = vec4(0.0, 0.0, 0.0, 0.0);
}
