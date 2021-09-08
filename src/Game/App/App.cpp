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
	}), std::array<std::tuple<Shader, const char*, const char*>, 16>({
		// Required Shaders for the game

		// Rounded Rect Shaders
		std::make_tuple(Shader::RoundedBottom, ".Islands/assets/shaders/roundedRectVert.glsl",
			".Islands/assets/shaders/roundedRect/roundedBottom.glsl"),
		std::make_tuple(Shader::RoundedHorizontal, ".Islands/assets/shaders/roundedRectVert.glsl",
			".Islands/assets/shaders/roundedRect/roundedHorizontal.glsl"),
		std::make_tuple(Shader::RoundedLeft, ".Islands/assets/shaders/roundedRectVert.glsl",
			".Islands/assets/shaders/roundedRect/roundedLeft.glsl"),
		std::make_tuple(Shader::RoundedRight, ".Islands/assets/shaders/roundedRectVert.glsl",
			".Islands/assets/shaders/roundedRect/roundedRight.glsl"),
		std::make_tuple(Shader::RoundedTop, ".Islands/assets/shaders/roundedRectVert.glsl",
			".Islands/assets/shaders/roundedRect/roundedTop.glsl"),
		std::make_tuple(Shader::RoundedVertical, ".Islands/assets/shaders/roundedRectVert.glsl",
			".Islands/assets/shaders/roundedRect/roundedVertical.glsl"),

		// Rounded Rect Inverse Shaders
		std::make_tuple(Shader::InverseHorizontal, ".Islands/assets/shaders/roundedRectVert.glsl",
			".Islands/assets/shaders/roundedRectInverse/inverseHorizontal.glsl"),
		std::make_tuple(Shader::InverseVertical, ".Islands/assets/shaders/roundedRectVert.glsl",
			".Islands/assets/shaders/roundedRectInverse/inverseVertical.glsl"),
		std::make_tuple(Shader::InverseBottom, ".Islands/assets/shaders/roundedRectVert.glsl",
			".Islands/assets/shaders/roundedRectInverse/inverseBottom.glsl"),
		std::make_tuple(Shader::InverseTop, ".Islands/assets/shaders/roundedRectVert.glsl",
			".Islands/assets/shaders/roundedRectInverse/inverseTop.glsl"),
		std::make_tuple(Shader::InverseLeft, ".Islands/assets/shaders/roundedRectVert.glsl",
			".Islands/assets/shaders/roundedRectInverse/inverseLeft.glsl"),
		std::make_tuple(Shader::InverseRight, ".Islands/assets/shaders/roundedRectVert.glsl",
			".Islands/assets/shaders/roundedRectInverse/inverseRight.glsl"),
		
		// Rounded Rect Combined Shaders
		std::make_tuple(Shader::CombinedBottomCap, ".Islands/assets/shaders/roundedRectVert.glsl",
			".Islands/assets/shaders/roundedRectCombined/combinedBottomCap.glsl"),
		std::make_tuple(Shader::CombinedTopCap, ".Islands/assets/shaders/roundedRectVert.glsl",
			".Islands/assets/shaders/roundedRectCombined/combinedTopCap.glsl"),
		std::make_tuple(Shader::CombinedLeftCap, ".Islands/assets/shaders/roundedRectVert.glsl",
			".Islands/assets/shaders/roundedRectCombined/combinedLeftCap.glsl"),
		std::make_tuple(Shader::CombinedRightCap, ".Islands/assets/shaders/roundedRectVert.glsl",
			".Islands/assets/shaders/roundedRectCombined/combinedRightCap.glsl")
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