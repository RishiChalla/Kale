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

#include "Scene.hpp"

#include <Kale/Application/Application.hpp>

using namespace Kale;

/**
 * Adds a node to the scene to render/update
 * @param node The node to add
 */
void Scene::addNode(Node& node) {
	std::lock_guard<std::mutex> guard(mutex);
	nodes.push_back(&node);
}

/**
 * Removes a node from the scene
 * @param node The node to remove
 */
void Scene::removeNode(Node* node) {
	std::lock_guard<std::mutex> guard(mutex);
	nodes.remove(node);
}

/**
 * Renders the current scene
 * @param threadNum The thread to render between 0 - std::thread::hardware_concurrency()
 */
void Scene::render(size_t threadNum) const {
	for (Node* node : nodes)
		node->render(threadNum);
}

/**
 * Updates the current scene
 * @param threadNum the index of this thread, ranged 0 - numUpdateThreads
 * @param ups The number of upates per second
 */
void Scene::update(size_t threadNum, float ups) {
	for (Node* node : nodes)
		node->update(threadNum, ups);
}

/**
 * Renders the current scene - this is GUARANTEED to be called on the main thread
 */
void Scene::present() const {
	// TODO - Vulkan commands to clear screen & swap frame buffers
}

/**
 * Called when the current scene is presented
 */
void Scene::onPresent() {
	mainApp->getWindow().registerEvents(dynamic_cast<EventHandler*>(this));
}

/**
 * Called when the scene is about to be changed
 */
void Scene::onSceneChange() {
	mainApp->getWindow().removeEvents(dynamic_cast<EventHandler*>(this));
}