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
#include <Events/Events.hpp>

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
 * Runs the application
 */
void Application::run() {
	// Creates the window
	window.create(sf::VideoMode(800, 600), "Islands");

	// update loop
	while (window.isOpen()) {
		
		// Handle events
		events->handleEvents();

		// TODO - delete this and replace it with scene rendering
		window.clear(sf::Color::Blue);
		window.display();
	}
}