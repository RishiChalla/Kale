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

#include <map>

#include <Kale/Events/Events.hpp>

namespace Kale {
	
	/**
	 * A list of the mappable/configurable actions for the game
	 */
	enum class Action {
		
		// All inputs which cause the player to jump are mapped to this control/action
		Jump,
		
		// All inputs which the player may use to shoot are mapped to this control/action
		Shoot,
		
		// All inputs which the player may use to use the bounce ability are mapped to this control/action
		Bounce,
		
		// All remote abilities (restore, spin, etc) are mapped to this control/action
		Remote,
		
		// Movement inputs
		MovementRight, MovementLeft, MovementDown, MovementUp
	};
	
	class Settings {
	private:

		/**
		 * The maximum FPS allowed for the window during game play
		 */
		float maxFps = 60.0f;

		/**
		 * The maximum updates per second allowed during gameplay
		 */
		float maxUps = maxFps * 2.0f;

		/**
		 * The minimum milli seconds allowed per frame, if a frame renders faster then this then execution will be
		 * paused
		 */
		float minMSpF = 1000.0f / maxFps;

		/**
		 * The minimum milli seconds allowed per update tick, if an update tick occurs faster than this then execution
		 * will be paused
		 */
		float minMSpU = 1000.0f / maxUps;

		/**
		 * The GPU used for rendering the game
		 */
		uint32_t gpuID;
		
		/**
		 * A map of all the set control keys to their action
		 */
		std::map<Key, Action> keyMap = {
			
			// Jumping
			{Key::B, Action::Jump},
			
			// Shooting
			{Key::Space, Action::Shoot},
			
			// Bounce
			{Key::C, Action::Bounce},
			
			// Remote
			{Key::V, Action::Remote},
			
			// Movement (Allowing both WASD and Arrows by default)
			{Key::W, Action::MovementUp}, {Key::Up, Action::MovementUp},
			{Key::S, Action::MovementDown}, {Key::Down, Action::MovementDown},
			{Key::A, Action::MovementLeft}, {Key::Left, Action::MovementLeft},
			{Key::D, Action::MovementRight}, {Key::Right, Action::MovementRight}
		};
		
		/**
		 * A map of all the set controller keys to their action
		 */
		std::map<ControllerButton, Action> controllerMap = {
			
			// Jumping
			{ControllerButton::A, Action::Jump},
			
			// Shooting
			{ControllerButton::B, Action::Shoot},

			// Bouncing
			{ControllerButton::Y, Action::Bounce},

			// Remote
			{ControllerButton::X, Action::Remote},

			// Movement (Both joysticks are auto mapped with no config)
			// Movement (Allowing both WASD and Arrows by default)
			{ControllerButton::DPadUp, Action::MovementUp},
			{ControllerButton::DPadDown, Action::MovementDown},
			{ControllerButton::DPadLeft, Action::MovementLeft},
			{ControllerButton::DPadRight, Action::MovementRight}
		};
		
	protected:
		
		/**
		 * Loads the settings from the config json file
		 * Should only be called once after the app is run
		 */
		void load();
		
		friend class Application;
		
	public:
		
		/**
		 * Gets the max Frames per second
		 * @returns The max Fps
		 */
		float getMaxFps() const;
		
		/**
		 * Gets the max Updates per second
		 * @returns The max Ups
		 */
		float getMaxUps() const;
		
		/**
		 * Gets the min Milli-Seconds per Frame
		 * @returns The min MSpF
		 */
		float getMinMSpF() const;
		
		/**
		 * Gets the min Milli-Seconds per Update
		 * @returns The min MSpU
		 */
		float getMinMSpU() const;

		/**
		 * Gets the used GPU id in the settings
		 * @returns The GPU Id
		 */
		uint32_t getGPUID() const;
		
		/**
		 * Sets the max FPS
		 * @param fps The max FPS to set to
		 */
		void setMaxFps(float fps);

		/**
		 * Sets the max UPS
		 * @param fps The max UPS to set to
		 */
		void setMaxUps(float ups);

		/**
		 * Sets the used GPU id in the settings
		 * @param id The id of the GPU to set to
		 */
		void setGPUID(uint32_t id);

		/**
		 * Binds the given key to a certain action
		 * @param key The key to bind
		 * @param action The action to bind to
		 */
		void updateKeyAction(Key key, Action action);

		/**
		 * Binds the given controller button to a certain action
		 * @param button The controller button to bind
		 * @param action The action to bind to
		 */
		void updateControllerAction(ControllerButton button, Action action);
		
		/**
		 * Gets the action for a key press
		 * @param key The key to get
		 * @returns the action for this key
		 */
		Action getKeyAction(Key key);

		/**
		 * Gets the action for a button
		 * @param button The button to get
		 * @returns the action for this button
		 */
		Action getControllerAction(ControllerButton button);
	};
	
	/**
	 * The main settings instance for this running application
	 */
	extern Settings settings;
}
