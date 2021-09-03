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

#pragma once

#include <SFML/Graphics.hpp>
#include <Engine/Math/Vector/Vector.hpp>
#include <list>
#include <memory>
#include <functional>

namespace Islands {

	/**
	 * Represents a single key
	 */
	using Key = sf::Keyboard::Key;

	/**
	 * Represents a single controller axis;
	 */
	using Axis = sf::Joystick::Axis;

	/**
	 * Represents a single mouse button
	 */
	using MouseButton = sf::Mouse::Button;

	/**
	 * The main events class
	 */
	class Events {

	private:

		/**
		 * The main window reference
		 */
		sf::RenderWindow& window;
		
		/**
		 * Stores all the event listeners in a linked list
		 */
		std::list<std::shared_ptr<std::function<void(const Vector2ui&)>>> windowResizeListeners;

		/**
		 * Stores all the event listeners in a linked list
		 */
		std::list<std::shared_ptr<std::function<void()>>> windowLostFocusListener;

		/**
		 * Stores all the event listeners in a linked list
		 */
		std::list<std::shared_ptr<std::function<void()>>> windowGainedFocusListener;
		
		/**
		 * Stores all the event listeners in a linked list
		 */
		std::list<std::shared_ptr<std::function<void(unsigned int controller)>>> controllerConnectListeners;
		
		/**
		 * Stores all the event listeners in a linked list
		 */
		std::list<std::shared_ptr<std::function<void(unsigned int controller)>>> controllerDisconnectListeners;
		
		/**
		 * Stores all the event listeners in a linked list
		 */
		std::list<std::shared_ptr<std::function<void(unsigned int controller, unsigned int button)>>> controllerButtonPressListeners;
		
		/**
		 * Stores all the event listeners in a linked list
		 */
		std::list<std::shared_ptr<std::function<void(unsigned int controller, unsigned int button)>>> controllerButtonReleaseListeners;
		
		/**
		 * Stores all the event listeners in a linked list
		 */
		std::list<std::shared_ptr<std::function<void(unsigned int controller, Axis handle, float position)>>> controllerHandleListeners;
		
		/**
		 * Stores all the event listeners in a linked list
		 */
		std::list<std::shared_ptr<std::function<void(Key key)>>> keyPressListeners;
		
		/**
		 * Stores all the event listeners in a linked list
		 */
		std::list<std::shared_ptr<std::function<void(Key key)>>> keyReleaseListeners;
		
		/**
		 * Stores all the event listeners in a linked list
		 */
		std::list<std::shared_ptr<std::function<void(const Vector2ui& pos)>>> mouseMoveListeners;
		
		/**
		 * Stores all the event listeners in a linked list
		 */
		std::list<std::shared_ptr<std::function<void(int scroll)>>> mouseScrollListeners;
		
		/**
		 * Stores all the event listeners in a linked list
		 */
		std::list<std::shared_ptr<std::function<void()>>> leftClickListeners;
		
		/**
		 * Stores all the event listeners in a linked list
		 */
		std::list<std::shared_ptr<std::function<void()>>> middleClickListeners;
		
		/**
		 * Stores all the event listeners in a linked list
		 */
		std::list<std::shared_ptr<std::function<void()>>> rightClickListeners;
		
		/**
		 * Stores all the event listeners in a linked list
		 */
		std::list<std::shared_ptr<std::function<void()>>> leftClickReleaseListeners;
		
		/**
		 * Stores all the event listeners in a linked list
		 */
		std::list<std::shared_ptr<std::function<void()>>> middleClickReleaseListeners;
		
		/**
		 * Stores all the event listeners in a linked list
		 */
		std::list<std::shared_ptr<std::function<void()>>> rightClickReleaseListeners;
		
		/**
		 * Stores all the event listeners in a linked list
		 */
		std::list<std::shared_ptr<std::function<void(unsigned int touch, const Vector2ui& pos)>>> touchBeginListeners;
		
