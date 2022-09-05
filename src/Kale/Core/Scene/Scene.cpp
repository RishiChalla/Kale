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

#include "Scene.hpp"

#include <Kale/Core/Application/Application.hpp>

#ifdef KALE_OPENGL

#include <Kale/OpenGL/Core/Core.hpp>

#endif

#ifdef KALE_VULKAN

#include <Kale/Vulkan/Core/Core.hpp>

#endif

#include <algorithm>
#include <sstream>

using namespace Kale;

/**
 * Constructs a new scene
 */
Scene::Scene() {
	updateNodes.resize(std::thread::hardware_concurrency());
	preUpdateNodes.resize(std::thread::hardware_concurrency());
	nodesPreUpdated = std::thread::hardware_concurrency();
	generation = 0;

	Vector2f size = mainApp->getWindow().getFramebufferSize().cast<float>();
	viewport = {size.x * 1080.0f / size.y, 1080.0f};

	worldToScreen.scale(2.0f / viewport);
	worldToScreen.translate(Vector2f(1920.0f, 1080.0f) / -2.0f);
	sceneBounds = Rect{{(1920.0f - viewport.x) / 2.0f, 1080.0f}, {(1920.0f + viewport.x) / 2.0f, 0.0f}};
}

/**
 * Called when the event is fired
 */
void Scene::onWindowResize(Vector2ui oldSize, Vector2ui newSize) {
	Vector2f size = mainApp->getWindow().getFramebufferSize().cast<float>();
	viewport = {size.x * 1080.0f / size.y, 1080.0f};

	worldToScreen.setIdentity();
	worldToScreen.scale(2.0f / viewport);
	worldToScreen.translate(Vector2f(1920.0f, 1080.0f) / -2.0f);
	sceneBounds = Rect{{(1920.0f - viewport.x) / 2.0f, 1080.0f}, {(1920.0f + viewport.x) / 2.0f, 0.0f}};
}

/**
 * Updates the data structures holding nodes based off of the queues, MUST be called on the main thread
 * after the completion of all updates in the frame.
 */
void Scene::updateNodeStructures() {

	// Return if no nodes need to be added
	if (nodesToAdd.empty() && nodesToRemove.empty()) return;

	// Add all the nodes till there's none left to add
	while (!nodesToAdd.empty()) {
		// Get the node and add it to the main list
		std::shared_ptr<Node> node = nodesToAdd.front();
		nodesToAdd.pop();
		nodes.push_back(node);

		// Find the thread with the current smallest total update time
		size_t threadIndex = std::distance(threadedNodePerformanceTimes.begin(),
			std::min_element(threadedNodePerformanceTimes.begin(), threadedNodePerformanceTimes.end(),
				[](const std::pair<float, float>& a, const std::pair<float, float>& b) -> bool {
					return a.first > b.first;
				}
			)
		);

		// Add the node to the thread with the smallest update time and add it to the thread's total time
		threadedNodePerformanceTimes[threadIndex].first += node->updateTime;
		updateNodes[threadIndex].push_back(node);

		// Find the thread with the current smallest total pre update time
		threadIndex = std::distance(threadedNodePerformanceTimes.begin(),
			std::min_element(threadedNodePerformanceTimes.begin(), threadedNodePerformanceTimes.end(),
				[](const std::pair<float, float>& a, const std::pair<float, float>& b) -> bool {
					return a.second > b.second;
				}
			)
		);

		// Add the node to the thread with the smallest pre update time and add it to the thread's total time
		threadedNodePerformanceTimes[threadIndex].second += node->preUpdateTime;
		preUpdateNodes[threadIndex].push_back(node);
	}

	while (!nodesToRemove.empty()) {
		// Get the node and remove it from the main list
		std::shared_ptr<Node> node = nodesToRemove.front();
		nodesToRemove.pop();
		nodes.remove(node);

		bool updateFound = false;
		bool preUpdateFound = false;

		// Loop through the threads till we find the thread holding the update
		for (size_t threadIndex = 0; threadIndex < updateNodes.size(); threadIndex++) {

			if (updateFound && preUpdateFound) break;

			if (!updateFound) {
				// Check if the node is updated in this thread
				auto it = std::find(updateNodes[threadIndex].begin(), updateNodes[threadIndex].end(), node);
				if (it != updateNodes[threadIndex].end()) {
					// Remove the node from updates & update the performance times
					threadedNodePerformanceTimes[threadIndex].first -= node->updateTime;
					updateNodes[threadIndex].erase(it);
					updateFound = true;
				}
			}

			if (!preUpdateFound) {
				// Check if the node is pre updated in this thread
				auto it = std::find(preUpdateNodes[threadIndex].begin(), preUpdateNodes[threadIndex].end(), node);
				if (it != preUpdateNodes[threadIndex].end()) {
					// Remove the node from updates & update the performance times
					threadedNodePerformanceTimes[threadIndex].second -= node->preUpdateTime;
					preUpdateNodes[threadIndex].erase(it);
					preUpdateFound = true;
				}
			}
		}
	}
}

