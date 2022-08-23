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

#include <Kale/Engine/Node/Node.hpp>

#include <unordered_map>
#include <vector>
#include <utility>

namespace Kale {

	/**
	 * All animatable nodes which follow the animation system inherit from this class.
	 * This class allows for nodes to have animation states and to lerp between them
	 * @tparam An enum class containing all the animation states
	 * @tparam A structure holding the data for each state
	 */
	template <typename T, typename S>
	class AnimatableNode : public Node {
	protected:

		/**
		 * The path state currently being rendered
		 */
		T state;

		/**
		 * The state currently being transitioned to
		 */
		T transitionState;

		/**
		 * Whether or not the path is morphing between two paths
		 */
		bool transitioning;

		/**
		 * The transition time between the two paths
		 */
		float transitionTime;

		/**
		 * The duration in seconds for the transition to last
		 */
		float transitionDuration;

		/**
		 * Called prior to update, perfect place to do things such as updating the bounding box, etc
		 * @param deltaTime The amount of microseconds since the last update
		 * @param scene The scene being updated to
		 */
		virtual void preUpdate(float deltaTime, const Scene& scene) override {
			if (!transitioning) return;
			transitionTime += deltaTime / (1000000.0f * transitionDuration);

			if (transitionTime >= 1.0f) {
				transitioning = false;
				state = transitionState;
			}
		}

		/**
		 * Gets the state composition for this frame
		 * @returns The composition for this frame
		 */
		std::vector<std::pair<T, float>> getStateComposition() const {
			std::vector<std::pair<T, float>> composition;
			if (!transitioning) {
				composition.push_back(std::make_pair(state, 1.0f));
				return composition;
			}

			composition.push_back(std::make_pair(state, 1.0f - transitionTime));
			composition.push_back(std::make_pair(transitionState, transitionTime));
			return composition;
		}
		
	public:

		/**
		 * A map of states to the structures
		 */
		std::unordered_map<T, S> structures;
		
		/**
		 * Animates to a state in a duration
		 * @param state The state to animate to
		 * @param duration The duration of the animation, set 0 to immediately change
		 */
		void animateTo(T state, float duration) {
			transitioning = true;
			transitionTime = 0.0f;
			transitionDuration = duration;
			transitionState = state;
		}

		/**
		 * Instantly sets a state
		 * @param state The state to set
		 */
		void setState(T state) {
			transitioning = false;
			this->state = state;
		}

		/**
		 * Gets the current state, if the node is currently animating between two states then the state closer to the current one is used
		 * @returns The current state
		 */
		T getCurrentState() const {
			if (!transitioning) return state;
			return transitionTime > 0.5f ? transitionState : state;
		}
	};
}
