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

#define EPSILON float(1e-8)

in vec4 fragColor;
in vec2 fragPos;
in vec2 bezierStart;
in vec2 bezierControlPoint1;
in vec2 bezierControlPoint2;
in vec2 bezierEnd;
in float hasBezier;

out vec4 outColor;

/**
 * Counts the number of roots in a cubic equation in the form ax^3 + bx^2 + cx + d = 0
 * Translated from https://stackoverflow.com/questions/27176423/function-to-solve-cubic-equation-analytically
 */
int countCubicRoots(float a, float b, float c, float d) {

	// Quadratic case, ax^2+bx+c=0
	if (abs(a) < EPSILON) {
		a = b; b = c; c = d;
		
		// Linear case, ax+b=0
		if (abs(a) < EPSILON) {
			a = b; b = c;

			// Degenerate case
			if (abs(a) < EPSILON) return 0;
			
			// Linear case, ax+b=0
			return 1;
		}

		// Quadratic case, ax^2+bx+c=0
		float D = b*b - 4.0*a*c;
		if (abs(D) < EPSILON) return 1;
		else if (D > 0) return 2;
		return 0;
	}

	// Convert to depressed cubic t^3+pt+q = 0 (subst x = t - b/3a)
	float p = (3.0*a*c - b*b)/(3.0*a*a);
	float q = (2.0*b*b*b - 9.0*a*b*c + 27.0*a*a*d)/(27.0*a*a*a);

	// p = 0 -> t^3 = -q -> t = -q^1/3
	if (abs(p) < EPSILON) return 1;
	// q = 0 -> t^3 + pt = 0 -> t(t^2+p)=0
	else if (abs(q) < EPSILON) {
		if (p < 0) return 3;
		else return 1;
	}
	else {
		float D = q*q/4.0 + p*p*p/27.0;

		// D = 0 -> two roots
		if (abs(D) < EPSILON) return 2;
		// Only one real root
		else if (D > 0) return 1;
		// D < 0, three roots, but needs to use complex numbers/trigonometric solution
		else return 3;
	}
}

void main() {

	// We only want to run collision detection when there actually is a bezier
	if (hasBezier != 0.0) {
		outColor = fragColor;
		return;
	}

	// Cubic Bezier line intersection based on the algorithm presented at https://www.particleincell.com/2013/cubic-line-intersection/

	// Create line perpendicular to the line created from the start - end of the bezier path, and ensure it passes through the frag coord
	vec2 p1 = bezierEnd - bezierStart;
	p1 = vec2(-p1.y, p1.x) + fragPos;
	vec2 p2 = fragPos;

	// Convert the line into the form Ax + By + C = 0
	float A = (p2.y - p1.y);
	float B = (p1.x - p2.x);
	float C = p1.x * (p1.y - p2.y) + p1.y * (p2.x - p1.x);

	// Substitute the cubic bezier formula into the line formula then merge to create
	// (1-T)^3a + (1-T)^2Tb + (1-T)T^2c + T^3d + C = 0
	// Where a, b, c, d are defined as:
	float a = A*bezierStart.x + B*bezierStart.y;
	float b = 3.0*A*bezierControlPoint1.x + 3.0*B*bezierControlPoint1.y;
	float c = 3.0*A*bezierControlPoint2.x + 3.0*B*bezierControlPoint2.y;
	float d = A*bezierEnd.x + B*bezierEnd.y;

	// Convert the substition equation to eT^3 + fT^2 + gT + h = 0 where e, f, g, h are defined as:
	float e = -a + b - c + d;
	float f = 3.0*a - 2.0*b + c;
	float g = -3.0*a + b;
	float h = a + C;

	// Solve using cubic formula:
	int numCubicRoots = countCubicRoots(e, f, g, h);

	// There are an odd number of roots, this fragment should be shaded
	if ((numCubicRoots & 1) == 1) outColor = fragColor;
	// There are an even number of roots, this fragment shouldn't be shaded
	else outColor = vec4(0.0, 0.0, 0.0, 0.0);
}
