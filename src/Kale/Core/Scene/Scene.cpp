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

using namespace Kale;

/**
 * Constructs a new scene
 */
Scene::Scene()  {
	Vector2f size = mainApp->getWindow().getSizeF();
	viewport = {size.x * 1080.0f / size.y, 1080.0f};

	worldToScreen.scale(size / 1080.0f);
}

/**
 * Called when the event is fired
 */
void Scene::onWindowResize(Vector2ui oldSize, Vector2ui newSize) {
	Vector2f size = newSize.cast<float>();
	viewport = {size.x * 1080.0f / size.y, 1080.0f};

	worldToScreen.setIdentity();
	worldToScreen.scale(size / 1080.0f);
}

/**
 * Adds a node to the scene to render/update
 * @param node The node to add
 */
void Scene::addNode(std::shared_ptr<Node> node) {
	std::lock_guard<std::mutex> guard(mutex);
	nodes.push_back(node);
}

/**
 * Removes a node from the scene
 * @param node The node to remove
 */
void Scene::removeNode(std::shared_ptr<Node> node) {
	std::lock_guard<std::mutex> guard(mutex);
	nodes.remove(node);
}

/**
 * Renders the current scene
 * @param threadNum The thread to render between 0 - std::thread::hardware_concurrency()
 */
void Scene::render() const {
	// Clear screen with background color
	{
		Vector2f size(mainApp->getWindow().getSizeF());
		mainApp->getWindow().getCanvas().drawRect({0.0f, 0.0f, size.x, size.y}, SkPaint(bgColor));
	}

	Transform cameraToScreen(worldToScreen * camera);
	for (std::shared_ptr<Node> node : nodes)
		node->render(cameraToScreen);
}

/**
 * Updates the current scene
 * @param threadNum the index of this thread, ranged 0 - numUpdateThreads
 * @param ups The number of updates per second
 */
void Scene::update(size_t threadNum, float ups) {
	// TODO - Delegate updating on separate threads per node to minimize render time
	// for (Node* node : nodes)
	// 	node->update(threadNum, ups);
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
