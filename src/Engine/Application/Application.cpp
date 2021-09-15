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

using namespace Islands;

/**
 * The main application instance
 */
Application* Islands::mainApp = nullptr;

/**
 * Creates a new application instance
 */
Application::Application() {
	
}

/**
 * Frees resources and deletes the application
 */
Application::~Application() {
	
}

/**
 * Handles updating the application in a separate thread
 */
void Application::update() {

    // Update loop
    while (window.isOpen()) {
        onUpdate();
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
	// Commented out for now - prevent CPU overheating
	// TODO - Fix this by adding UPS and max UPS to avoid 100% CPU thread usage
	// std::thread updateThread(&Application::update, this);

	// Render loop
    while (window.isOpen()) {
		
		// TODO - Handle events

		// TODO - Designate rendering to scene
    }

	// Wait for the update thread to finish
	// Commented out for now - prevent CPU overheating
	// updateThread.join();

	onEnd();
}
