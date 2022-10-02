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
#include <Kale/Engine/Node/Node.hpp>
#include <Kale/Engine/StateAnimatable/StateAnimatable.hpp>

#include <mutex>
#include <optional>

namespace Kale {

	/**
	 * All transformable nodes must inherit from this class. Inheritance from this class is required for synchronized access to
	 * transformation matrices across threads & for usage from the animation system.
	 */
	class Transformable {
	protected:

		/**
		 * Updates the transform depending on state node animations
		 * @param deltaTime The duration of the last frame in microseconds
		 */
		void updateTransform(float deltaTime);

		/**
		 * The transform produced from the FSM
		 */
		std::optional<Transform> stateTransform;
	
	public:

		/**
		 * The FSM used for transform states. Transformables support FSM based animations, to use the FSM simply set the value of this optional and fill
		 * the map. Access to the FSM must be externally synchronized if done from multiple threads.
		 */
		std::optional<StateAnimatable<Transform>> transformFSM;

		/**
		 * The transformation matrix of this object
		 */
		Transform transform;

		/**
		 * Gets the full transform including FSM based transformations
		 * @returns the full transform including FSM based transformations
		 */
		Transform getFullTransform() const;

	};
}
