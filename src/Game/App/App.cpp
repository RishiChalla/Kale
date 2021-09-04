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

#include "App.hpp"
#include <Game/Scenes/TitleScene.hpp>

using namespace Islands;

/**
 * Heap allocates the application
 */
Islands::Application* createApplication() {
	return new App();
}

/**
 * Creates a new app instance
 */
App::App() : Application(
	std::array<std::tuple<Font, const char*>, 1>({
		// Fonts for the game
		std::make_tuple(Font::Future, ".Islands/assets/fonts/future.ttf")
	}), std::array<std::tuple<Texture, const char*>, 0>({
		// Required Textures for the game
	}), std::array<std::tuple<Shader, sf::Shader::Type, const char*>, 0>({
		// Required Shaders for the game
	})
) {
	// Empty constructor - nothing to do here.
}

/**
 * Called when the application begins
 */
void App::onBegin() {

	// Display the title scene when the app begins
	displayScene(std::make_shared<TitleScene>(window));
}