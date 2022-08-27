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

using namespace Kale;

/**
 * Constructs a new scene
 */
Scene::Scene()  {
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
 * Renders the current scene
 * @param threadNum The thread to render between 0 - std::thread::hardware_concurrency()
 */
void Scene::render() const {

#ifdef KALE_OPENGL
	OpenGL::Core::clearScreen(bgColor);
#endif

	Transform cameraToScreen(worldToScreen * camera);
	for (const std::shared_ptr<Node>& node : nodes)
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
 * Called before all nodes are updated
 * @param deltaTime The microseconds since the last update
 */
void Scene::onUpdate(float deltaTime) {
	// Empty Body
}

/**
 * Called before all nodes are pre updated
 * @param deltaTime The microseconds since the last update
 */
void Scene::onPreUpdate(float deltaTime) {
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