/**
 * Renders the current scene
 * @param deltaTime The time the last frame has taken to update and render
 */
void Scene::render(float deltaTime) const {

#ifdef KALE_OPENGL
	OpenGL::Core::clearScreen(bgColor);
#endif

	Transform cameraToScreen(worldToScreen * camera);
	for (const std::shared_ptr<Node>& node : nodes)
		node->render(cameraToScreen, deltaTime);
	
	// Swaps the buffers/uses the swapchain to display output
#ifdef KALE_OPENGL
	OpenGL::Core::swapBuffers();
#endif

#ifdef KALE_VULKAN
	// Vulkan::Core::swapBuffers();
#endif
}

/**
 * Updates the current scene
 * @param threadNum the index of this thread, ranged 0 - numUpdateThreads
 * @param deltaTime The time the last frame has taken to update and render
 */
void Scene::update(size_t threadNum, float deltaTime) {
	
	// Pre updating
	onPreUpdate(threadNum, deltaTime);
	for (std::shared_ptr<Node>& node : preUpdateNodes[threadNum]) node->preUpdate(threadNum, *this, deltaTime);
	
	// mark pre updating as done & notify other threads if necessary
	{
		std::unique_lock lock(nodePreUpdateMutex);
		nodesPreUpdated--;
		size_t localGeneration = generation;
		if (nodesPreUpdated == 0) {
			generation++;
			nodesPreUpdated = mainApp->getNumUpdateThreads();
			nodePreUpdateCondVar.notify_all();
		}
		else {
			nodePreUpdateCondVar.wait(lock, [&]() -> bool { return localGeneration != generation; });
		}
	}

	// notify other threads if applicable
	if (nodesPreUpdated == mainApp->getNumUpdateThreads()) nodePreUpdateCondVar.notify_all();

	// Updating
	onUpdate(threadNum, deltaTime);
	for (std::shared_ptr<Node>& node : updateNodes[threadNum]) node->preUpdate(threadNum, *this, deltaTime);
}

/**
 * Called before all nodes are updated. Do NOT write code directly in here, this function is called once on each
 * thread every frame. If you must run code every frame outside of a node, then make sure you're only running
 * your required code on one thread by using an if statement on threadNum.
 * @param threadNum The thread this function is being called on
 * @param deltaTime The microseconds since the last update
 */
void Scene::onUpdate(size_t threadNum, float deltaTime) {
	// Empty Body
}

/**
 * Called before all nodes are pre updated. Do NOT write code directly in here, this function is called once on each
 * thread every frame. If you must run code every frame outside of a node, then make sure you're only running
 * your required code on one thread by using an if statement on threadNum.
 * @param threadNum The thread this function is being called on
 * @param deltaTime The microseconds since the last update
 */
void Scene::onPreUpdate(size_t threadNum, float deltaTime) {
	// Empty Body
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

/**
 * Gets the ndoes within the scene
 * @returns The nodes
 */
const std::list<std::shared_ptr<Node>>& Scene::getNodes() const {
	return nodes;
}

/**
 * Gets the background color of the scene
 * @returns The background color
 */
Color Scene::getBgColor() const {
	return bgColor;
}

/**
 * Gets the camera used to render this scene
 * @returns The camera
 */
const Camera& Scene::getCamera() const {
	return camera;
}

/**
 * Gets the current viewport of the scene
 * @returns The viewport
 */
Vector2f Scene::getViewport() const {
	return viewport;
}

/**
 * Due to the engine being scaled from 1080p, when dealing with wide or tall windows the screen space may start from a negative number
 * or an unusually large number. This function returns the scene bounds (the x coordinates of the left and right points on the scene display)
 * @returns The window bounds in world coordinates
 */
Rect Scene::getSceneBounds() const {
	return sceneBounds;
}
