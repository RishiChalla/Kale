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
	std::array<std::tuple<Font, const char*>, 18>({
		// Fonts for the game
		std::make_tuple(Font::RalewayBlack, ".Islands/assets/fonts/Raleway-Black.ttf"),
		std::make_tuple(Font::RalewayBlackItalic, ".Islands/assets/fonts/Raleway-BlackItalic.ttf"),
		std::make_tuple(Font::RalewayBold, ".Islands/assets/fonts/Raleway-Bold.ttf"),
		std::make_tuple(Font::RalewayBoldItalic, ".Islands/assets/fonts/Raleway-BoldItalic.ttf"),
		std::make_tuple(Font::RalewayExtraBold, ".Islands/assets/fonts/Raleway-ExtraBold.ttf"),
		std::make_tuple(Font::RalewayExtraBoldItalic, ".Islands/assets/fonts/Raleway-ExtraBoldItalic.ttf"),
		std::make_tuple(Font::RalewayExtraLight, ".Islands/assets/fonts/Raleway-ExtraLight.ttf"),
		std::make_tuple(Font::RalewayExtraLightItalic, ".Islands/assets/fonts/Raleway-ExtraLightItalic.ttf"),
		std::make_tuple(Font::RalewayItalic, ".Islands/assets/fonts/Raleway-Italic.ttf"),
		std::make_tuple(Font::RalewayLight, ".Islands/assets/fonts/Raleway-Light.ttf"),
		std::make_tuple(Font::RalewayLightItalic, ".Islands/assets/fonts/Raleway-LightItalic.ttf"),
		std::make_tuple(Font::RalewayMedium, ".Islands/assets/fonts/Raleway-Medium.ttf"),
		std::make_tuple(Font::RalewayMediumItalic, ".Islands/assets/fonts/Raleway-MediumItalic.ttf"),
		std::make_tuple(Font::RalewayRegular, ".Islands/assets/fonts/Raleway-Regular.ttf"),
		std::make_tuple(Font::RalewaySemiBold, ".Islands/assets/fonts/Raleway-SemiBold.ttf"),
		std::make_tuple(Font::RalewaySemiBoldItalic, ".Islands/assets/fonts/Raleway-SemiBoldItalic.ttf"),
		std::make_tuple(Font::RalewayThin, ".Islands/assets/fonts/Raleway-Thin.ttf"),
		std::make_tuple(Font::RalewayThinItalic, ".Islands/assets/fonts/Raleway-ThinItalic.ttf")
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