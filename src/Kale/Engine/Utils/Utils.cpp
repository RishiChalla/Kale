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

#include "Utils.hpp"

#include <delaunator.hpp>

#include <iterator>
#include <algorithm>

using namespace Kale;

/**
 * Triangulates the given path and returns both the vertices and indices
 * @param begin The beginning of the path to triangulate
 * @param end The ending of the path to triangulate
 * @returns A tuple of vectors for the vertices and indices
 */
std::pair<std::vector<Vector2f>, std::vector<unsigned int>> Kale::triangulatePath(const Vector2f* begin, const Vector2f* end) {
	std::vector<double> coords;
	coords.reserve(std::distance(begin, end) * 2);
	
	std::for_each(begin, end, [&](const Vector2f& vec) {
		coords.push_back(static_cast<double>(vec.x));
		coords.push_back(static_cast<double>(vec.y));
	});
	for (const Vector2f* it = begin; it != end; ++it) {
	}

	delaunator::Delaunator d(coords);

	std::pair<std::vector<Vector2f>, std::vector<unsigned int>> output;
	std::vector<Vector2f>& vertices = output.first;
	std::vector<unsigned int>& elements = output.second;

	elements.reserve(d.triangles.size());
	std::transform(d.triangles.begin(), d.triangles.end(), std::back_inserter(elements), [](size_t index) {
		return static_cast<unsigned int>(index);
	});

	vertices.reserve(d.coords.size() / 2);
	for (size_t i = 0; i < d.coords.size(); i += 2)
		vertices.emplace_back(static_cast<float>(d.coords[i]), static_cast<float>(d.coords[i+1]));

	return output;
}

/**
 * Triangulates the given path and returns both the vertices as floats and indices
 * @param begin The beginning of the path to triangulate
 * @param end The ending of the path to triangulate
 * @returns A tuple of vectors for the vertices and indices
 */
std::pair<std::vector<float>, std::vector<unsigned int>> Kale::triangulatePathFloat(const Vector2f* begin, const Vector2f* end) {
	std::vector<double> coords;
	coords.reserve(std::distance(begin, end) * 2);
	
	for (const Vector2f* it = begin; it != end; ++it) {
		const Vector2f& vec = *it;
		coords.push_back(static_cast<double>(vec.x));
		coords.push_back(static_cast<double>(vec.y));
	}

	delaunator::Delaunator d(coords);

	std::pair<std::vector<float>, std::vector<unsigned int>> output;
	std::vector<float>& vertices = output.first;
	std::vector<unsigned int>& elements = output.second;

	elements.reserve(d.triangles.size());
	std::transform(d.triangles.begin(), d.triangles.end(), std::back_inserter(elements), [](size_t index) {
		return static_cast<unsigned int>(index);
	});

	vertices.reserve(d.coords.size());
	std::transform(d.coords.begin(), d.coords.end(), std::back_inserter(vertices), [](double value) {
		return static_cast<float>(value);
	});

	return output;
}
