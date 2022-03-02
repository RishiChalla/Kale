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
#include <Kale/Math/Path/Path.hpp>
#include <Kale/Engine/Light/Light.hpp>

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
		 * The z position/depth of the node. Lower values are drawn first, higher values are placed above lower values.
		 */
		float zPosition = 0.0f;

		/**
		 * Renders the node, this method is guaranteed to be called from the main thread
		 * @param camera The camera to render with
		 * @param lights The lights currently affecting the scene
		 */
		virtual void render(const Camera& camera, const std::list<std::shared_ptr<Light>>& lights) const = 0;

		/**
		 * Updates the node, may be called from any thread. Is guaranteed to NOT call on the main thread.
		 * @param ups The amount of updates the current thread is doing in a second
		 */
		virtual void update(float ups) = 0;

		/**
		 * Gets the bounding box of the node
		 * The box returned will be used for collision detection with other nodes for functional gameplay, and for determining whether it needs to be rendered
		 * This method may be called from any thread
		 * @returns The bounding box
		 */
		virtual const RotatedRect& getBoundingBox() const = 0;

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
		friend class Light;

	public:

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
