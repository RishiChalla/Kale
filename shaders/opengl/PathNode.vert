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

uniform mat3 camera;
uniform mat3 local;
uniform float zPosition;

in vec2 pos;

out vec2 fragPos;

/**
 * Helper function to transform a vector by a transformation matrix
 */
vec2 transform(mat3 mat, vec2 vert) {
	return vec2(
		mat[0][0] * vert.x + mat[0][1] * vert.y + mat[0][2],
		mat[1][0] * vert.x + mat[1][1] * vert.y + mat[1][2]
	);
}

/**
 * Transforms a position vector by the camera and local transforms
 */
vec2 transformPosition(vec2 pos) {
	return transform(camera, transform(local, pos));
}

/**
 * Entry point
 */
void main() {
	gl_Position = vec4(transformPosition(pos), zPosition, 1.0);
	fragPos = pos;
}
