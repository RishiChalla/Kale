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

#include "Light.hpp"

using namespace Kale;

/**
 * Creates a new light given its attributes
 * @param color The color of the light
 * @param shading The shading type of the light
 * @param emission The emission type of the light
 * @param bloom Whether or not to use bloom/blur effects in this light
 */
Light::Light(Vector4f color, LightShading shading, LightEmission emission, bool bloom) : color(color), shading(shading), emission(emission), bloom(bloom) {
	// Empty Body
}
