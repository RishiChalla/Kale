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
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>

using namespace Islands;

static constexpr const char* settingsFilePath = ".Islands/settings.json";

Settings Islands::settings;

/**
 * Loads the settings from the config json file
 * Should only be called once after the app is run
 */
void Settings::load() {
	using namespace nlohmann;

	// Load the json from the settings file
	if (!std::filesystem::exists(".Islands"))
		std::filesystem::create_directory(".Islands");
	
	std::ifstream settingsFile(settingsFilePath);
	json settingsJson;

	// If the settings is empty, make a default version and save it
	if (settingsFile.peek() == std::ifstream::traits_type::eof()) {
		settingsFile.close();
		settingsJson = {
			{"maxFps", maxFps},
			{"maxUps", maxUps},
			{"keyMap", keyMap},
			{"controllerMap", controllerMap}
		};
		std::ifstream settingsFile(settingsFilePath);
		std::ofstream settingsFileOutput(settingsFilePath);
		settingsFileOutput << settingsJson.dump(4);
		settingsFileOutput.close();
	}
	else {
		settingsJson = json::parse(settingsFile);
		settingsFile.close();
	}

	// Load the values
	maxFps = settingsJson["maxFps"].get<float>();
	minMSpF = 1000.0f / maxFps;
	maxUps = settingsJson["maxUps"].get<float>();
	minMSpU = 1000.0f / maxUps;

	keyMap = settingsJson["keyMap"].get<std::map<Key, Action>>();
	controllerMap = settingsJson["controllerMap"].get<std::map<ControllerButton, Action>>();
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
	// Read the Json in
	using namespace nlohmann;
	json settingsJson = json::parse(std::ifstream(settingsFilePath));
	// Set the New Json Property
	settingsJson["maxFps"] = fps;

	// Save the Json
	std::ofstream settingsFile(settingsFilePath);
	settingsFile << settingsJson.dump(4);
	settingsFile.close();

	// Update the properties locally for immediate rendering change
	maxFps = fps;
	minMSpF = fps / 1000.0f;
}

/**
 * Sets the max UPS
 * @param fps The max UPS to set to
 */
void Settings::setMaxUps(float ups) {
	// Read the Json in
	using namespace nlohmann;
	json settingsJson = json::parse(std::ifstream(settingsFilePath));
	// Set the New Json Property
	settingsJson["maxUps"] = ups;

	// Save the Json
	std::ofstream settingsFile(settingsFilePath);
	settingsFile << settingsJson.dump(4);
	settingsFile.close();

	// Update the properties locally for immediate rendering change
	maxUps = ups;
	minMSpU = ups / 1000.0f;
}

/**
 * Binds the given key to a certain action
 * @param key The key to bind
 * @param action The action to bind to
 */
void Settings::updateKeyAction(Key key, Action action) {
	// Update the properties locally for immediate change
	keyMap[key] = action;
	
	// Read the Json in
	using namespace nlohmann;
	json settingsJson = json::parse(std::ifstream(settingsFilePath));
	// Set the New Json Property
	settingsJson["keyMap"] = keyMap;

	// Save the Json
	std::ofstream settingsFile(settingsFilePath);
	settingsFile << settingsJson.dump(4);
	settingsFile.close();
}

/**
 * Binds the given controller button to a certain action
 * @param button The controller button to bind
 * @param action The action to bind to
 */
void Settings::updateControllerAction(ControllerButton button, Action action) {
	// Update the properties locally for immediate change
	controllerMap[button] = action;
	
	// Read the Json in
	using namespace nlohmann;
	json settingsJson = json::parse(std::ifstream(settingsFilePath));
	// Set the New Json Property
	settingsJson["controllerMap"] = controllerMap;

	// Save the Json
	std::ofstream settingsFile(settingsFilePath);
	settingsFile << settingsJson.dump(4);
	settingsFile.close();
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
Action Settings::getControllerAction(ControllerButton button) {
	return controllerMap[button];
}