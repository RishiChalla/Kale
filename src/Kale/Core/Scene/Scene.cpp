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
	worldToScreen.translate((viewport.x - 1920.0f) / 2.0f, 0.0f);
}

/**
 * Called when the event is fired
 */
void Scene::onWindowResize(Vector2ui oldSize, Vector2ui newSize) {
	Vector2f size = newSize.cast<float>();
	viewport = {size.x * 1080.0f / size.y, 1080.0f};
	worldToScreen.setIdentity();
	worldToScreen.scale(size / viewport);
	worldToScreen.translate((viewport.x - 1920.0f) / 2.0f, 0.0f);
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
	mainApp->getWindow().getCanvas().clear(bgColor);

	// Combine the camera transformation matrix with the world coordinates to Skia's coordinates matrix
	Camera cameraToScreen(camera * worldToScreen);

	// Go through each node and render it if it's in the bounds of the view
	for (const std::shared_ptr<Node>& node : nodes)
		node->render(cameraToScreen, *this);
}

/**
 * Updates the current scene
 * @param deltaTime The microseconds since the last update
 */
void Scene::update(float deltaTime) {
	for (std::shared_ptr<Node>& node : nodes)
		node->preUpdate(deltaTime, *this);

	for (std::shared_ptr<Node>& node : nodes)
		node->update(deltaTime, *this);
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
 * Gets the lights within the scene
 * @returns The lights
 */
const std::unordered_set<std::shared_ptr<Light>>& Scene::getLights() const {
	return lights;
}

/**
 * Gets the background color of the scene
 * @returns The background color
 */
Color Scene::getBgColor() const {
	return bgColor;
}

/**
 * Gets the ambient color and intensity of the scene
 * @returns a pair of the ambient color (where alpha is color intensity), and light intensity
 */
std::pair<Color, float> Scene::getAmbient() const {
	return std::make_pair(ambient, ambientIntesity);
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
const Vector2f Scene::getViewport() const {
	return viewport;
}
