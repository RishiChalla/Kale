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
	title.setFont(assets->get(Font::RalewayBlack));
	title.setFillColor(sf::Color::White);
	title.setStyle(sf::Text::Regular);

	// Add the title to the scene
	addNode(UINT_MAX, dynamic_cast<sf::Drawable*>(&title));
}

/**
 * Called to position elements
 * @param size The window size
 */
void TitleScene::onPosition(const Vector2f& size) {

	// Title label
	title.setCharacterSize(static_cast<unsigned int>(0.15 * size.y));
	title.setOrigin(
		title.getLocalBounds().left + title.getLocalBounds().width / 2.0f,
		title.getLocalBounds().top + title.getLocalBounds().height / 2.0f
	);
	title.setPosition(camera.getCenter().x, camera.getViewport().top - 0.3f * size.y);
}