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

#include "Events.hpp"

using namespace Islands;

/**
 * The main events handler for this application instance
 */
Events* Islands::events = nullptr;

/**
 * Creates a new events instance
 * @param window A reference to the render window
 */
Events::Events(sf::RenderWindow& window) : window(window) {
	// Empty body - all the list constructors are called automatically
}

/**
 * Handles polling events, should be called once per frame
 */
void Events::handleEvents() {
	sf::Event event;
	while (window.pollEvent(event)) {
		switch (event.type) {
			case sf::Event::EventType::Closed: {
				window.close();
				return;
			}

			case sf::Event::EventType::Resized: {
				Vector2ui size(event.size.width, event.size.height);
				for (auto listener : windowResizeListeners)
					listener->operator()(size);
				return;
			}

			case sf::Event::EventType::LostFocus: {
				for (auto listener : windowLostFocusListener)
					listener->operator()();
				return;
			}

			case sf::Event::EventType::GainedFocus: {
				for (auto listener : windowGainedFocusListener)
					listener->operator()();
				return;
			}

			case sf::Event::EventType::KeyPressed: {
				for (auto listener : keyPressListeners)
					listener->operator()(event.key.code);
				return;
			}

			case sf::Event::EventType::KeyReleased: {
				for (auto listener : keyReleaseListeners)
					listener->operator()(event.key.code);
				return;
			}

			case sf::Event::EventType::MouseWheelScrolled: {
				for (auto listener : mouseScrollListeners)
					listener->operator()(event.mouseWheel.delta);
				return;
			}

			case sf::Event::EventType::MouseButtonPressed: {
				switch (event.mouseButton.button) {
					case sf::Mouse::Button::Left: {
						for (auto listener : leftClickListeners)
							listener->operator()();
						return;
					}
					case sf::Mouse::Button::Middle: {
						for (auto listener : middleClickListeners)
							listener->operator()();
						return;
					}
					case sf::Mouse::Button::Right: {
						for (auto listener : rightClickListeners)
							listener->operator()();
						return;
					}
					default:
						return;
				}
			}

			case sf::Event::EventType::MouseButtonReleased: {
				switch (event.mouseButton.button) {
					case sf::Mouse::Button::Left: {
						for (auto listener : leftClickReleaseListeners)
							listener->operator()();
						return;
					}
					case sf::Mouse::Button::Middle: {
						for (auto listener : middleClickReleaseListeners)
							listener->operator()();
						return;
					}
					case sf::Mouse::Button::Right: {
						for (auto listener : rightClickReleaseListeners)
							listener->operator()();
						return;
					}
					default:
						return;
				}
			}

			case sf::Event::EventType::MouseMoved: {
				Vector2ui pos = Vector2i(event.mouseMove.x, event.mouseMove.y).cast<unsigned int>();
				for (auto listener : mouseMoveListeners)
					listener->operator()(pos);
				return;
			}

			case sf::Event::EventType::JoystickButtonPressed: {
				for (auto listener : controllerButtonPressListeners)
					listener->operator()(event.joystickButton.joystickId, event.joystickButton.button);
				return;
			}

			case sf::Event::EventType::JoystickButtonReleased: {
				for (auto listener : controllerButtonReleaseListeners)
					listener->operator()(event.joystickButton.joystickId, event.joystickButton.button);
				return;
			}

			case sf::Event::EventType::JoystickMoved: {
				for (auto listener : controllerHandleListeners)
					listener->operator()(event.joystickMove.joystickId, event.joystickMove.axis,
						event.joystickMove.position);
				return;
			}

			case sf::Event::EventType::JoystickConnected: {
				for (auto listener : controllerConnectListeners)
					listener->operator()(event.joystickConnect.joystickId);
				return;
			}

			case sf::Event::EventType::JoystickDisconnected: {
				for (auto listener : controllerDisconnectListeners)
					listener->operator()(event.joystickConnect.joystickId);
				return;
			}

			case sf::Event::EventType::TouchBegan: {
				Vector2ui pos(event.touch.x, event.touch.y);
				for (auto listener : touchBeginListeners)
					listener->operator()(event.touch.finger, pos);
				return;
			}

			case sf::Event::EventType::TouchMoved: {
				Vector2ui pos(event.touch.x, event.touch.y);
				for (auto listener : touchMoveListeners)
					listener->operator()(event.touch.finger, pos);
				return;
			}

			case sf::Event::EventType::TouchEnded: {
				Vector2ui pos(event.touch.x, event.touch.y);
				for (auto listener : touchEndListeners)
					listener->operator()(event.touch.finger);
				return;
			}

			default:
				return;
		}
	}
}

