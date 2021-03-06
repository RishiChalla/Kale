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

#include <Kale/Core/Application/Application.hpp>

#ifdef KALE_OPENGL

#include <Kale/OpenGL/Core/Core.hpp>

#endif

#ifdef KALE_VULKAN

#include <Kale/Vulkan/Core/Core.hpp>

#endif

using namespace Kale;

/**
 * Constructs a new scene
 */
Scene::Scene()  {
	Vector2f size = mainApp->getWindow().getSizeF();
	viewport = {size.x * 1080.0f / size.y, 1080.0f};

	worldToScreen.scale(2.0f / viewport);
	worldToScreen.translate(viewport / -2.0f);
}

/**
 * Called when the event is fired
 */
void Scene::onWindowResize(Vector2ui oldSize, Vector2ui newSize) {
	Vector2f size = newSize.cast<float>();
	viewport = {size.x * 1080.0f / size.y, 1080.0f};

	worldToScreen.setIdentity();
	worldToScreen.scale(2.0f / viewport);
	worldToScreen.translate(viewport / -2.0f);
}

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
void Scene::render() const {

#ifdef KALE_OPENGL
	OpenGL::Core::clearScreen(bgColor);
#endif

	Transform cameraToScreen(worldToScreen * camera);
	for (Node* node : nodes)
		node->render(cameraToScreen);
	
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
