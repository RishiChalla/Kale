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
#include <mutex>
#include <memory>

namespace Kale {

	/**
	 * This class allows for nodes to have animation states and to lerp between them.
	 * @tparam S A structure holding the data for a particular state
	 */
	template <typename S> class StateAnimatable {
	protected:

		/**
		 * The path state currently being rendered
		 */
		int state;

		/**
		 * The state currently being transitioned to
		 */
		int transitionState;

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
		 * Contains structures for each state
		 */
		std::unordered_map<int, S> structures;

		/**
		 * Information regarding the current animation. If set, it means we're currently animating through a series of states and durations
		 */
		std::vector<std::pair<int, float>> animationInfo;

		/**
		 * The current animation index in animationInfo
		 */
		size_t animationIndex;

		/**
		 * Whether or not we are to loop through the information in animationInfo in an infinite loop
		 */
		bool animationLoop;
		
	public:

		/**
		 * Updates the state of the node
		 * @note There is no need to call this manually, it is the responsibility of the node which contains this state animatable to update.
		 * @param deltaTime The duration of the last frame in microseconds
		 */
		void updateState(float deltaTime) {
			if (!transitioning) return;
			transitionTime += deltaTime / (1000000.0f * transitionDuration);

			// Animation complete
			if (transitionTime >= 1.0f) {
				state = transitionState;

				// There is no further animation required
				if (animationInfo.empty()) {
					transitioning = false;
					return;
				}

				// We need to move to the next animation stage
				if (++animationIndex >= animationInfo.size()) {
					// We don't need to loop, we've finished.
					if (!animationLoop) {
						animationInfo.clear();
						transitioning = false;
						return;
					}

					// We finished going through the animation info, check if we need to repeat
					animationIndex = 0;
				}

				// Prepare the next animation
				transitionTime = 0.0f;
				transitionDuration = animationInfo[animationIndex].second;
				transitionState = animationInfo[animationIndex].first;
			}
		}

		/**
		 * Adds a structure to the map of states
		 * @tparam An enum containing all the states, the underlying type must be int.
		 * @param state The state to add the structure to
		 * @param structure THe structure to add
		 */
		template <typename T> void addStructure(T state, const S& structure) {
			structures[static_cast<int>(state)] = structure;
		}

		/**
		 * Removes a structure from the map of states
		 * @tparam An enum containing all the states, the underlying type must be int.
		 * @param state The state to remove the structure from
		 */
		template <typename T> void removeStructure(T state) {
			structures.erase(static_cast<int>(state));
		}

		/**
		 * Gets a structure for a specific state
		 * @tparam An enum containing all the states, the underlying type must be int.
		 * @param state The state to get the structure for
		 * @returns A copy of the structure at this state (a copy is used to avoid references across threads)
		 */
		template <typename T> S getStructure(T state) const {
			return structures.at(static_cast<int>(state));
		}
		
		/**
		 * Animates to a state in a duration, thread safe. If called in pre-update then the state may be updated next frame depending
		 * on whether or not preUpdate has already been called.
		 * @tparam An enum containing all the states, the underlying type must be int.
		 * @param state The state to animate to
		 * @param duration The duration of the animation, set 0 to immediately change
		 */
		template <typename T> void animateTo(T state, float duration) {
			animationInfo.clear();
			transitioning = true;
			transitionTime = 0.0f;
			transitionDuration = duration;
			transitionState = static_cast<int>(state);
		}

		/**
		 * Animates through multiple states once
		 * @tparam T An enum containing all the states, the underlying type must be int.
		 * @param stages An array of pairs of each state with the duration to spend animating to the state
		 */
		template <typename T> void animateOnce(std::vector<std::pair<T, float>> stages) {
			animationInfo.clear();
			for (const std::pair<T, float> stage : stages)
				animationInfo.push_back(std::make_pair(static_cast<int>(stage.first), stage.second));
			animationIndex = 0;
			animationLoop = false;

			transitioning = true;
			transitionTime = 0.0f;
			transitionDuration = stages[0].second;
			transitionState = static_cast<int>(stages[0].first);
		}

		/**
		 * Animates through multiple states in an infinite loop
		 * @tparam T An enum containing all the states, the underlying type must be int.
		 * @param stages An array of pairs of each state with the duration to spend animating to the state
		 */
		template <typename T> void animateLoop(std::vector<std::pair<T, float>> stages) {
			animationInfo.clear();
			for (const std::pair<T, float> stage : stages)
				animationInfo.push_back(std::make_pair(static_cast<int>(stage.first), stage.second));
			animationIndex = 0;
			animationLoop = true;

			transitioning = true;
			transitionTime = 0.0f;
			transitionDuration = stages[0].second;
			transitionState = static_cast<int>(stages[0].first);
		}

		/**
		 * Instantly sets a state, thread safe. If called in pre-update then the state may be updated next frame depending
		 * on whether or not preUpdate has already been called.
		 * @tparam An enum containing all the states, the underlying type must be int.
		 * @param state The state to set
		 */
		template <typename T> void setState(T state) {
			animationInfo.clear();
			transitioning = false;
			this->state = static_cast<int>(state);
		}

		/**
		 * Gets the current state, if the node is currently animating between two states then the state closer to the current one is used
		 * Thread safe if called in update. If called in pre-update, there may be a data race as to whether or not the animation has
		 * been updated for this frame.
		 * @tparam An enum containing all the states, the underlying type must be int.
		 * @returns The current state
		 */
		template <typename T> T getCurrentState() const {
			if (!transitioning) return static_cast<T>(state);
			return transitionTime > 0.5f ? static_cast<T>(transitionState) : static_cast<T>(state);
		}

		/**
		 * Gets the state composition for this frame.
		 * Thread safe if called in update. If called in pre-update, there may be a data race as to whether or not the animation has
		 * been updated for this frame.
		 * @tparam An enum containing all the states, the underlying type must be int.
		 * @returns The composition for this frame
		 */
		template <typename T> std::vector<std::pair<T, float>> getStateComposition() const {
			std::vector<std::pair<T, float>> composition;
			if (!transitioning) {
				composition.push_back(std::make_pair(static_cast<T>(state), 1.0f));
				return composition;
			}

			composition.push_back(std::make_pair(static_cast<T>(state), 1.0f - transitionTime));
			composition.push_back(std::make_pair(static_cast<T>(transitionState), transitionTime));
			return composition;
		}

		/**
		 * Returns whether or not the animation is transitioning between two states.
		 * Thread safe if called in update. If called in pre-update, there may be a data race as to whether or not the animation has
		 * been updated for this frame.
		 * @returns Whether or not the animation is transitioning between two states
		 */
		bool isTransitioning() const {
			return transitioning;
		}
	};
}
