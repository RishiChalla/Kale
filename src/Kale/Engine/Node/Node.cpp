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

#include "Node.hpp"

#include <Kale/Core/Scene/Scene.hpp>

#include <utility>

using namespace Kale;

/**
 * Calculates lighting for specific coordinates given the 
 * @param pos The position of the colored entity
 * @param color The color of the entity
 * @param scene The scene being rendered to
 * @returns The shaded color
 */
Color Node::calculateLighting(Vector2f pos, Color color, const Scene& scene) const {
	// Lighting formula:
	// [Light Intensity * (Light Color * Light Color Intensity + Color * (1 - Light Color Intensity))]

	// Apply ambient lighting
	std::pair<Color, float> ambient = scene.getAmbient();
	color = Color(ambient.second * (Vector3f(ambient.first.xyz() * ambient.first.w) + Vector3f(color.xyz() * (1.0f - ambient.first.w))), color.w);
	
	// Apply lighting from scene lights
	for (const std::shared_ptr<Light>& light : scene.getLights()) {

		// Skip light if not applicable
		float intensity = light->getIntensity(pos);
		if (intensity == -1.0f) continue;

		// Calculate lighting
		color = Color(intensity * (Vector3f(light->color.xyz() * light->color.w) + Vector3f(color.xyz() * (1.0f - light->color.w))), color.w);
	}
	
	return color;
}
