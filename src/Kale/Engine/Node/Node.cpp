/*
   Copyright 2022 Rishi Challa

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

#include "Node.hpp"

using namespace Kale;

/**
 * Creates the node parent
 */
Node::Node() {
	// Empty Body
}

/**
 * Creates the node parent
 * @param updateTime The average update time, please see Node::updateTime for documentation
 */
Node::Node(float updateTime) : updateTime(updateTime) {
	// Empty Body
}

/**
 * Called when the node is added to the scene, guaranteed to be called before any updates & renders
 * and from the main thread.
 * @param scene The scene the node has been added to
 */
void Node::begin(const Scene& scene) {
	// Empty Body
}

/**
 * Renders the node
 * @param camera The camera to render with
 */
void Node::render(const Camera& camera, float deltaTime) const {
	// Empty Body
}

/**
 * Called on update, perfect place to do any physics updating, game logic, etc
 * @param threadNum the index of the thread this update is called on
 * @param scene The scene being updated to
 * @param deltaTime The duration of the last frame in microseconds
 */
void Node::update(size_t threadNum, const Scene& scene, float deltaTime) {
	// Empty Body
}

/**
 * Called prior to update, perfect place to do things such as updating the bounding box, etc
 * @param threadNum the index of the thread this update is called on
 * @param scene The scene being updated to
 * @param deltaTime The duration of the last frame in microseconds
 */
void Node::preUpdate(size_t threadNum, const Scene& scene, float deltaTime) {
	// Empty Body
}

/**
 * Called when the node is removed from the scene, guaranteed to be called from the main thread
 */
void Node::end(const Scene& scene) {
	// Empty Body
}
