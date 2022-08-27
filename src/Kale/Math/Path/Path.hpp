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

#include <Kale/Math/Vector/Vector.hpp>
#include <Kale/Math/Rect/Rect.hpp>
#include <Kale/Math/Transform/Transform.hpp>

#include <vector>

namespace Kale {

	/**
	 * Represents a single cubic bezier
	 */
	struct CubicBezier {
		Vector2f start, controlPoint1, controlPoint2, end;
	};

	/**
	 * Represents a path of beziers
	 */
	struct Path {

		/**
		 * The beziers held in this path
		 */
		std::vector<CubicBezier> beziers;

		/**
		 * Creates a new empty path
		 */
		Path();

		/**
		 * Creates a path with a size with all points at 0
		 * @param n The size
		 */
		Path(size_t n);

		/**
		 * Gets the bounding box for this path
		 * @returns The bounding box
		 */
		Rect getBoundingBox() const;

		/**
		 * Adds another path to this
		 * @param other The path to add to this
		 */
		void operator+=(const Path& other);

		/**
		 * Multiplies this path's points by a value
		 * @param value The scalar value
		 */
		Path operator*(float value) const;

	};
}
