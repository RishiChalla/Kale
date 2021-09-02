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
#include <Math/Vector/Vector.hpp>

namespace Islands {

	/**
	 * Represents an event type
	 */
	enum class EventType {
		// Controller
		ControllerConnectEvent, ControllerDisconnectEvent, ControllerJoystickEvent,
		ControllerHandleEvent, ControllerButtonEvent,

		// Keyboard
		KeyPressEvent, KeyReleaseEvent,

		// Mouse
		LeftClickBeginEvent, RightClickBeginEvent, MiddleClickBeginEvent,
		LeftClickReleaseEvent, RightClickReleaseEvent, MiddleClickReleaseEvent,

		MouseScrollEvent, MouseMoveEvent,

		// Touch
		TouchBeginEvent, TouchMoveEvent, TouchEndEvent
	};

	struct DirectionalPad {
		bool up;
		bool down;
		bool left;
		bool right;
	}

	struct Controller {
		Vector2f leftJoystick;
		Vector2f rightJoystick;
		DirectionalPad dpad;
		DirectionalPad buttons;
		bool lMenu;
		bool rMenu;
	};

	struct Keyboard {

	};

	struct Mouse {

	};

	struct Touch {

	};

	/**
	 * The main events class
	 */
	class Events {

	private:

		/**
		 * The main window reference
		 */
		sf::RenderWindow& window;

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

	};

	/**
	 * The main events handler for this application instance
	 */
	extern Events* events;
}