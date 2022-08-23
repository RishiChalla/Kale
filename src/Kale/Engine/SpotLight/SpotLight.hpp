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
#include <Kale/Math/Utils/Utils.hpp>
#include <Kale/Math/Transform/Transform.hpp>

namespace Kale {

	/**
	 * Represents a light originating from a point within two rays
	 */
	class SpotLight : public Light {
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
		 * This light's transform
		 */
		Transform transform;

		/**
		 * The radius of the light
		 */
		float radius = 1.0f;

		/**
		 * The intensity of the light
		 */
		float intensity = 100.0f;

		/**
		 * The start ray unit vector
		 */
		Vector2f start = Vector2f(0.5f, 0.5f).unit();

		/**
		 * The end ray unit vector
		 */
		Vector2f end = Vector2f(-0.5f, 0.5f).unit();

		/**
		 * Creates an empty spot light object
		 */
		SpotLight();

		/**
		 * Creates a white spot light
		 * @param position The position of the spot light
		 * @param radius The radius of the spot light
		 * @param intensity The intensity of the spot light
		 */
		SpotLight(Vector2f position, float radius, float intensity);
		
		/**
		 * Creates a white spot light
		 * @param position The position of the spot light
		 * @param radius The radius of the spot light
		 * @param intensity The intensity of the spot light
		 * @param start The start ray unit vector of the light
		 * @param end The end ray unit vector of the light
		 */
		SpotLight(Vector2f position, float radius, float intensity, Vector2f start, Vector2f end);
		
		/**
		 * Creates a white spot light
		 * @param position The position of the spot light
		 * @param radius The radius of the spot light
		 * @param intensity The intensity of the spot light
		 * @param start The starting angle of the light
		 * @param end The ending angle of the light
		 * @param unit The unit of the start and end angles
		 */
		SpotLight(Vector2f position, float radius, float intensity, float start, float end, AngleUnit unit);
		
		/**
		 * Creates a spot light
		 * @param position The position of the spot light
		 * @param radius The radius of the spot light
		 * @param intensity The intensity of the spot light
		 * @param start The start ray unit vector of the light
		 * @param end The end ray unit vector of the light
		 * @param color The color of the light
		 */
		SpotLight(Vector2f position, float radius, float intensity, Vector2f start, Vector2f end, Color color);
		
		/**
		 * Creates a spot light
		 * @param position The position of the spot light
		 * @param radius The radius of the spot light
		 * @param intensity The intensity of the spot light
		 * @param start The starting angle of the light
		 * @param end The ending angle of the light
		 * @param unit The unit of the start and end angles
		 * @param color The color of the light
		 */
		SpotLight(Vector2f position, float radius, float intensity, float start, float end, AngleUnit unit, Color color);

	};
}