		/**
		 * Stores all the event listeners in a linked list
		 */
		std::list<std::shared_ptr<std::function<void(unsigned int touch, const Vector2ui& pos)>>> touchMoveListeners;
		
		/**
		 * Stores all the event listeners in a linked list
		 */
		std::list<std::shared_ptr<std::function<void(unsigned int touch)>>> touchEndListeners;

	protected:

		/**
		 * Creates a new events instance
		 * @param window A reference to the render window
		 */
		Events(sf::RenderWindow& window);

		/**
		 * Handles polling events, should be called once per frame
		 */
		void handleEvents();

		friend class Application;

	public:

		// Events class doesn't support copying/moving
		Events(const Events& other) = delete;
		Events(Events&& other) = delete;
		void operator=(const Events& other) = delete;
		void operator=(Events&& other) = delete;

		// Polling Events

		/**
		 * Gets the window size
		 * @returns the window size
		 */
		Vector2ui getWindowSize() const;

		/**
		 * Checks whether or not a key is pressed
		 * @param key The key to check for
		 * @returns Whether or not the key is pressed
		 */
		bool isKeyPressed(Key key) const;

		/**
		 * Gets a touch given the id
		 * @param touch The touch id
		 * @returns the touch
		 */
		Vector2i getTouch(unsigned int touch) const;

		/**
		 * Checks whether or not a controller is connected
		 * @param controller the controller
		 * @returns Whether or not the controller is connected
		 */
		bool isControllerConnected(unsigned int controller) const;

		/**
		 * Checks whether or not a controller button is pressed
		 * @param controller the controller
		 * @param button the button to check for
		 * @returns Whether or not the button is pressed
		 */
		bool isControllerButtonPressed(unsigned int controller, unsigned int button) const;

		/**
		 * Gets a controller axis position
		 * @param controller the controller
		 * @param axis the axis
		 * @returns the position
		 */
		float getControllerAxisPosition(unsigned int controller, Axis axis) const;

		/**
		 * Checks whether or not a button on the mouse is pressed
		 * @param button the button on the mouse
		 * @returns whether or not the button is pressed
		 */
		bool isMouseButtonPressed(MouseButton button) const;

		/**
		 * Gets the position of the cursor on the screen relative to the window
		 * @returns the position of the cursor on the screen relative to the window
		 */
		Vector2i getMousePos() const;

		// Event Listeners
		
		/**
		 * Provides a callback on window resizes
		 * @param handler The handler/callback function to call on the event
		 */
		void addWindowResizeListener(std::shared_ptr<std::function<void(const Vector2ui&)>> handler);

		/**
		 * Provides a callback on window lost focus
		 * @param handler The handler/callback function to call on the event
		 */
		void addWindowLostFocusListener(std::shared_ptr<std::function<void()>> handler);

		/**
		 * Provides a callback on window focus
		 * @param handler The handler/callback function to call on the event
		 */
		void addWindowGainFocusListener(std::shared_ptr<std::function<void()>> handler);

		
		/**
		 * Provides a callback when a controller connects
		 * @param handler The handler/callback function to call on the event
		 */
		void addControllerConnectListener(std::shared_ptr<std::function<void(unsigned int controller)>> handler);
		
		/**
		 * Provides a callback when a controller disconnects
		 * @param handler The handler/callback function to call on the event
		 */
		void addControllerDisconnectListener(std::shared_ptr<std::function<void(unsigned int controller)>> handler);
		
		/**
		 * Provides a callback when a button is pressed on a controller
		 * @param handler The handler/callback function to call on the event
		 */
		void addControllerButtonPressListener(std::shared_ptr<std::function<void(unsigned int controller, unsigned int button)>> handler);
		
		/**
		 * Provides a callback when a button is released on a controller
		 * @param handler The handler/callback function to call on the event
		 */
		void addControllerButtonReleaseListener(std::shared_ptr<std::function<void(unsigned int controller, unsigned int button)>> handler);
		
