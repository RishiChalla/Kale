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

#include "TitleScene.hpp"
#include <Game/App/App.hpp>
#include <limits>

using namespace Islands;

/**
 * Creates a new title scene instance
 * @param window The window to link to/draw on
 */
TitleScene::TitleScene(sf::RenderWindow& window) : Scene(window) {
	
	// Create the titles text label
	title.setString("Islands");
	title.setFont(assets[Font::Future]);
	title.setFillColor(sf::Color::White);
	title.setCharacterSize(50);
	title.setStyle(sf::Style::Resize);
	title.setOrigin(
		title.getLocalBounds().left + title.getLocalBounds().width / 2.0f,
		title.getLocalBounds().top + title.getLocalBounds().height / 2.0f
	);
	title.setPosition(0, camera.getViewport().top - 150.0f);

	// Add the title to the scene
	addNode(UINT_MAX, std::make_shared<sf::Text>(title));
}

/**
 * Frees resources/destructs the scene
 */
TitleScene::~TitleScene() {
	
}