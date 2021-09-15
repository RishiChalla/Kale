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

// #include "RoundedRect.hpp"
// #include <Engine/Events/Events.hpp>
// #include <Engine/Application/Application.hpp>

// using namespace Islands;

// /**
//  * Creates a new rounded rectangle instance
//  * @param type The type of rounded rect to create
//  * @param shader The shader to use for rendering this rounded rect
//  * @param pos The position of the rounded rectangle (normalized to be multiplied by the window size)
//  * @param size The size of the rounded rectangle (normalized to be multiplied by the window size)
//  * @param color The color of the rounded rectangle
//  */
// RoundedRect::RoundedRect(RoundedRectType type, sf::Shader* shader, const Vector2f& pos,
// 	const Vector2f& size, const sf::Color& color) : vertices(sf::Quads, 4), shader(shader),
// 	type(type) {
	
// 	// Create the vertex array
// 	vertices[0].position = sf::Vector2f(pos.x, pos.y);
// 	vertices[1].position = sf::Vector2f(pos.x + size.x, pos.y);
// 	vertices[2].position = sf::Vector2f(pos.x + size.x, pos.y + size.y);
// 	vertices[3].position = sf::Vector2f(pos.x, pos.y + size.y);

// 	for (size_t i = 0; i < 4; i++)
// 		vertices[i].color = color;
	
// 	calculateCenters(pos, size);
// }

// /**
//  * heap allocates and calculates the centers
//  */
// void RoundedRect::calculateCenters(const Vector2f& pos, const Vector2f& size) {
// 	// Create the centers for passing to shaders
// 	switch (type) {
// 		case RoundedRectType::LeftCenter: {
// 			centers = new sf::Vector2f[1] {
// 				sf::Vector2f(
// 					pos.x + size.y / 2.0f,
// 					pos.y + size.y / 2.0f
// 				)
// 			};
// 			threshold = size.y / 2;
// 			break;
// 		}
// 		case RoundedRectType::RightCenter: {
// 			centers = new sf::Vector2f[1] {
// 				sf::Vector2f(
// 					pos.x + size.x - size.y / 2.0f,
// 					pos.y + size.y / 2.0f
// 				)
// 			};
// 			threshold = size.y / 2;
// 			break;
// 		}
// 		case RoundedRectType::TopCenter: {
// 			centers = new sf::Vector2f[1] {
// 				sf::Vector2f(
// 					pos.x + size.x / 2.0f,
// 					pos.y + size.x / 2.0f
// 				)
// 			};
// 			threshold = size.x / 2;
// 			break;
// 		}
// 		case RoundedRectType::BottomCenter: {
// 			centers = new sf::Vector2f[1] {
// 				sf::Vector2f(
// 					pos.x + size.x / 2.0f,
// 					pos.y + size.y - size.x / 2.0f
// 				)
// 			};
// 			threshold = size.x / 2;
// 			break;
// 		}
// 		case RoundedRectType::HorizontalCenters: {
// 			centers = new sf::Vector2f[2] {
// 				sf::Vector2f(
// 					pos.x + size.y / 2.0f,
// 					pos.y + size.y / 2.0f
// 				),
// 				sf::Vector2f(
// 					pos.x + size.x - size.y / 2.0f,
// 					pos.y + size.y / 2.0f
// 				)
// 			};
// 			threshold = size.y / 2;
// 			break;
// 		}
// 		case RoundedRectType::VerticalCenters: {
// 			centers = new sf::Vector2f[2] {
// 				sf::Vector2f(
// 					pos.x + size.x / 2.0f,
// 					pos.y + size.x / 2.0f
// 				),
// 				sf::Vector2f(
// 					pos.x + size.x / 2.0f,
// 					pos.y + size.y - size.x / 2.0f
// 				)
// 			};
// 			threshold = size.x / 2;
// 			break;
// 		}
// 	}
// }

// /**
//  * Copies the rounded rectangle
//  * @param other The rounded rectangle to copy from
//  */
// RoundedRect::RoundedRect(const RoundedRect& other) : vertices(other.vertices),
// 	shader(other.shader), type(other.type), threshold(other.threshold) {
	
// 	size_t nc = numCenters();
// 	centers =  new sf::Vector2f[nc];
// 	for (size_t i = 0; i < nc; i++)
// 		centers[i] = other.centers[i];
// }

// /**
//  * Copies the rounded rectangle
//  * @param other The rounded rectangle to copy from
//  */
// void RoundedRect::operator=(const RoundedRect& other) {
// 	vertices = other.vertices;
// 	shader = other.shader;
// 	type = other.type;
// 	threshold = other.threshold;

// 	size_t nc = numCenters();
// 	centers =  new sf::Vector2f[nc];
// 	for (size_t i = 0; i < nc; i++)
// 		centers[i] = other.centers[i];
// }

// /**
//  * Frees resources
//  */
// RoundedRect::~RoundedRect() {
// 	delete centers;
// }

// /**
//  * Draws the node to the window
//  * @param target The target to draw to
//  * @param states The context/states data regarding drawing
//  */
// void RoundedRect::draw(sf::RenderTarget& target, sf::RenderStates states) const {

// 	// Multiply by the transform
// 	states.transform *= getTransform();

// 	// Get the shader and set uniforms
// 	states.shader = shader;
// 	shader->setUniform("threshold", threshold);

// 	// Pass the centers
// 	shader->setUniform("center1", states.transform.transformPoint(centers[0]));
// 	if (numCenters() > 1) shader->setUniform("center2", states.transform.transformPoint(centers[1]));
	
// 	// Draw the node
// 	target.draw(vertices, states);
// }