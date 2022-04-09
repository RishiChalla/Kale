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
Scene::Scene() {
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
 * Adds a node to the scene to render/update
 * @param node The node to add
 */
void Scene::addNode(std::shared_ptr<Node>& node) {
	nodes.insert(std::upper_bound(nodes.begin(), nodes.end(), node, [](const std::shared_ptr<Node>& first, const std::shared_ptr<Node>& second) -> bool {
		return first->zPosition < second->zPosition;
	}), node);
}

/**
 * Removes a node from the scene
 * @param node The node to remove
 */
void Scene::removeNode(std::shared_ptr<Node>& node) {
	nodes.erase(std::remove(nodes.begin(), nodes.end(), node), nodes.end());
}

/**
 * Adds a light to the scene
 * @param light The light to add to the scene
 */
void Scene::addLight(std::shared_ptr<Light>& light) {
	lights.insert(light);
	std::shared_ptr<Node> node = std::dynamic_pointer_cast<Node>(light);
	nodes.insert(std::upper_bound(nodes.begin(), nodes.end(), node, [](const std::shared_ptr<Node>& first, const std::shared_ptr<Node>& second) -> bool {
		return first->zPosition < second->zPosition;
	}), node);
}

/**
 * Removes a light from the scene
 * @param light The light to remove from the scene
 */
void Scene::removeLight(std::shared_ptr<Light>& light) {
	lights.erase(light);
	nodes.erase(std::remove(nodes.begin(), nodes.end(), std::dynamic_pointer_cast<Node>(light)), nodes.end());
}

/**
 * Renders the current scene
 */
void Scene::render() {
	// Clear screen with background color
	Vector2f size(mainApp->getWindow().getSizeF());
	mainApp->getWindow().getCanvas().drawRect({0.0f, 0.0f, size.x, size.y}, SkPaint(bgColor));

	// Combine the camera transformation matrix with the world coordinates to Skia's coordinates matrix
	Transform cameraToScreen(worldToScreen * camera);

	// Go through each node and render it if it's in the bounds of the view
	for (const std::shared_ptr<Node>& node : nodes) {
		Rect boundingBox = node->getBoundingBox();
		if (!cameraToScreen.isInView(boundingBox, viewport)) continue;
		node->render(cameraToScreen);
	}
}

/**
 * Updates the current scene
 * @param deltaTime The microseconds since the last update
 */
void Scene::update(float deltaTime) {
	for (std::shared_ptr<Node>& node : nodes)
		node->update(deltaTime, lights);
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
