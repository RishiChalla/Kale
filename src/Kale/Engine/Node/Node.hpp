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

#pragma once

#include <Kale/Math/Transform/Transform.hpp>

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
		 * @param camera The camera to render with
		 */
		virtual void render(const Camera& camera, float deltaTime) const = 0;

		/**
		 * Called on update, perfect place to do any physics updating, game logic, etc
		 * @param threadNum the index of the thread this update is called on
		 * @param scene The scene being updated to
		 * @param deltaTime The duration of the last frame in microseconds
		 */
		virtual void update(size_t threadNum, const Scene& scene, float deltaTime) = 0;

		/**
		 * Called prior to update, perfect place to do things such as updating the bounding box, etc
		 * @param threadNum the index of the thread this update is called on
		 * @param scene The scene being updated to
		 * @param deltaTime The duration of the last frame in microseconds
		 */
		virtual void preUpdate(size_t threadNum, const Scene& scene, float deltaTime) = 0;

		/**
		 * Creates the node parent
		 */
		Node();

		/**
		 * Creates the node parent
		 * @param updateTime The average update time, please see Node::updateTime for documentation
		 */
		Node(float updateTime);

		friend class Scene;
	
	public:

		/**
		 * The amount of time (in microseconds) this node takes to update & pre update on average
		 * 
		 * You can gain this statistic using the Kale Editor calibration tools.
		 * This measure is device specific, when developing on multiple devices it is recommended that you use
		 * a low powered device for calibration, the same device must be used for calibrating all nodes
		 * to avoid potential bias 
		 */
		const float updateTime = 100.0f;
	};
}
