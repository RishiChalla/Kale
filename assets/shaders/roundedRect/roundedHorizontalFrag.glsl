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

// #version 420 core

uniform vec2 center1;
uniform vec2 center2;

void main() {
	// if (gl_FragCoord.x < center1.x && distance(center1, gl_FragCoord.xy) > 50.0) {
	// 	gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
	// }
	// else if (gl_FragCoord.x > center2.x && distance(center2, gl_FragCoord.xy) > 50.0) {
	// 	gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
	// }
	// else {
		gl_FragColor = gl_Color;
	// }
}