// Polling Events

/**
 * Gets the window size
 * @returns the window size
 */
Vector2ui Events::getWindowSize() const {
	return window.getSize();
}

/**
 * Checks whether or not a key is pressed
 * @param key The key to check for
 * @returns Whether or not the key is pressed
 */
bool Events::isKeyPressed(Key key) const {
	return sf::Keyboard::isKeyPressed(key);
}

/**
 * Gets a touch given the id
 * @param touch The touch id
 * @returns the touch
 */
Vector2i Events::getTouch(unsigned int touch) const {
	return sf::Touch::getPosition(touch);
}

/**
 * Checks whether or not a controller is connected
 * @param controller the controller
 * @returns Whether or not the controller is connected
 */
bool Events::isControllerConnected(unsigned int controller) const {
	return sf::Joystick::isConnected(controller);
}

/**
 * Checks whether or not a controller button is pressed
 * @param controller the controller
 * @param button the button to check for
 * @returns Whether or not the button is pressed
 */
bool Events::isControllerButtonPressed(unsigned int controller, unsigned int button) const {
	return sf::Joystick::isButtonPressed(controller, button);
}

/**
 * Gets a controller axis position
 * @param controller the controller
 * @param axis the axis
 * @returns the position
 */
float Events::getControllerAxisPosition(unsigned int controller, Axis axis) const {
	return sf::Joystick::getAxisPosition(controller, axis);
}

/**
 * Checks whether or not a button on the mouse is pressed
 * @param button the button on the mouse
 * @returns whether or not the button is pressed
 */
bool Events::isMouseButtonPressed(MouseButton button) const {
	return sf::Mouse::isButtonPressed(button);
}

/**
 * Gets the position of the cursor on the screen relative to the window
 * @returns the position of the cursor on the screen relative to the window
 */
Vector2i Events::getMousePos() const {
	return sf::Mouse::getPosition();
}

// Event Listeners

/**
 * Provides a callback on window resizes
 * @param handler The handler/callback function to call on the event
 */
void Events::addWindowResizeListener(std::shared_ptr<std::function<void(const Vector2ui&)>> handler) {
	windowResizeListeners.push_back(handler);
}


/**
 * Provides a callback when a controller connects
 * @param handler The handler/callback function to call on the event
 */
void Events::addControllerConnectListener(std::shared_ptr<std::function<void(unsigned int controller)>> handler) {
	controllerConnectListeners.push_back(handler);
}

/**
 * Provides a callback when a controller disconnects
 * @param handler The handler/callback function to call on the event
 */
void Events::addControllerDisconnectListener(std::shared_ptr<std::function<void(unsigned int controller)>> handler) {
	controllerDisconnectListeners.push_back(handler);
}

/**
 * Provides a callback when a button is pressed on a controller
 * @param handler The handler/callback function to call on the event
 */
void Events::addControllerButtonPressListener(std::shared_ptr<std::function<void(unsigned int controller, unsigned int button)>> handler) {
	controllerButtonPressListeners.push_back(handler);
}

/**
 * Provides a callback when a button is released on a controller
 * @param handler The handler/callback function to call on the event
 */
void Events::addControllerButtonReleaseListener(std::shared_ptr<std::function<void(unsigned int controller, unsigned int button)>> handler) {
	controllerButtonReleaseListeners.push_back(handler);
}

/**
 * Provides a callback when a controller handle is moved
 * @param handler The handler/callback function to call on the event
 */
void Events::addControllerHandleListener(std::shared_ptr<std::function<void(unsigned int controller, Axis handle, float position)>> handler) {
	controllerHandleListeners.push_back(handler);
}


/**
 * Provides a callback when a key is pressed
 * @param handler The handler/callback function to call on the event
 */
