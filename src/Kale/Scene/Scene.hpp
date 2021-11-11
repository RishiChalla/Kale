/*
   Copyright 2021 Rishi Challa

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

#include <Kale/Node/Node.hpp>
#include <Kale/Events/Events.hpp>

#include <list>
#include <mutex>

namespace Kale {
	
	/**
	 * The main scene class for the game
	 * This class handles rendering and nodes management
	 */
	class Scene : public EventHandler {
	private:

		/**
		 * A list of all the nodes to be presented in the current scene
		 */
		std::list<Node*> nodes;

		/**
		 * The mutex used for node thread safety
		 */
		std::mutex mutex;

		/**
		 * Renders the current scene
		 * @param threadNum the index of this thread, ranged 0 - numRenderThreads
		 */
		void render(size_t threadNum) const;

		/**
		 * Updates the current scene
		 * @param threadNum the index of this thread, ranged 0 - numUpdateThreads
		 * @param ups The number of upates per second
		 */
		void update(size_t threadNum, float ups);

		/**
		 * Renders the current scene
		 */
		void present() const;

		friend class Application;

	protected:

		/**
		 * The background color of the scene to use for clearing the screen
		 */
		Vector4f bgColor = {1.0f, 1.0f, 1.0f, 1.0f};

		/**
		 * Adds a node to the scene to render/update
		 * @param node The node to add
		 */
		void addNode(Node& node);

		/**
		 * Removes a node from the scene
		 * @param node The node to remove
		 */
		void removeNode(Node* node);

		/**
		 * Called when the current scene is presented
		 */
		virtual void onPresent();

		/**
		 * Called when the scene is about to be changed
		 */
		virtual void onSceneChange();

		friend class Application;
	};
}