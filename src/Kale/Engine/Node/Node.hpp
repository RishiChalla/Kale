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
#include <Kale/Math/Geometry/Geometry.hpp>
#include <Kale/Math/Rect/Rect.hpp>

#include <unordered_set>

namespace Kale {

	/**
	 * Forward declaration of classes
	 */
	class Scene; class Light;
	
	/**
	 * The main class for nodes, all nodes must inherit from this class
	 */
	class Node {
	private:

	protected:

		/**
		 * The z position/depth of the node. Lower values are drawn first, higher values are placed above lower values.
		 */
		float zPosition = 0.0f;

		/**
		 * Renders the node
		 * @param camera The camera to render with
		 */
		virtual void render(const Camera& camera) const = 0;

		/**
		 * Updates the node
		 * @param deltaTime The amount of microseconds since the last update
		 * @param lights The lights to update
		 */
		virtual void update(float deltaTime, const std::unordered_set<std::shared_ptr<Light>>& lights) = 0;

		/**
		 * Gets the bounding box of the node
		 * The box returned will be used for collision detection with other nodes for functional gameplay,
		 * and for determining whether it needs to be rendered
		 * @returns The bounding box
		 */
		virtual Rect getBoundingBox() const = 0;

		friend class Scene;
	};
}
