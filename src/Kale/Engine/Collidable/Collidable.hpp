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

#include <mutex>

namespace Kale {
	
	/**
	 * All nodes capable of collision must inherit from this class
	 */
	class Collidable {
	protected:

		/**
		 * The bounding box of the node, bounding box should be updated in preUpdate where possible allowing other
		 * nodes to use it during update. Thread syncrhonization to bounding box updating is expected by the inheriting node.
		 */
		Rect boundingBox;

	public:

		/**
		 * Thread safe method to get the bounding box
		 * @returns The bounding box of this node
		 */
		Rect getBoundingBox() const;
		
	};
}
