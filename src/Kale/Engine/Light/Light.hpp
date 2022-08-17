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

namespace Kale {

	/**
	 * The main Light class, all other light emitting entities are inherited from this class
	 */
	class Light : public Node {
	public:

		/**
		 * Whether or not to apply border shading on nodes from this light
		 */
		bool applyShading = false;

		/**
		 * Whether or not to use bloom effects for this light
		 */
		bool bloom = true;

		/**
		 * The color of the light, alpha is color intensity
		 */
		Vector4f color = Color(0xffffff);

	};
}
