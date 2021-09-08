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
 * Frees resources and deletes the application
 */
Application::~Application() {
	delete events;
	delete assets;
}

/**
 * Starts displaying and updating a scene
 * @param scene The scene to display
 */
void Application::displayScene(std::shared_ptr<Scene> scene) {

	// The scene isn't defined, and won't be displayed
	if (scene == nullptr) {
		error("Unable to display scene of nullptr - Aborting");
		return;
	}

	// Call scene begin/end methods
	if (currentScene != nullptr)
		currentScene->end();
	scene->begin();

	// Call the scene change listener
	onSceneChange(currentScene, scene);

	// Change the scene
	currentScene = scene;
}

/**
 * Handles updating the application in a separate thread
 */
void Application::update() {

	// Update loop
	while (window.isOpen()) {

		onUpdate();
		
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
	{
		sf::ContextSettings settings;
		settings.antialiasingLevel = 4;
		window.create(sf::VideoMode(800, 600), "Islands", sf::Style::Default, settings);
	}

	onBegin();

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

	onEnd();
}

// --------------------------------- Events ---------------------------------

/**
 * Called when the event is fired
 */
void Application::onWindowResize(const Vector2ui& oldSize, const Vector2ui& newSize) {
	if (currentScene != nullptr)
		currentScene->onWindowResize(oldSize, newSize);
}

/**
 * Called when the event is fired
 */
void Application::onWindowLostFocus() {
	if (currentScene != nullptr)
		currentScene->onWindowLostFocus();
}

/**
 * Called when the event is fired
 */
void Application::onWindowGainedFocus() {
	if (currentScene != nullptr)
		currentScene->onWindowGainedFocus();
}

/**
 * Called when the event is fired
 */
void Application::onControllerConnect(unsigned int controller) {
	if (currentScene != nullptr)
		currentScene->onControllerConnect(controller);
}

/**
 * Called when the event is fired
 */
void Application::onControllerDisconnect(unsigned int controller) {
	if (currentScene != nullptr)
		currentScene->onControllerDisconnect(controller);
}

/**
 * Called when the event is fired
 */
void Application::onControllerButtonPress(unsigned int controller, unsigned int button) {
	if (currentScene != nullptr)
		currentScene->onControllerButtonPress(controller, button);
}

/**
 * Called when the event is fired
 */
void Application::onControllerButtonRelease(unsigned int controller, unsigned int button) {
	if (currentScene != nullptr)
		currentScene->onControllerButtonRelease(controller, button);
}

/**
 * Called when the event is fired
 */
void Application::onControllerHandle(unsigned int controller, Axis handle, float position) {
	if (currentScene != nullptr)
		currentScene->onControllerHandle(controller, handle, position);
}

/**
 * Called when the event is fired
 */
void Application::onKeyPress(Key key) {
	if (currentScene != nullptr)
		currentScene->onKeyPress(key);
}

/**
 * Called when the event is fired
 */
void Application::onKeyRelease(Key key) {
	if (currentScene != nullptr)
		currentScene->onKeyRelease(key);
}

/**
 * Called when the event is fired
 */
void Application::onMouseMove(const Vector2ui& pos) {
	if (currentScene != nullptr)
		currentScene->onMouseMove(pos);
}

/**
 * Called when the event is fired
 */
void Application::onMouseScroll(int scroll) {
	if (currentScene != nullptr)
		currentScene->onMouseScroll(scroll);
}

/**
 * Called when the event is fired
 */
void Application::onLeftClick() {
	if (currentScene != nullptr)
		currentScene->onLeftClick();
}

/**
 * Called when the event is fired
 */
void Application::onMiddleClick() {
	if (currentScene != nullptr)
		currentScene->onMiddleClick();
}

/**
 * Called when the event is fired
 */
void Application::onRightClick() {
	if (currentScene != nullptr)
		currentScene->onRightClick();
}

/**
 * Called when the event is fired
 */
void Application::onLeftClickRelease() {
	if (currentScene != nullptr)
		currentScene->onLeftClickRelease();
}

/**
 * Called when the event is fired
 */
void Application::onMiddleClickRelease() {
	if (currentScene != nullptr)
		currentScene->onMiddleClickRelease();
}

/**
 * Called when the event is fired
 */
void Application::onRightClickRelease() {
	if (currentScene != nullptr)
		currentScene->onRightClickRelease();
}

/**
 * Called when the event is fired
 */
void Application::onTouchBegin(unsigned int touch, const Vector2ui& pos) {
	if (currentScene != nullptr)
		currentScene->onTouchBegin(touch, pos);
}

/**
 * Called when the event is fired
 */
void Application::onTouchMove(unsigned int touch, const Vector2ui& pos) {
	if (currentScene != nullptr)
		currentScene->onTouchMove(touch, pos);
}

/**
 * Called when the event is fired
 */
void Application::onTouchEnd(unsigned int touch) {
	if (currentScene != nullptr)
		currentScene->onTouchEnd(touch);
}