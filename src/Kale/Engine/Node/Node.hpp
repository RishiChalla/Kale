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

#include <Kale/Math/Rect/Rect.hpp>

namespace Kale {

	/**
	 * Forward declaration of classes
	 */
	class Scene;
	
	/**
	 * The main class for nodes, all nodes must inherit from this class
	 */
	class Node {
	private:

	protected:

		/**
		 * The bounding box of the node.
		 */
		Rect boundingBox;

		/**
		 * Renders the node
		 * @param scene The scene being rendered on
		 */
		virtual void render(const Scene& scene) const = 0;

		/**
		 * Called prior to update, perfect place to do things such as updating the bounding box, etc
		 * @param deltaTime The amount of microseconds since the last update
		 * @param scene The scene being updated to
		 */
		virtual void preUpdate(float deltaTime, const Scene& scene) {}

		/**
		 * Updates the node
		 * @param deltaTime The amount of microseconds since the last update
		 * @param scene The scene being updated to
		 */
		virtual void update(float deltaTime, const Scene& scene) {}

		friend class Scene;

	public:

		/**
		 * The z position/depth of the node. Lower values are drawn first, higher values are placed above lower values.
		 */
		float zPosition = 0.0f;

		/**
		 * Gets the bounding box of the node
		 * The box returned will be used for collision detection with other nodes for functional gameplay,
		 * and for determining whether it needs to be rendered
		 * @returns The bounding box
		 */
		virtual Rect getBoundingBox() const {
			return boundingBox;
		}

	};
}
