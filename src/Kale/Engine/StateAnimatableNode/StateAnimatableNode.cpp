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

#include "StateAnimatableNode.hpp"

using namespace Kale;

/**
 * Called prior to update, perfect place to do things such as updating the bounding box, etc
 * @param threadNum the index of the thread this update is called on
 * @param scene The scene being updated to
 * @param deltaTime The duration of the last frame in microseconds
 */
void StateAnimatableNode::preUpdate(size_t threadNum, const Scene& scene, float deltaTime) {
	std::lock_guard lock(mutex);
	if (!transitioning) return;
	transitionTime += deltaTime / (1000000.0f * transitionDuration);

	if (transitionTime >= 1.0f) {
		transitioning = false;
		state = transitionState;
	}
}

/**
 * Returns whether or not the animation is transitioning between two states.
 * Thread safe if called in update. If called in pre-update, there may be a data race as to whether or not the animation has
 * been updated for this frame.
 * @returns Whether or not the animation is transitioning between two states
 */
bool StateAnimatableNode::isTransitioning() const {
	return transitioning;
}
