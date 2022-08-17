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
	 * Represents a light affecting the entire scene from a specific direction
	 */
	class DirectionalLight : public Light {
	protected:

		/**
		 * Renders the node
		 * @param camera The camera to render with
		 * @param scene The scene being rendered on
		 */
		void render(const Camera& camera, const Scene& scene) const override;

	public:

	};
}
