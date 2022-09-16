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

#include <utility>
#include <vector>

namespace Kale {

	/**
	 * Triangulates the given path and returns both the vertices and indices
	 * @param begin The beginning of the path to triangulate
	 * @param end The ending of the path to triangulate
	 * @returns A tuple of vectors for the vertices and indices
	 */
	std::pair<std::vector<Vector2f>, std::vector<unsigned int>> triangulatePath(const Vector2f* begin, const Vector2f* end);

	/**
	 * Triangulates the given path and returns both the vertices as floats and indices
	 * @param begin The beginning of the path to triangulate
	 * @param end The ending of the path to triangulate
	 * @returns A tuple of vectors for the vertices and indices
	 */
	std::pair<std::vector<float>, std::vector<unsigned int>> triangulatePathFloat(const Vector2f* begin, const Vector2f* end);
}