void Events::addKeyPressListener(std::shared_ptr<std::function<void(Key key)>> handler) {
	keyPressListeners.push_back(handler);
}

/**
 * Provides a callback when a key is released
 * @param handler The handler/callback function to call on the event
 */
void Events::addKeyReleaseListener(std::shared_ptr<std::function<void(Key key)>> handler) {
	keyReleaseListeners.push_back(handler);
}


/**
 * Provides a callback when the mouse moves
 * @param handler The handler/callback function to call on the event
 */
void Events::addMouseMoveListener(std::shared_ptr<std::function<void(const Vector2ui& pos)>> handler) {
	mouseMoveListeners.push_back(handler);
}

/**
 * Provides a callback when the mouse scrolls
 * @param handler The handler/callback function to call on the event
 */
void Events::addMouseScrollListener(std::shared_ptr<std::function<void(int scroll)>> handler) {
	mouseScrollListeners.push_back(handler);
}

/**
 * Provides a callback when the left button is pressed on the mouse
 * @param handler The handler/callback function to call on the event
 */
void Events::addLeftClickListener(std::shared_ptr<std::function<void()>> handler) {
	leftClickListeners.push_back(handler);
}

/**
 * Provides a callback when the middle button is pressed on the mouse
 * @param handler The handler/callback function to call on the event
 */
void Events::addMiddleClickListener(std::shared_ptr<std::function<void()>> handler) {
	middleClickListeners.push_back(handler);
}

/**
 * Provides a callback when the right button is pressed on the mouse
 * @param handler The handler/callback function to call on the event
 */
void Events::addRightClickListener(std::shared_ptr<std::function<void()>> handler) {
	rightClickListeners.push_back(handler);
}

/**
 * Provides a callback when the left button is released on the mouse
 * @param handler The handler/callback function to call on the event
 */
void Events::addLeftClickReleaseListener(std::shared_ptr<std::function<void()>> handler) {
	leftClickReleaseListeners.push_back(handler);
}

/**
 * Provides a callback when the middle button is released on the mouse
 * @param handler The handler/callback function to call on the event
 */
void Events::addMiddleClickReleaseListener(std::shared_ptr<std::function<void()>> handler) {
	middleClickReleaseListeners.push_back(handler);
}

/**
 * Provides a callback when the right button is released on the mouse
 * @param handler The handler/callback function to call on the event
 */
void Events::addRightClickReleaseListener(std::shared_ptr<std::function<void()>> handler) {
	rightClickReleaseListeners.push_back(handler);
}


/**
 * Provides a callback when a touch begins
 * @param handler The handler/callback function to call on the event
 */
void Events::addTouchBeginListener(std::shared_ptr<std::function<void(unsigned int touch, const Vector2ui& pos)>> handler) {
	touchBeginListeners.push_back(handler);
}

/**
 * Provides a callback when a touch is moved
 * @param handler The handler/callback function to call on the event
 */
void Events::addTouchMoveListener(std::shared_ptr<std::function<void(unsigned int touch, const Vector2ui& pos)>> handler) {
	touchMoveListeners.push_back(handler);
}

/**
 * Provides a callback when a touch is released
 * @param handler The handler/callback function to call on the event
 */
void Events::addTouchEndListener(std::shared_ptr<std::function<void(unsigned int touch)>> handler) {
	touchEndListeners.push_back(handler);
}

// Disable Listeners

/**
 * Removes a callback on window resizes
 * @param handler The handler/callback function to remove
 */
void Events::removeWindowResizeListener(std::shared_ptr<std::function<void(const Vector2ui&)>> handler) {
	windowResizeListeners.remove(handler);
}


/**
 * Removes a callback when a controller connects
 * @param handler The handler/callback function to remove
 */
void Events::removeControllerConnectListener(std::shared_ptr<std::function<void(unsigned int controller)>> handler) {
	controllerConnectListeners.remove(handler);
}

/**
 * Removes a callback when a controller disconnects
 * @param handler The handler/callback function to remove
 */
void Events::removeControllerDisconnectListener(std::shared_ptr<std::function<void(unsigned int controller)>> handler) {
	controllerDisconnectListeners.remove(handler);
}

/**
 * Removes a callback when a button is pressed on a controller
 * @param handler The handler/callback function to remove
 */
