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

#include <Kale/Core/Events/Events.hpp>
#include <Kale/Math/Transform/Transform.hpp>
#include <Kale/Engine/Node/Node.hpp>
#include <Kale/Engine/Light/Light.hpp>

#include <unordered_set>
#include <vector>
#include <list>
#include <mutex>
#include <memory>
#include <queue>

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
		std::vector<std::shared_ptr<Node>> nodes;

		/**
		 * A queue of the nodes to add after nodes updating has been completed
		 */
		std::queue<std::shared_ptr<Node>> nodesToAdd;

		/**
		 * A queue of the nodes to remove after nodes updating has been completed
		 */
		std::list<std::shared_ptr<Node>> nodesToRemove;

		/**
		 * A vector for each thread, within each thread a vector of indices to nodes to update
		 */
		std::vector<std::list<size_t>> nodeThreads;

		/**
		 * A list of the lights affecting the current scene
		 */
		std::unordered_set<std::shared_ptr<Light>> lights;

		/**
		 * The mutex used for node thread safety
		 */
		std::mutex mutex;

		/**
		 * The world to screen transformation matrix. Used internally for rendering and converting
		 * vertices from world points to screen points.
		 */
		Transform worldToScreen;

		/**
		 * Whether or not to update the thread layout when all threads are synchronized
		 */
		bool shouldUpdateThreadLayout = false;

		/**
		 * Renders the current scene
		 */
		void render();

		/**
		 * Updates the current scene
		 * @param threadNum the index of this thread, ranged 0 - numUpdateThreads
		 * @param ups The number of updates per second
		 */
		void update(size_t threadNum, float ups);

		/**
		 * Updates the layouts of node rendering by thread
		 */
		void updateThreadLayout();

		friend class Application;
		friend class Node;

	protected:

		/**
		 * The background color of the scene to use for clearing the screen
		 */
		Color bgColor = {1.0f, 0.0f, 1.0f, 1.0f};

		/**
		 * The scene's camera for viewing
		 */
		Camera camera;
		
		/**
		 * The viewport of the world
		 * (the width and height of the screen in the world's coordinates)
		 * This always has a height of 1080p and the width is dependant on the screen's aspect ratio.
		 * It is recommended to code all nodes based off of the 1080p height and let the engine do the scaling
		 */
		Vector2f viewport;

		/**
		 * Adds a node to the scene to render/update
		 * @param node The node to add
		 * @param updateThreadLayout Whether or not to update the layout of threads
		 */
		void addNode(std::shared_ptr<Node>& node, bool updateThreadLayout = true);

		/**
		 * Removes a node from the scene
		 * @param node The node to remove
		 * @param updateThreadLayout Whether or not to update the layout of threads
		 */
		void removeNode(std::shared_ptr<Node>& node, bool updateThreadLayout = true);

		/**
		 * Adds a light to the scene
		 * @param light The light to add to the scene
		 */
		void addLight(std::shared_ptr<Light>& light);

		/**
		 * Removes a light from the scene
		 * @param light The light to remove from the scene
		 */
		void removeLight(std::shared_ptr<Light>& light);

		/**
		 * Called when the current scene is presented
		 */
		virtual void onPresent();

		/**
		 * Called when the scene is about to be changed
		 */
		virtual void onSceneChange();

		/**
		 * Called when the event is fired
		 */
		void onWindowResize(Vector2ui oldSize, Vector2ui newSize) override;

		friend class Application;

	public:

		/**
		 * Constructs a new scene
		 */
		Scene();
	};
}
