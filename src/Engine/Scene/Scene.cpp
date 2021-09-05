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
#include <Engine/Application/Application.hpp>

using namespace Islands;

/**
 * Creates a new scene instance
 * @param window A reference to the window used for rendering
 */
Scene::Scene(sf::RenderWindow& mainWindow) : window(mainWindow) {
	camera.setCenter(0, 0);
	camera.setSize(events->getWindowSize().cast<float>());
}

/**
 * Draws/renders the scene in its entirety
 */
void Scene::render() const {
	window.clear(bgColor);

	// Render all nodes
	for (auto node : nodes) {
		window.draw(*std::get<1>(node));
	}

	window.display();
}

/**
 * Adds a node to the scene
 * @param zIndex The rendering order of the element
 * @param node The node to add
 */
void Scene::addNode(unsigned int zIndex, std::shared_ptr<sf::Drawable> node) {
	auto it = nodes.end();
	for (; it != nodes.begin(); it--) {
		if (zIndex > std::get<0>(*it)) continue;
		break;
	}
	nodes.insert(it, std::make_tuple(zIndex, node));
}

/**
 * Removes a node from the scene
 * @param node The node to remove
 */
void Scene::removeNode(std::shared_ptr<sf::Drawable> node) {
	nodes.remove_if([&](auto n) {
		return std::get<1>(n) == node;
	});
}

/**
 * Begins the rendering of the scene
 */
void Scene::begin() {
	window.setView(camera);
	camera.setSize(events->getWindowSize().cast<float>());
	onBegin();
}

/**
 * Updates the scene
 * This method is thread safe, and is intended to be called from a separate update thread
 */
void Scene::update() {
	onUpdate();
}

/**
 * Ends the rendering of the scene
 */
void Scene::end() {
	window.setView(window.getDefaultView());
	onEnd();
}

/**
 * Called when the event is fired
 */
void Scene::onWindowResize(const Vector2ui& newSize) {
	camera.setSize(newSize.cast<float>());
	window.setView(camera);
}