		/**
		 * Provides a callback when a controller handle is moved
		 * @param handler The handler/callback function to call on the event
		 */
		void addControllerHandleListener(std::shared_ptr<std::function<void(unsigned int controller, Axis handle, float position)>> handler);

		
		/**
		 * Provides a callback when a key is pressed
		 * @param handler The handler/callback function to call on the event
		 */
		void addKeyPressListener(std::shared_ptr<std::function<void(Key key)>> handler);
		
		/**
		 * Provides a callback when a key is released
		 * @param handler The handler/callback function to call on the event
		 */
		void addKeyReleaseListener(std::shared_ptr<std::function<void(Key key)>> handler);

		
		/**
		 * Provides a callback when the mouse moves
		 * @param handler The handler/callback function to call on the event
		 */
		void addMouseMoveListener(std::shared_ptr<std::function<void(const Vector2ui& pos)>> handler);
		
		/**
		 * Provides a callback when the mouse scrolls
		 * @param handler The handler/callback function to call on the event
		 */
		void addMouseScrollListener(std::shared_ptr<std::function<void(int scroll)>> handler);
		
		/**
		 * Provides a callback when the left button is pressed on the mouse
		 * @param handler The handler/callback function to call on the event
		 */
		void addLeftClickListener(std::shared_ptr<std::function<void()>> handler);
		
		/**
		 * Provides a callback when the middle button is pressed on the mouse
		 * @param handler The handler/callback function to call on the event
		 */
		void addMiddleClickListener(std::shared_ptr<std::function<void()>> handler);
		
		/**
		 * Provides a callback when the right button is pressed on the mouse
		 * @param handler The handler/callback function to call on the event
		 */
		void addRightClickListener(std::shared_ptr<std::function<void()>> handler);
		
		/**
		 * Provides a callback when the left button is released on the mouse
		 * @param handler The handler/callback function to call on the event
		 */
		void addLeftClickReleaseListener(std::shared_ptr<std::function<void()>> handler);
		
		/**
		 * Provides a callback when the middle button is released on the mouse
		 * @param handler The handler/callback function to call on the event
		 */
		void addMiddleClickReleaseListener(std::shared_ptr<std::function<void()>> handler);
		
		/**
		 * Provides a callback when the right button is released on the mouse
		 * @param handler The handler/callback function to call on the event
		 */
		void addRightClickReleaseListener(std::shared_ptr<std::function<void()>> handler);

		
		/**
		 * Provides a callback when a touch begins
		 * @param handler The handler/callback function to call on the event
		 */
		void addTouchBeginListener(std::shared_ptr<std::function<void(unsigned int touch, const Vector2ui& pos)>> handler);
		
		/**
		 * Provides a callback when a touch is moved
		 * @param handler The handler/callback function to call on the event
		 */
		void addTouchMoveListener(std::shared_ptr<std::function<void(unsigned int touch, const Vector2ui& pos)>> handler);
		
		/**
		 * Provides a callback when a touch is released
		 * @param handler The handler/callback function to call on the event
		 */
		void addTouchEndListener(std::shared_ptr<std::function<void(unsigned int touch)>> handler);

		// Disable Listeners
		
		/**
		 * Removes a callback on window resizes
		 * @param handler The handler/callback function to remove
		 */
		void removeWindowResizeListener(std::shared_ptr<std::function<void(const Vector2ui&)>> handler);

		/**
		 * Removes a callback on window lost focus
		 * @param handler The handler/callback function to call on the event
		 */
		void removeWindowLostFocusListener(std::shared_ptr<std::function<void()>> handler);

		/**
		 * Removes a callback on window focus
		 * @param handler The handler/callback function to call on the event
		 */
		void removeWindowGainFocusListener(std::shared_ptr<std::function<void()>> handler);

		
		/**
		 * Removes a callback when a controller connects
		 * @param handler The handler/callback function to remove
		 */
		void removeControllerConnectListener(std::shared_ptr<std::function<void(unsigned int controller)>> handler);
		
