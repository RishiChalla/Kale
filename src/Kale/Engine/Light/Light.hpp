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
	struct Light {

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
		 * The color of the light
		 */
		Vector4f color = {1.0f, 1.0f, 1.0f, 1.0f};

		/**
		 * Gets bounding rays of the light given the rect
		 * @param boundingBox
		 * @returns The rays which bound the light's path
		 */
		virtual std::pair<Ray, Ray> getBoundingRays(Rect boundingBox) const = 0;

		/**
		 * Renders the light
		 */
		virtual void render() const = 0;

	};
}
