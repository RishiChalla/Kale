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

uniform vec2 center1;
uniform vec2 center2;
uniform float threshold;

void main() {
	float dist1 = distance(center1, gl_FragCoord.xy);
	float dist2 = distance(center2, gl_FragCoord.xy);

	if (dist1 > threshold && dist2 > threshold) {
		gl_FragColor = vec4(gl_Color.xyz, 0.0);
		return;
	}

	gl_FragColor = gl_Color;
}