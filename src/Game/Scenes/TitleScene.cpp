// /*
//    Copyright 2021 Rishi Challa

//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at

// 	   http://www.apache.org/licenses/LICENSE-2.0

//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
// */

// #include "TitleScene.hpp"
// #include <Game/App/App.hpp>

// using namespace Islands;

// /**
//  * Creates a new title scene instance
//  * @param window The window to link to/draw on
//  */
// TitleScene::TitleScene(sf::RenderWindow& window) : Scene(window),
// 	testRect(RoundedRectType::HorizontalCenters, &assets->get(Shader::RoundedHorizontal),
// 		{300.0f, 300.0f}, {500.0f, 100.0f}, sf::Color::Red) {
// 	addNode(1, dynamic_cast<sf::Drawable*>(&testRect));
// 	// testRect.move(100.0f, 100.0f);
// 	testRect.setOrigin(400.0f, 400.0f);
// 	testRect.move(300.0f, 300.0f);
// 	// testRect.rotate(40.0f);
// }

// void TitleScene::onKeyPress(Key key) {
// 	if (key == Key::Left) {
// 		testRect.rotate(5.0f);
// 	}
// 	if (key == Key::Right) {
// 		testRect.rotate(-5.0f);
// 	}
// }

// /**
//  * Called to position elements
//  */
// void TitleScene::onWindowResize(const Vector2ui& oldSize, const Vector2ui& newSize) {
// 	// Call the super event handler
// 	Scene::onWindowResize(oldSize, newSize);
// }