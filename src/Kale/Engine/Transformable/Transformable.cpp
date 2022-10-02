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

#include "Transformable.hpp"

using namespace Kale;

/**
 * Updates the transform depending on state node animations
 * @param deltaTime The duration of the last frame in microseconds
 */
void Transformable::updateTransform(float deltaTime) {
	// Update the Transform based on the FSM if applicable
	if (!transformFSM.has_value()) return;

	// Update the state
	transformFSM->updateState(deltaTime);

	// Clear the transform
	stateTransform = Transform({
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	});

	// Loop through the composition & lerp between states as applicable
	for (std::pair<int, float> composition : transformFSM->getStateComposition<int>())
		*stateTransform += transformFSM->getStructure<int>(composition.first) * composition.second;
}

/**
 * Gets the full transform including FSM based transformations
 * @returns the full transform including FSM based transformations
 */
Transform Transformable::getFullTransform() const {
	if (!transformFSM.has_value()) return transform;
	return transform * (*stateTransform);
}
