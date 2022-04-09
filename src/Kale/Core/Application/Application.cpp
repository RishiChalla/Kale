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

#include <exception>
#include <string>
#include <chrono>

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
 * Presents a given scene
 * @param scene The scene to present
 */
void Application::presentScene(const std::shared_ptr<Scene>& scene) {
	
	using namespace std::string_literals;
	
	try {
		scene->onPresent();
		if (presentedScene != nullptr) presentedScene->onSceneChange();
		presentedScene = scene;
	}
	catch (const std::exception& e) {
		console.error("Failed to present scene - "s + e.what());
		throw e;
	}
}

/**
 * Runs the application
 */
void Application::run() noexcept {
	
	using namespace std::string_literals;
	
	// Creates the window
	window.create(applicationName.c_str());

	try {
		onBegin();
	}
	catch (const std::exception& e) {
		console.error("Failed to call onBegin in application - "s + e.what());
	}

	// Render loop
	auto previousTime = std::chrono::high_resolution_clock::now();
	while (window.isOpen()) {

		// Calculate FPS
		auto currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(currentTime - previousTime).count());

		// Update the window for event polling, etc
		window.update();

		// Update presented scene
		if (presentedScene != nullptr) try {
			presentedScene->update(deltaTime);
		}
		catch (const std::exception& e) {
			console.error("Failed to update scene - "s + e.what());
		}

		// Render scene
		if (presentedScene != nullptr) try {
			presentedScene->render();
		}
		catch (const std::exception& e) {
			console.error("Failed to render scene - "s + e.what());
		}
		
		window.swapBuffers();
	}

	onEnd();
}
