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

#include <mutex>

namespace Kale {

	/**
	 * All transformable nodes must inherit from this class. Inheritance from this class is required for synchronized access to
	 * transformation matrices across threads & for usage from the animation system.
	 */
	class Transformable {
	protected:

		/**
		 * The transformation matrix of this object
		 */
		Transform transform;

		/**
		 * The mutex used for synchronized write access to the transform
		 */
		std::mutex mutex;
	
	public:

		/**
		 * Thread safe method to get the transformation matrix
		 * @returns The transformation matrix of this object
		 */
		Transform getTransform() const;

		/**
		 * Thread safe method to set the transformation matrix
		 * @param transform The transformation matrix to set the transform of this object to
		 */
		void setTransform(const Transform& transform);

	};
}
