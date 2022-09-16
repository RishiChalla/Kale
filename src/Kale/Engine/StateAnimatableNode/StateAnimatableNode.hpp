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
	 * 
	 * This class allows for nodes to have animation states and to lerp between them
	 */
	class StateAnimatableNode : public Node {
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
		 * Used for thread safety
		 */
		std::mutex mutex;

		/**
		 * Called prior to update, perfect place to do things such as updating the bounding box, etc
		 * @param threadNum the index of the thread this update is called on
		 * @param scene The scene being updated to
		 * @param deltaTime The duration of the last frame in microseconds
		 */
		virtual void preUpdate(size_t threadNum, const Scene& scene, float deltaTime) override;
		
	public:
		
		/**
		 * Animates to a state in a duration, thread safe. If called in pre-update then the state may be updated next frame depending
		 * on whether or not preUpdate has already been called.
		 * @tparam An enum containing all the states, the underlying type must be int.
		 * @param state The state to animate to
		 * @param duration The duration of the animation, set 0 to immediately change
		 */
		template <typename T> void animateTo(T state, float duration) {
			std::lock_guard lock(mutex);
			transitioning = true;
			transitionTime = 0.0f;
			transitionDuration = duration;
			transitionState = static_cast<int>(state);
		}

		/**
		 * Instantly sets a state, thread safe. If called in pre-update then the state may be updated next frame depending
		 * on whether or not preUpdate has already been called.
		 * @tparam An enum containing all the states, the underlying type must be int.
		 * @param state The state to set
		 */
		template <typename T> void setState(T state) {
			std::lock_guard lock(mutex);
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
		bool isTransitioning() const;
	};

	/**
	 * All nodes which allow for state based animation must inherit from this class. This is an implementation of a
	 * Finate State Machine. There are saved hardcoded "states" in the animation where the data for each state is
	 * saved in structure S. It is the responsibility of the inherited node to lerp between any states as required.
	 * @tparam A structure representing the data required for a single state of the animation
	 */
	template <typename S> class FiniteStateAnimatable {
	private:

		/**
		 * Contains structures for each state
		 */
		std::unordered_map<int, S> structures;

		/**
		 * Used for syncing access to structures
		 */
		std::mutex mutex;

	public:

		/**
		 * A pointer to the state animatable node being used for this node's animations. If this is nullptr then it is
		 * assumed that the inherited class will not be using the FSM for animations.
		 */
		std::shared_ptr<StateAnimatableNode> stateAnimationNode;

		/**
		 * Thread safe method to add a structure to the fsm
		 * @tparam An enum containing all the states, the underlying type must be int.
		 * @param state The state to assign the structure to
		 * @param structure The structure to save at the state
		 */
		template <typename T> void addStructure(T state, S structure) {
			std::lock_guard lock(mutex);
			structures[static_cast<int>(state)] = structure;
		}

		/**
		 * Thread safe method to remove a structure from the fsm
		 * @tparam An enum containing all the states, the underlying type must be int.
		 * @param state The state to remove from the fsm
		 */
		template <typename T> void removeStructure(T state) {
			std::lock_guard lock(mutex);
			structures.erase(static_cast<int>(state));
		}

		/**
		 * Thread safe method to get a structure at a state, throws if the state does not exist in the map
		 * @tparam An enum containing all the states, the underlying type must be int.
		 * @param state The state to get the structure at
		 * @returns A *COPY* of the strucutre at the state. A copy is returned to avoid illegal references across threads.
		 */
		template <typename T> S getStructure(T state) const {
			std::lock_guard lock(mutex);
			return structures.at(static_cast<int>(state));
		}
	};
}
