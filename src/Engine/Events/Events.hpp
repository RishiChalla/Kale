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

#include <Engine/Math/Vector/Vector.hpp>

namespace Islands {
	
	/**
	 * Represents a single axis in a game controller
	 */
	enum class ControllerAxis {
		Left, Right
	};
	
	/**
	 * Represents a single button on the game controller
	 */
	enum class ControllerButton {
		LeftJoystick, RightJoystick,
		DPadUp, DPadRight, DPadDown, DPadLeft,
		LeftMenu, RightMenu,
		A, B, X, Y,
		R1, L1
	};
	
	/**
	 * Represents a single key on the keyboard
	 */
	enum class Key {
		Escape, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		Backtick, One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Zero, Minus, Equals, Backspace,
		Tab, Q, W, E, R, T, Y, U, I, O, P, LeftBracket, RightBracket, Backslash,
		A, S, D, F, G, H, J, K, L, Semicolon, Apostrophe, Enter,
		LeftShift, Z, X, C, V, B, N, M, Comma, Period, Slash, RightShift,
		Control, Alt, Space, Up, Down, Left, Right
	};
	
	/**
	 * Represents any class intended to handle events
	 */
	class EventHandler {
		
	public:
		
		/**
		 * Called when the event is fired
		 */
		virtual void onWindowResize(Vector2ui oldSize, Vector2ui newSize) {}
		
		/**
		 * Called when the event is fired
		 */
		virtual void onWindowLostFocus() {}
		
		/**
		 * Called when the event is fired
		 */
		virtual void onWindowGainedFocus() {}
		
		/**
		 * Called when the event is fired
		 */
		virtual void onControllerConnect(unsigned int controller) {}
		
		/**
		 * Called when the event is fired
		 */
		virtual void onControllerDisconnect(unsigned int controller) {}
		
		/**
		 * Called when the event is fired
		 */
		virtual void onControllerButtonPress(unsigned int controller, ControllerButton button) {}
		
		/**
		 * Called when the event is fired
		 */
		virtual void onControllerButtonRelease(unsigned int controller, ControllerButton button) {}
		
		/**
		 * Called when the event is fired
		 */
		virtual void onControllerHandle(unsigned int controller, ControllerAxis handle, float position) {}
		
		/**
		 * Called when the event is fired
		 */
		virtual void onControllerJoystick(unsigned int controller, ControllerAxis joystick, Vector2f position) {}
		
		/**
		 * Called when the event is fired
		 */
		virtual void onKeyPress(Key key) {}
		
		/**
		 * Called when the event is fired
		 */
		virtual void onKeyRelease(Key key) {}
		
		/**
		 * Called when the event is fired
		 */
		virtual void onMouseMove(Vector2f pos) {}
		
		/**
		 * Called when the event is fired
		 */
		virtual void onMouseScroll(float scroll) {}
		
		/**
		 * Called when the event is fired
		 */
		virtual void onLeftClick() {}
		
		/**
		 * Called when the event is fired
		 */
		virtual void onMiddleClick() {}
		
		/**
		 * Called when the event is fired
		 */
		virtual void onRightClick() {}
		
		/**
		 * Called when the event is fired
		 */
		virtual void onLeftClickRelease() {}
		
		/**
		 * Called when the event is fired
		 */
		virtual void onMiddleClickRelease() {}
		
		/**
		 * Called when the event is fired
		 */
		virtual void onRightClickRelease() {}
		
		/**
		 * Called when the event is fired
		 */
		virtual void onTouchBegin(unsigned int touch, Vector2f pos) {}
		
		/**
		 * Called when the event is fired
		 */
		virtual void onTouchMove(unsigned int touch, Vector2f pos) {}
		
		/**
		 * Called when the event is fired
		 */
		virtual void onTouchEnd(unsigned int touch) {}
		
		friend class Window;
	};
}
