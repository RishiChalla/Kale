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

#include <Kale/Math/Vector/Vector.hpp>
#include <Kale/Math/Transform/Transform.hpp>

#include <list>
#include <memory>

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
	class Light {
	private:

	protected:

		/**
		 * The z position/depth of the node. Lower values are drawn first, higher values are placed above lower values.
		 */
		float zPosition = 0.0f;

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
		 * The color of the light
		 */
		Vector4f color;

		/**
		 * Renders the light, will only be called from the main thread. This only renders the emission and bloom, shading must be called from nodes.
		 * @param camera The camera to render with
		 */
		virtual void render(const Camera& camera) const = 0;

		/**
		 * Checks whether the light affects the view within a camera
		 * @param camera The camera to check within
		 * @returns Whether or not the light affects the view within the camera
		 */
		virtual bool isInView(const Camera& camera) const = 0;

		friend class Scene;

	public:

		/**
		 * Creates a new light given its attributes
		 * @param color The color of the light
		 * @param shading The shading type of the light
		 * @param emission The emission type of the light
		 * @param bloom Whether or not to use bloom/blur effects in this light
		 */
		Light(Vector4f color = {1.0f, 1.0f, 1.0f, 1.0f}, LightShading shading = LightShading::Lightening, LightEmission emission = LightEmission::Gradient,
				bool bloom = true);

		/**
		 * Draws the shading for a node given the camera and the node geometry. Must be called only from the main thread.
		 * @param camera The camera object
		 * @param geometry The geometry of the node
		 */
		virtual void shadeNode(const Camera& camera, const Geometry& geometry) = 0;

	};
}
