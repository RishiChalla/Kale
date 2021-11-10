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

#pragma once

#include <cstddef>

namespace Kale {
	
	/**
	 * The main class for nodes, all nodes must inherit from this class
	 */
	class Node {
	private:

	protected:

		/**
		 * Renders the node
		 * @param threadNum the index of this thread, ranged 0 - numRenderThreads
		 */
		virtual void render(size_t threadNum) const = 0;

		/**
		 * Updates the node
		 * @param threadNum the index of this thread, ranged 0 - numUpdateThreads
		 * @param ups The amount of updates the current thread is doing in a second
		 */
		virtual void update(size_t threadNum, float ups) = 0;

		/**
		 * Creates the node parent
		 */
		Node();

		/**
		 * Creates the node parent
		 * @param renderTime The average render time, please see Node::renderTime for documentation
		 * @param updateTime The average update time, please see Node::updateTime for documentation
		 */
		Node(float renderTime, float updateTime);

		/**
		 * Creates the node parent
		 * @param renderTime The average render time, please see Node::renderTime for documentation
		 * @param updateTime The average update time, please see Node::updateTime for documentation
		 * @param priority The priority, please see Node::priority for documentation
		 */
		Node(float renderTime, float updateTime, float priority);

		friend class Scene;
	
	public:

		/**
		 * The priority of updating this node
		 * Update order within a thread is based on priority
		 * 
		 * The default is 0.0
		 */
		const float priority = 0.0f;

		/**
		 * The amount of time this node takes to render on average
		 * 
		 * You can gain this statistic using the Kale Editor callibration tools.
		 * This measure is device specific, when developing on multiple devices it is recommended that you use
		 * a low powered device for callibration, the same device must be used for callibrating all nodes
		 * to avoid potential bias
		 */
		const float renderTime = -1.0f;

		/**
		 * The amount of time this node takes to update on average
		 * 
		 * You can gain this statistic using the Kale Editor callibration tools.
		 * This measure is device specific, when developing on multiple devices it is recommended that you use
		 * a low powered device for callibration, the same device must be used for callibrating all nodes
		 * to avoid potential bias 
		 */
		const float updateTime = -1.0f;
	};
}