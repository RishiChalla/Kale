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

#include <Kale/Vulkan/GraphicsPipeline/GraphicsPipeline.hpp>
#include <Kale/Vulkan/CommandBuffer/CommandBuffer.hpp>
#include <Kale/Vulkan/Renderer/Renderer.hpp>

namespace Kale {

	/**
	 * Forward declaration of scene class
	 */
	class Scene;
	
	/**
	 * The main class for nodes, all nodes must inherit from this class
	 */
	class Node {
	private:

	protected:

		/**
		 * Renders the node
		 * @param renderer The renderer to render to
		 */
		virtual void render() = 0;

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
		 * @param updateTime The average update time, please see Node::updateTime for documentation
		 */
		Node(float updateTime);

		/**
		 * Creates the node parent
		 * @param updateTime The average update time, please see Node::updateTime for documentation
		 * @param priority The priority, please see Node::priority for documentation
		 */
		Node(float updateTime, float priority);

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
		 * The amount of time this node takes to update on average
		 * 
		 * You can gain this statistic using the Kale Editor calibration tools.
		 * This measure is device specific, when developing on multiple devices it is recommended that you use
		 * a low powered device for calibration, the same device must be used for calibrating all nodes
		 * to avoid potential bias 
		 */
		const float updateTime = -1.0f;
	};
}
