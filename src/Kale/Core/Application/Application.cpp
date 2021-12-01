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

#include "Application.hpp"

#include <Kale/Core/Clock/Clock.hpp>
#include <Kale/Core/Settings/Settings.hpp>
#include <Kale/Vulkan/Core/Core.hpp>

using namespace Kale;

/**
 * Creates a new application instance
 * @param applicationName The name of your application
 */
Application::Application(const char* applicationName) : applicationName(applicationName) {
	console.load(this->applicationName);
	settings.load(this->applicationName);
}

/**
 * Frees resources and deletes the application
 */
Application::~Application() {
	// Empty Body
}

/**
 * Gets the window
 */
Window& Application::getWindow() {
	return window;
}

/**
 * Gets the currently presented scene
 * @returns The currently presented scene pointer
 */
std::shared_ptr<Scene> Application::getPresentedScene() {
	return presentedScene;
}

/**
 * Gets the window
 */
const Window& Application::getWindow() const {
	return window;
}

/**
 * Gets the currently presented scene
 * @returns The currently presented scene pointer
 */
const std::shared_ptr<Scene> Application::getPresentedScene() const {
	return presentedScene;
}

/**
 * Gets the number of threads currently being used to update
 * @returns The number of threads used for updating
 */
size_t Application::getNumUpdateThreads() const {
	return updateThreads.size();
}

/**
 * Presents a given scene
 * @param scene The scene to present
 */
void Application::presentScene(std::shared_ptr<Scene> scene) {
	scene->onPresent();
	presentedScene->onSceneChange();
	presentedScene = scene;
}

/**
 * Handles updating the application in a separate thread
 * @param threadNum the index of this thread, ranged 0 - numUpdateThreads
 */
void Application::update(size_t threadNum) {

	// Update loop
	Clock clock;
	while (window.isOpen()) {

		// Limit UPS and retrieve it
		float ups = clock.sleep(settings.getMinMSpU());
		
		// Perform updating
		if (presentedScene != nullptr)
			presentedScene->update(threadNum, ups);
	}
}

/**
 * Runs the application
 */
void Application::run() {
	// Creates the window
	window.create(applicationName.c_str());
	
	// Setup Vulkan
	Vulkan::Core::setupCore();
	
	onBegin();

	// Create update threads
	for (size_t i = 0; i < std::thread::hardware_concurrency() - 1; i++) {
		updateThreads.emplace_back(&Application::update, this, i);
	}

	// Render loop
	Clock clock;
	while (window.isOpen()) {

		// Limit FPS and retrieve it
		float fps = clock.sleep(settings.getMinMSpF());
		
		// Update the window for event polling, etc
		window.update();

		if (presentedScene != nullptr)
			presentedScene->render();
	}

	onEnd();

	// Wait for threads
	for (std::thread& thread : updateThreads) thread.join();

	// Cleanup vulkan now that execution is done
	Vulkan::Core::cleanupCore();
}
