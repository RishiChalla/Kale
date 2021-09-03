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
#include <Engine/Events/Events.hpp>
#include <thread>

using namespace Islands;

/**
 * The main application instance
 */
Application* Islands::mainApp = nullptr;

/**
 * Creates a new application instance
 */
Application::Application() {
	events = new Events(window);
}

/**
 * Frees resources and deletes the application
 */
Application::~Application() {
	delete events;
}

/**
 * Starts displaying and updating a scene
 * @param scene The scene to display
 */
void Application::displayScene(std::shared_ptr<Scene> scene) {
	currentScene = scene;
}

/**
 * Handles updating the application in a separate thread
 */
void Application::update() {

	// Update loop
	while (window.isOpen()) {
		
		// Update the current scene
		if (currentScene != nullptr)
			currentScene->update();
	}
}

/**
 * Runs the application
 */
void Application::run() {
	// Creates the window
	window.create(sf::VideoMode(800, 600), "Islands");

	// Create the update thread
	std::thread updateThread(&Application::update, this);

	// Render loop
	while (window.isOpen()) {
		
		// Handle events
		events->handleEvents();

		// Render the scene
		if (currentScene != nullptr)
			currentScene->render();
		else {

			// Just use a black window as a default
			window.clear(sf::Color::Black);
			window.display();
		}
	}

	// Wait for the update thread to finish
	updateThread.join();
}