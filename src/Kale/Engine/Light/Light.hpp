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

#include <Kale/Math/Ray/Ray.hpp>
#include <Kale/Engine/Node/Node.hpp>

#include <utility>

namespace Kale {

	/**
	 * The type of shading this light uses. Lightening means that exposed areas of nodes towards the light will be lightened in color.
	 * Darkening means the areas to the opposite of the light within a node will be darkened.
	 */
	enum class LightShading {
		Lightening, Darkening, Both, None
	};

	/**
	 * The type of emission to use for this light. Gradient lighting effects use lighting effects similar to god-rays. Blocks uses lighting similar
	 * to god-rays except with block based color transitions instead of gradients.
	 */
	enum class LightEmission {
		Gradient, Blocks, None
	};

	/**
	 * The main Light class, all other light emitting entities are inherited from this class
	 */
	class Light : public Node {
	public:

		/**
		 * The z position/depth of the node. Lower values are drawn first, higher values are placed above lower values.
		 */
		float zPosition = 0.0f;

		/**
		 * The type of light shading this uses
		 */
		LightShading shading = LightShading::Both;

		/**
		 * The type of light emission this uses
		 */
		LightEmission emission = LightEmission::Gradient;

		/**
		 * Whether or not to use bloom effects for this light
		 */
		bool bloom = true;

		/**
		 * The color of the light, alpha is color intensity
		 */
		Vector4f color = Color(0xffffff);

		/**
		 * The intensity of the light
		 */
		float intensity;

		/**
		 * Gets the intensity of the light at a certain position. Returns -1 if light does not affect the position
		 * @param pos The position to get the intensity at
		 * @returns The light intensity
		 */
		virtual float getIntensity(Vector2f pos) = 0;

	};
}
