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

#pragma once

namespace Kale {

	/**
	 * Converts an angle in degrees to radians
	 * @param deg The angle in degrees
	 * @returns The angle in radians
	 */
	float degToRad(float deg);

	/**
	 * Converts an angle in radians to degrees
	 * @param rad The angle in radians
	 * @returns The angle in degrees
	 */
	float radToDeg(float rad);

}
