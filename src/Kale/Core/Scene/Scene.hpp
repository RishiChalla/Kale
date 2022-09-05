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
#include <Kale/Core/Events/Events.hpp>
#include <Kale/Math/Transform/Transform.hpp>

#include <list>
#include <vector>
#include <queue>
#include <utility>
#include <memory>
#include <mutex>
#include <condition_variable>

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
		std::list<std::shared_ptr<Node>> nodes;

		/**
		 * Stores the nodes to be updated by their update thread
		 */
		std::vector<std::list<std::shared_ptr<Node>>> updateNodes;

		/**
		 * Stores the nodes to be pre updated by their pre update thread
		 */
		std::vector<std::list<std::shared_ptr<Node>>> preUpdateNodes;

		/**
		 * Holds the sum of the update times and pre update times of each set per thread
		 */
		std::vector<std::pair<float, float>> threadedNodePerformanceTimes;

		/**
		 * A queue of nodes to add
		 */
		std::queue<std::shared_ptr<Node>> nodesToAdd;

		/**
		 * A queue of nodes to remove
		 */
		std::queue<std::shared_ptr<Node>> nodesToRemove;

		/**
		 * The mutex used for adding/removing nodes safety
		 */
		std::mutex nodeQueueUpdateMutex;

		/**
		 * Mutex used for syncrhonizing pre updates
		 */
		std::mutex nodePreUpdateMutex;

		/**
		 * Condition variable to check if threads have completed pre updating
		 */
		std::condition_variable nodePreUpdateCondVar;

		/**
		 * The count of nodes which have pre updated
		 */
		size_t nodesPreUpdated;
		size_t generation;

		/**
		 * The world to screen transformation matrix. Used internally for rendering and converting
		 * vertices from world points to screen points.
		 */
		Transform worldToScreen;

		/**
		 * Renders the current scene
		 * @param deltaTime The time the last frame has taken to update and render
		 */
		void render(float deltaTime) const;

		/**
		 * Updates the current scene
		 * @param threadNum the index of this thread, ranged 0 - numUpdateThreads
		 * @param deltaTime The time the last frame has taken to update and render
		 */
		void update(size_t threadNum, float deltaTime);

		/**
		 * Updates the data structures holding nodes based off of the queues, MUST be called on the main thread
		 * after the completion of all updates in the frame.
		 */
		void updateNodeStructures();

		friend class Application;
		friend class Node;

	protected:

		/**
		 * The background color of the scene to use for clearing the screen
		 */
		Vector4f bgColor = 0xFFFFFF;

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
		 * Due to the engine being scaled from 1080p, when dealing with wide or tall windows the screen space may start from a negative number
		 * or an unusually large number. This variable holds the scene bounds, (the x position of the left & right points on the window)
		 */
		Rect sceneBounds;

		/**
		 * Adds a node to the scene to render/update
		 * @param node The node to add
		 */
		template <typename T> void addNode(std::shared_ptr<T>& node) {
			std::shared_ptr<Kale::Node> nodePtr = std::dynamic_pointer_cast<Kale::Node>(node);
			std::lock_guard guard(nodeQueueUpdateMutex);
			nodesToAdd.push(nodePtr);
		}

		/**
		 * Removes a node from the scene
		 * @param node The node to remove
		 */
		void removeNode(std::shared_ptr<Node>& node);

		/**
		 * Removes a node from the scene
		 * @param node The node to remove
		 */
		template <typename T> void removeNode(std::shared_ptr<T>& node) {
			std::shared_ptr<Kale::Node> nodePtr = std::dynamic_pointer_cast<Kale::Node>(node);
			std::lock_guard guard(nodeQueueUpdateMutex);
			nodesToRemove.push(nodePtr);
		}

		/**
		 * Called when the current scene is presented
		 */
		virtual void onPresent();

		/**
		 * Called when the scene is about to be changed
		 */
		virtual void onSceneChange();

		/**
		 * Called before all nodes are updated. Do NOT write code directly in here, this function is called once on each
		 * thread every frame. If you must run code every frame outside of a node, then make sure you're only running
		 * your required code on one thread by using an if statement on threadNum.
		 * @param threadNum The thread this function is being called on
		 * @param deltaTime The microseconds since the last update
		 */
		virtual void onUpdate(size_t threadNum, float deltaTime);

		/**
		 * Called before all nodes are pre updated. Do NOT write code directly in here, this function is called once on each
		 * thread every frame. If you must run code every frame outside of a node, then make sure you're only running
		 * your required code on one thread by using an if statement on threadNum.
		 * @param threadNum The thread this function is being called on
		 * @param deltaTime The microseconds since the last update
		 */
		virtual void onPreUpdate(size_t threadNum, float deltaTime);

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

		/**
		 * Gets the ndoes within the scene
		 * @returns The nodes
		 */
		const std::list<std::shared_ptr<Node>>& getNodes() const;

		/**
		 * Gets the background color of the scene
		 * @returns The background color
		 */
		Color getBgColor() const;

		/**
		 * Gets the camera used to render this scene
		 * @returns The camera
		 */
		const Camera& getCamera() const;

		/**
		 * Gets the current viewport of the scene
		 * @returns The viewport
		 */
		Vector2f getViewport() const;

		/**
		 * Due to the engine being scaled from 1080p, when dealing with wide or tall windows the screen space may start from a negative number
		 * or an unusually large number. This function returns the scene bounds (the x coordinates of the left and right points on the scene display)
		 * @returns The window bounds in world coordinates
		 */
		Rect getSceneBounds() const;

	};
}
