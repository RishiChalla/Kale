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

#include <Engine/Events/Events.hpp>

namespace Islands {
	
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
		float maxFps = 60.0;
		float maxUps = maxFps * 2.0;
		float minMSpF = 1000.0 / maxFps;
		float minMSpU = 1000.0 / maxUps;
		
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
		
		std::map<ControllerButton, Action> controllerMap = {
			
			// Jumping
			{ControllerButton::A, Action::Jump},
			
			// Shooting
			{}
		};
		
	protected:
		
		void load();
		
		friend class Application;
		
	public:
		
		float getMaxFps() const;
		float getMaxUps() const;
		float getMinMSpF() const;
		float getMinMSpU() const;
		
		void setMaxFps(float fps);
		void setMaxUps(float ups);
		
		Key getKeyControl(Action action);
		ControllerButton getButtonControl(Action action);
	};
	
	extern Settings settings;
}
