/*
   Copyright 2021 Rishi Challa

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

#define PI 3.1415926535897932384626433832795

uniform vec2 center1;
uniform vec2 center2;
uniform float threshold;

void main() {
	float angle = atan(center1.y - center2.y, center1.x - center2.x) + PI;
	float angle1 = atan(gl_FragCoord.y - center1.y, gl_FragCoord.x - center1.x) + PI;

	// if (angle1 - angle < 0 || angle1 - angle > 2.0 * PI) {
	// 	float dist = distance(center1, gl_FragCoord.xy);
	// 	if (dist >= threshold - 1.0) {
	// 		gl_FragColor = vec4(gl_Color.xyz, clamp(threshold - dist, 0.0, 1.0));
	// 		return;
	// 	}
	// }
	
	gl_FragColor = vec4(1.0, 0.0, 0.0, (angle1 - angle + 2.0 * PI) / (4.0 * PI));
	gl_FragColor = gl_Color;
}