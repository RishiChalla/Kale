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

#include <Kale/Engine/Light/Light.hpp>
#include <Kale/Math/Transform/Transform.hpp>

namespace Kale {

	/**
	 * Represents a light originating from a point in all directions
	 */
	class PointLight : public Light {
	protected:

		/**
		 * Renders the node
		 * @param scene The scene being rendered on
		 */
		void render(const Scene& scene) const override;

	public:

		/**
		 * Gets the clipping mask to use when shading
		 * @returns The clipping mask
		 */
		SkPath getShadingMask() const override;

		/**
		 * This light's position
		 */
		Vector2f position;

		/**
		 * The radius of the light
		 */
		float radius = 1.0f;

		/**
		 * The intensity of the light
		 */
		float intensity = 100.0f;

		/**
		 * Creates an empty point light object
		 */
		PointLight();

		/**
		 * Creates a white point light at a position given the radius
		 * @param position The position of the light
		 * @param radius The radius of the light
		 * @param intensity The intensity of the light
		 */
		PointLight(Vector2f position, float radius, float intensity);

		/**
		 * Creates a point light
		 * @param position The position of the light
		 * @param radius The radius of the light
		 * @param intensity The intensity of the light
		 * @param color The color of the light
		 */
		PointLight(Vector2f position, float radius, float intensity, Color color);

	};
}
