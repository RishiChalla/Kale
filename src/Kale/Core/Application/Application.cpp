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

#include "Application.hpp"

#ifdef KALE_VULKAN
#include <Kale/Vulkan/Core/Core.hpp>
#endif

#ifdef KALE_OPENGL
#include <Kale/OpenGL/Core/Core.hpp>
#endif

#include <Kale/Engine/Engine.hpp>

#include <exception>
#include <chrono>
#include <string>

using namespace Kale;

/**
 * Creates a new application instance
 * @param applicationName The name of your application
 */
Application::Application(const char* applicationName) noexcept : applicationName(applicationName) {
	try {
		console.load(this->applicationName);
	}
	catch (const std::exception&) {
		// We have no way of logging effectively, so we'll just have to exit
		exit(-1);
	}
}

/**
 * Gets the window
 */
Window& Application::getWindow() noexcept {
	return window;
}

/**
 * Gets the currently presented scene
 * @returns The currently presented scene pointer
 */
std::shared_ptr<Scene> Application::getPresentedScene() noexcept {
	return presentedScene;
}

/**
 * Gets the window
 */
const Window& Application::getWindow() const noexcept {
	return window;
}

/**
 * Gets the currently presented scene
 * @returns The currently presented scene pointer
 */
std::shared_ptr<const Scene> Application::getPresentedScene() const noexcept {
	return presentedScene;
}

/**
 * Gets the number of threads currently being used to update
 * @returns The number of threads used for updating
 */
size_t Application::getNumUpdateThreads() const noexcept {
	return updateThreads.size();
}

/**
 * Presents a given scene
 * @param scene The scene to present
 */
void Application::presentScene(const std::shared_ptr<Scene>& scene) {
	sceneToPresent = scene;
}

/**
 * Synchronizes udpates
 */
void Application::synchronizeUpdate() {
	std::unique_lock lock(threadSyncMutex);
	numThreadsUpdated--;
	renderingFinished = false;

	if (numThreadsUpdated == 0) {
		updatingFinished = true;
		numThreadsUpdated = getNumUpdateThreads();
		renderSyncCondVar.notify_one();
	}

	threadSyncCondVar.wait(lock, [&]() -> bool { return renderingFinished; });
}

/**
 * Handles updating the application in a separate thread
 * @param threadNum the index of this thread, ranged 0 - numUpdateThreads
 */
void Application::update(size_t threadNum) noexcept {

	// Update loop
	while (window.isOpen()) {

		// Wait until we should update
		synchronizeUpdate();

		// Perform updating
		if (presentedScene != nullptr) try {
			presentedScene->update(threadNum, deltaTime);
		}
		catch (const std::exception& e) {
			console.error("Failed to update presented screen on update thread " + std::to_string(threadNum) + " - " + e.what());
		}
	}

	// Avoid the main thread waiting for update threads which are already finished
	synchronizeUpdate();
}

/**
 * Runs the application
 */
void Application::run() noexcept {
	
	using namespace std::string_literals;
	using namespace std::chrono_literals;
	
	// Creates the window
	window.create(applicationName.c_str());
	
	
#ifdef KALE_VULKAN
	// Setup Vulkan
	Vulkan::Core::setupCore();
#endif

#ifdef KALE_OPENGL
	// Setup OpenGL
	OpenGL::Core::setupCore();
#endif
	
	try {
		onBegin();
	}
	catch (const std::exception& e) {
		console.error("Failed to call onBegin in application - "s + e.what());
	}

	// Setup nodes
	try {
		PathNode::setup();
	}
	catch (const std::exception& e) {
		console.error("Terminating application due to failure to setup nodes - "s + e.what());
		return;
	}

	// Create update threads
	numThreadsUpdated = std::thread::hardware_concurrency();
	for (size_t i = 0; i < std::thread::hardware_concurrency(); i++)
		updateThreads.emplace_back(&Application::update, this, i);

	// Render loop
	auto previousTime = std::chrono::high_resolution_clock::now();
	while (window.isOpen()) {
		
		// Update the window for event polling, etc
		window.update();

		// Calculate FPS
		auto currentTime = std::chrono::high_resolution_clock::now();
		deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(currentTime - previousTime).count());
		previousTime = std::chrono::high_resolution_clock::now();

		// Check if scene has changed prior to updating
		if (sceneToPresent != nullptr) {
			try {
				sceneToPresent->onPresent();
				if (presentedScene != nullptr) presentedScene->onSceneChange();
				presentedScene = sceneToPresent;
				sceneToPresent = nullptr;
			}
			catch (const std::exception& e) {
				console.error("Failed to present scene - "s + e.what());
			}
		}

		// Synchronize with the update threads
		{
			std::unique_lock lock(threadSyncMutex);
			renderingFinished = true;
			threadSyncCondVar.notify_all();
			renderSyncCondVar.wait(lock, [&]() -> bool { return updatingFinished; });
			updatingFinished = false;
		}

		// Render scene
		if (presentedScene != nullptr) try {
			presentedScene->updateNodeStructures();
			presentedScene->render(deltaTime);
		}
		catch (const std::exception& e) {
			console.error("Failed to render presented scene - "s + e.what());
		}
	}

	// Wait for threads
	renderingFinished = true;
	threadSyncCondVar.notify_all();
	for (std::thread& thread : updateThreads) thread.join();

	onEnd();

	// Cleanup nodes
	try {
		PathNode::cleanup();
	}
	catch (const std::exception& e) {
		console.error("Terminating application due to failure to cleanup nodes - "s + e.what());
		return;
	}

#ifdef KALE_VULKAN
	// Cleanup vulkan now that execution is done
	Vulkan::Core::cleanupCore();
#endif

#ifdef KALE_OPENGL
	// Setup OpenGL
	OpenGL::Core::cleanupCore();
#endif
}
