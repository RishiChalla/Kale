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
 * @param renderTime The average render time, please see Node::renderTime for documentation
 * @param updateTime The average update time, please see Node::updateTime for documentation
 */
Node::Node(float renderTime, float updateTime) : renderTime(renderTime), updateTime(updateTime) {
	// Empty Body
}

/**
 * Creates the node parent
 * @param renderTime The average render time, please see Node::renderTime for documentation
 * @param updateTime The average update time, please see Node::updateTime for documentation
 * @param priority The priority, please see Node::priority for documentation
 */
Node::Node(float renderTime, float updateTime, float priority) : renderTime(renderTime), updateTime(updateTime),
	priority(priority) {
	// Empty Body
}