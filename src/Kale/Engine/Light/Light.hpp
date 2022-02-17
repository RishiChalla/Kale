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

#include <Kale/Math/Transform/Transform.hpp>

#include <list>
#include <memory>

namespace Kale {

	/**
	 * Forward declaration of node class
	 */
	class Node;

	/**
	 * The movement of light, static lights are stationary and do not move. This allows for precalculating of lighting and shading.
	 * Dynamic lights may move around and cause the shading of nodes around them to update. This results in lightning being calculated every frame.
	 */
	enum class LightMovement {
		Static, Dynamic
	};

	/**
	 * The type of shading this light uses. Lightening means that exposed areas of nodes towards the light will be lightened in color.
	 * Darkening means the areas to the opposite of the light within a node will be darkened.
	 */
	enum class LightShading {
		Lightening, Darkening, None
	};

	/**
	 * The type of emission to use for this light. Gradient lighting effects use lighting effects similar to godrays. Blocks uses lighting similar
	 * to godrays except with block based color transitions instead of gradients.
	 */
	enum class LightEmission {
		Gradient, Blocks, None
	};

	/**
	 * The main Light class, all other light emitting entities are inherited from this class
	 */
	class Light {
	private:

	protected:

		/**
		 * The type of light movement this uses
		 */
		LightMovement movement;

		/**
		 * The type of light shading this uses
		 */
		LightShading shading;

		/**
		 * The type of light emission this uses
		 */
		LightEmission emission;

		/**
		 * Whether or not to use bloom effects for this light
		 */
		bool bloom;

		/**
		 * Renders the light, will only be called from the main thread
		 * @param camera The camera to render with
		 */
		virtual void render(const Camera& camera, const std::list<std::shared_ptr<Node>>& nodes) const = 0;

	public:

		/**
		 * Creates a new light given its attributes
		 * @param movement The movement frequency of the light
		 * @param shading The shading type of the light
		 * @param emission The emission type of the light
		 * @param bloom Whether or not to use bloom/blur effects in this light
		 */
		Light(LightMovement movement = LightMovement::Static, LightShading shading = LightShading::Lightening,
			LightEmission emission = LightEmission::Gradient, bool bloom = true);

	};
}
