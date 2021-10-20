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
#include <thread>
#include <Kale/Clock/Clock.hpp>
#include <Kale/Settings/Settings.hpp>

using namespace Kale;

/**
 * The main application instance
 */
Application* Kale::mainApp = nullptr;

/**
 * Creates a new application instance
 */
Application::Application() {
	settings.load();
}

/**
 * Frees resources and deletes the application
 */
Application::~Application() {
	
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
 * Presents a given scene
 * @param scene The scene to present
 */
void Application::presentScene(std::shared_ptr<Scene> scene) {
	presentedScene = scene;
}

/**
 * Handles updating the application in a separate thread
 */
void Application::update() {

    // Update loop
	Clock clock;
    while (window.isOpen()) {

		// Limit UPS and retrieve it
		float ups = clock.sleep(settings.getMinMSpU());
		
		// Perform updating
        onUpdate(ups);
		if (presentedScene != nullptr)
			presentedScene->update();
    }
}

/**
 * Runs the application
 */
void Application::run() {
	// Creates the window
    window.create("Islands");
    
	onBegin();

	// Create the update thread
	std::thread updateThread(&Application::update, this);

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

	// Wait for the update thread to finish
	updateThread.join();

	onEnd();
}
