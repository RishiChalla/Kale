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

using namespace Kale;

/**
 * Adds a node to the scene to render/update
 * @param node The node to add
 */
void Scene::addNode(Node& node) {
	nodes.push_back(&node);
}

/**
 * Removes a node from the scene
 * @param node The node to remove
 */
void Scene::removeNode(Node* node) {
	nodes.remove(node);
}

/**
 * Renders the current scene
 */
void Scene::render() {
	// TODO - Vulkan commands to clear screen with background color

	for (Node* node : nodes)
		node->render();
}

/**
 * Updates the current scene
 */
void Scene::update() {
	for (Node* node : nodes)
		node->update();
}