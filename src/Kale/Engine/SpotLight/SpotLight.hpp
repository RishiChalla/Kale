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

namespace Kale {

	/**
	 * Represents a light which originates from a single point within bounds, ie a cone shape
	 */
	class SpotLight : public Light {
	protected:

		/**
		 * Called prior to update, perfect place to do things such as updating the bounding box, etc
		 * @param deltaTime The amount of microseconds since the last update
		 * @param scene The scene being updated to
		 */
		void preUpdate(float deltaTime, const Scene& scene) override;

		/**
		 * Renders the node
		 * @param camera The camera to render with
		 * @param scene The scene being rendered on
		 */
		void render(const Camera& camera, const Scene& scene) const override;

	public:

		/**
		 * Gets the intensity of the light at a certain position. Returns -1 if light does not affect the position
		 * @param pos The position to get the intensity at
		 * @returns The light intensity
		 */
		float getIntensity(Vector2f pos) const override;

	};
}