void Events::removeControllerButtonPressListener(std::shared_ptr<std::function<void(unsigned int controller, unsigned int button)>> handler) {
	controllerButtonPressListeners.remove(handler);
}

/**
 * Removes a callback when a button is released on a controller
 * @param handler The handler/callback function to remove
 */
void Events::removeControllerButtonReleaseListener(std::shared_ptr<std::function<void(unsigned int controller, unsigned int button)>> handler) {
	controllerButtonReleaseListeners.remove(handler);
}

/**
 * Removes a callback when a controller handle is moved
 * @param handler The handler/callback function to remove
 */
void Events::removeControllerHandleListener(std::shared_ptr<std::function<void(unsigned int controller, Axis handle, float position)>> handler) {
	controllerHandleListeners.remove(handler);
}


/**
 * Removes a callback when a key is pressed
 * @param handler The handler/callback function to remove
 */
void Events::removeKeyPressListener(std::shared_ptr<std::function<void(Key key)>> handler) {
	keyPressListeners.remove(handler);
}

/**
 * Removes a callback when a key is released
 * @param handler The handler/callback function to remove
 */
void Events::removeKeyReleaseListener(std::shared_ptr<std::function<void(Key key)>> handler) {
	keyReleaseListeners.remove(handler);
}


/**
 * Removes a callback when the mouse moves
 * @param handler The handler/callback function to remove
 */
void Events::removeMouseMoveListener(std::shared_ptr<std::function<void(const Vector2ui& pos)>> handler) {
	mouseMoveListeners.remove(handler);
}

/**
 * Removes a callback when the mouse scrolls
 * @param handler The handler/callback function to remove
 */
void Events::removeMouseScrollListener(std::shared_ptr<std::function<void(int scroll)>> handler) {
	mouseScrollListeners.remove(handler);
}

/**
 * Removes a callback when the left button is pressed on the mouse
 * @param handler The handler/callback function to remove
 */
void Events::removeLeftClickListener(std::shared_ptr<std::function<void()>> handler) {
	leftClickListeners.remove(handler);
}

/**
 * Removes a callback when the middle button is pressed on the mouse
 * @param handler The handler/callback function to remove
 */
void Events::removeMiddleClickListener(std::shared_ptr<std::function<void()>> handler) {
	middleClickListeners.remove(handler);
}

/**
 * Removes a callback when the right button is pressed on the mouse
 * @param handler The handler/callback function to remove
 */
void Events::removeRightClickListener(std::shared_ptr<std::function<void()>> handler) {
	rightClickListeners.remove(handler);
}

/**
 * Removes a callback when the left button is released on the mouse
 * @param handler The handler/callback function to remove
 */
void Events::removeLeftClickReleaseListener(std::shared_ptr<std::function<void()>> handler) {
	leftClickReleaseListeners.remove(handler);
}

/**
 * Removes a callback when the middle button is released on the mouse
 * @param handler The handler/callback function to remove
 */
void Events::removeMiddleClickReleaseListener(std::shared_ptr<std::function<void()>> handler) {
	middleClickReleaseListeners.remove(handler);
}

/**
 * Removes a callback when the right button is released on the mouse
 * @param handler The handler/callback function to remove
 */
void Events::removeRightClickReleaseListener(std::shared_ptr<std::function<void()>> handler) {
	rightClickReleaseListeners.remove(handler);
}


/**
 * Removes a callback when a touch begins
 * @param handler The handler/callback function to remove
 */
void Events::removeTouchBeginListener(std::shared_ptr<std::function<void(unsigned int touch, const Vector2ui& pos)>> handler) {
	touchBeginListeners.remove(handler);
}

/**
 * Removes a callback when a touch is moved
 * @param handler The handler/callback function to remove
 */
void Events::removeTouchMoveListener(std::shared_ptr<std::function<void(unsigned int touch, const Vector2ui& pos)>> handler) {
	touchMoveListeners.remove(handler);
}

/**
 * Removes a callback when a touch is released
 * @param handler The handler/callback function to remove
 */
void Events::removeTouchEndListener(std::shared_ptr<std::function<void(unsigned int touch)>> handler) {
	touchEndListeners.remove(handler);
}