		/**
		 * Removes a callback when a controller disconnects
		 * @param handler The handler/callback function to remove
		 */
		void removeControllerDisconnectListener(std::shared_ptr<std::function<void(unsigned int controller)>> handler);
		
		/**
		 * Removes a callback when a button is pressed on a controller
		 * @param handler The handler/callback function to remove
		 */
		void removeControllerButtonPressListener(std::shared_ptr<std::function<void(unsigned int controller, unsigned int button)>> handler);
		
		/**
		 * Removes a callback when a button is released on a controller
		 * @param handler The handler/callback function to remove
		 */
		void removeControllerButtonReleaseListener(std::shared_ptr<std::function<void(unsigned int controller, unsigned int button)>> handler);
		
		/**
		 * Removes a callback when a controller handle is moved
		 * @param handler The handler/callback function to remove
		 */
		void removeControllerHandleListener(std::shared_ptr<std::function<void(unsigned int controller, Axis handle, float position)>> handler);

		
		/**
		 * Removes a callback when a key is pressed
		 * @param handler The handler/callback function to remove
		 */
		void removeKeyPressListener(std::shared_ptr<std::function<void(Key key)>> handler);
		
		/**
		 * Removes a callback when a key is released
		 * @param handler The handler/callback function to remove
		 */
		void removeKeyReleaseListener(std::shared_ptr<std::function<void(Key key)>> handler);

		
		/**
		 * Removes a callback when the mouse moves
		 * @param handler The handler/callback function to remove
		 */
		void removeMouseMoveListener(std::shared_ptr<std::function<void(const Vector2ui& pos)>> handler);
		
		/**
		 * Removes a callback when the mouse scrolls
		 * @param handler The handler/callback function to remove
		 */
		void removeMouseScrollListener(std::shared_ptr<std::function<void(int scroll)>> handler);
		
		/**
		 * Removes a callback when the left button is pressed on the mouse
		 * @param handler The handler/callback function to remove
		 */
		void removeLeftClickListener(std::shared_ptr<std::function<void()>> handler);
		
		/**
		 * Removes a callback when the middle button is pressed on the mouse
		 * @param handler The handler/callback function to remove
		 */
		void removeMiddleClickListener(std::shared_ptr<std::function<void()>> handler);
		
		/**
		 * Removes a callback when the right button is pressed on the mouse
		 * @param handler The handler/callback function to remove
		 */
		void removeRightClickListener(std::shared_ptr<std::function<void()>> handler);
		
		/**
		 * Removes a callback when the left button is released on the mouse
		 * @param handler The handler/callback function to remove
		 */
		void removeLeftClickReleaseListener(std::shared_ptr<std::function<void()>> handler);
		
		/**
		 * Removes a callback when the middle button is released on the mouse
		 * @param handler The handler/callback function to remove
		 */
		void removeMiddleClickReleaseListener(std::shared_ptr<std::function<void()>> handler);
		
		/**
		 * Removes a callback when the right button is released on the mouse
		 * @param handler The handler/callback function to remove
		 */
		void removeRightClickReleaseListener(std::shared_ptr<std::function<void()>> handler);

		
		/**
		 * Removes a callback when a touch begins
		 * @param handler The handler/callback function to remove
		 */
		void removeTouchBeginListener(std::shared_ptr<std::function<void(unsigned int touch, const Vector2ui& pos)>> handler);
		
		/**
		 * Removes a callback when a touch is moved
		 * @param handler The handler/callback function to remove
		 */
		void removeTouchMoveListener(std::shared_ptr<std::function<void(unsigned int touch, const Vector2ui& pos)>> handler);
				
		/**
		 * Removes a callback when a touch is released
		 * @param handler The handler/callback function to remove
		 */
		void removeTouchEndListener(std::shared_ptr<std::function<void(unsigned int touch)>> handler);
	};

	/**
	 * The main events handler for this application instance
	 */
	extern Events* events;
}