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

#include "Settings.hpp"

using namespace Islands;

Settings Islands::settings;

/**
 * Loads the settings from the config json file
 * Should only be called once after the app is run
 */
void Settings::load() {
	// TODO - Implement
}

/**
 * Gets the max Frames per second
 * @returns The max Fps
 */
float Settings::getMaxFps() const {
	// TODO - Implement
	return maxFps;
}

/**
 * Gets the max Updates per second
 * @returns The max Ups
 */
float Settings::getMaxUps() const {
	// TODO - Implement
	return maxUps;
}

/**
 * Gets the min Milli-Seconds per Frame
 * @returns The min MSpF
 */
float Settings::getMinMSpF() const {
	// TODO - Implement
	return minMSpF;
}

/**
 * Gets the min Milli-Seconds per Update
 * @returns The min MSpU
 */
float Settings::getMinMSpU() const {
	// TODO - Implement
	return minMSpU;
}

/**
 * Sets the max FPS
 * @param fps The max FPS to set to
 */
void Settings::setMaxFps(float fps) {
	// TODO - Implement
}

/**
 * Sets the max UPS
 * @param fps The max UPS to set to
 */
void Settings::setMaxUps(float ups) {
	// TODO - Implement
}

/**
 * Gets the action for a key press
 * @param key The key to get
 * @returns the action for this key
 */
Action Settings::getKeyAction(Key key) {
	return keyMap[key];
}

/**
 * Gets the action for a button
 * @param button The button to get
 * @returns the action for this button
 */
Action Settings::getControllerButtonAction(ControllerButton button) {
	return controllerMap[button];
}