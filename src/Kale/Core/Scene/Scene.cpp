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

#include <algorithm>

using namespace Kale;

/**
 * Constructs a new scene
 */
Scene::Scene() : nodeThreads(mainApp->getNumUpdateThreads()) {
	Vector2f size = mainApp->getWindow().getSizeF();
	viewport = {size.x * 1080.0f / size.y, 1080.0f};
	worldToScreen.scale(size / viewport);
}

/**
 * Called when the event is fired
 */
void Scene::onWindowResize(Vector2ui oldSize, Vector2ui newSize) {
	Vector2f size = newSize.cast<float>();
	viewport = {size.x * 1080.0f / size.y, 1080.0f};
	worldToScreen.setIdentity();
	worldToScreen.scale(size / viewport);
}

/**
 * Updates the layouts of node rendering by thread
 */
void Scene::updateThreadLayout() {
	for (std::list<size_t>& nodeIndices : nodeThreads) nodeIndices.clear();

	for (size_t i = nodes.size() - 1; i >= 0; i--) {
		std::vector<float> totalThreadTimes;
		totalThreadTimes.reserve(nodeThreads.size());

		for (std::list<size_t>& nodeIndices : nodeThreads) {
			float totalThreadTime = 0.0f;
			for (size_t index : nodeIndices) totalThreadTime += nodes[index]->updateTime;
			totalThreadTimes.push_back(totalThreadTime);
		}

		size_t minIndex = std::min_element(totalThreadTimes.begin(), totalThreadTimes.end()) - totalThreadTimes.begin();
		nodeThreads[minIndex].push_back(i);
	}
}

/**
 * Adds a node to the scene to render/update
 * @param node The node to add
 * @param updateThreadLayout Whether or not to update the layout of threads
 */
void Scene::addNode(std::shared_ptr<Node>& node, bool updateThreadLayout) {
	std::lock_guard<std::mutex> guard(mutex);
	if (updateThreadLayout) shouldUpdateThreadLayout = true;
	nodesToAdd.push(node);
}

/**
 * Removes a node from the scene
 * @param node The node to remove
 * @param updateThreadLayout Whether or not to update the layout of threads
 */
void Scene::removeNode(std::shared_ptr<Node>& node, bool updateThreadLayout) {
	std::lock_guard<std::mutex> guard(mutex);
	if (updateThreadLayout) shouldUpdateThreadLayout = true;
	nodesToRemove.push_back(node);
}

/**
 * Adds a light to the scene
 * @param light The light to add to the scene
 */
void Scene::addLight(std::shared_ptr<Light>& light) {
	std::lock_guard<std::mutex> guard(mutex);
	lights.insert(light);
}

/**
 * Removes a light from the scene
 * @param light The light to remove from the scene
 */
void Scene::removeLight(std::shared_ptr<Light>& light) {
	std::lock_guard<std::mutex> guard(mutex);
	lights.erase(light);
}

/**
 * Renders the current scene
 */
void Scene::render() {
	// Clear screen with background color
	Vector2f size(mainApp->getWindow().getSizeF());
	mainApp->getWindow().getCanvas().drawRect({0.0f, 0.0f, size.x, size.y}, SkPaint(bgColor));

	// Render is always called from main thread, update threads & add/remove nodes here
	while (!nodesToAdd.empty()) {
		nodes.insert(std::upper_bound(nodes.begin(), nodes.end(), nodesToAdd.front(), [](const auto& first, const auto& second) -> bool {
			return first->updateTime < second->updateTime;
		}), nodesToAdd.front());
		nodesToAdd.pop();
	}

	if (!nodesToRemove.empty()) {
		nodes.erase(std::remove_if(nodes.begin(), nodes.end(), [&](const std::shared_ptr<Node>& node) -> bool {
			return std::find(nodesToRemove.begin(), nodesToRemove.end(), node) != nodesToRemove.end();
		}), nodes.end());
		nodesToRemove.clear();
	}

	if (shouldUpdateThreadLayout) updateThreadLayout();

	// Combine the camera transformation matrix with the world coordinates to Skia's coordinates matrix
	Transform cameraToScreen(worldToScreen * camera);

	// Go through each node and render it if it's in the bounds of the view
	bool invalidPtr = false;
	for (const std::shared_ptr<Node>& node : nodes) {
		Rect boundingBox = node->getBoundingBox();
		if (!cameraToScreen.isInView(boundingBox, viewport)) continue;
		node->render(cameraToScreen);
	}
}

/**
 * Updates the current scene
 * @param threadNum the index of this thread, ranged 0 - numUpdateThreads
 * @param ups The number of updates per second
 */
void Scene::update(size_t threadNum, float ups) {
	for (size_t i : nodeThreads[threadNum])
		nodes[i]->update(ups, lights);
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
