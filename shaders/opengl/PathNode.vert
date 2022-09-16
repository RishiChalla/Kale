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
// #define MAX_SKELETON_SIZE 128

uniform mat3 camera;
uniform mat3 local;
uniform vec4 vertexColor;
uniform float zPosition;
uniform vec2[MAX_BEZIERS * 4] beziers;
// uniform mat3[128] skeletonTransforms;

in vec2 pos;
in float bezier;
// in float skeletonTransformWeight;
// in float skeletonTransform1;
// in float skeletonTransform2;

out vec4 fragColor;
out vec2 fragPos;
out vec2 bezierStart;
out vec2 bezierControlPoint1;
out vec2 bezierControlPoint2;
out vec2 bezierEnd;
out float hasBezier;

vec2 transform(mat3 mat, vec2 vert) {
	return vec2(
		mat[0][0] * vert.x + mat[0][1] * vert.y + mat[0][2],
		mat[1][0] * vert.x + mat[1][1] * vert.y + mat[1][2]
	);
}

vec2 transformPosition(vec2 pos) {
	return transform(camera, transform(local, pos));
}

void main() {
	// TODO - Skeletal based vertex animation
    gl_Position = vec4(transformPosition(pos), zPosition, 1.0);
	fragPos = gl_Position.xy;
	fragColor = vertexColor;
	int bezierIndex = int(bezier) * 4;
	if (bezierIndex < 0) {
		hasBezier = -1.0;
	}
	else {
		hasBezier = 1.0;
		bezierStart = transformPosition(beziers[bezierIndex]);
		bezierControlPoint1 = transformPosition(beziers[bezierIndex + 1]);
		bezierControlPoint2 = transformPosition(beziers[bezierIndex + 2]);
		bezierEnd = transformPosition(beziers[bezierIndex + 3]);
	}
}
