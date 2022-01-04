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

#include "Utils.hpp"

#include <iterator>

#include <delaunator.hpp>

using namespace Kale;

/**
 * Triangulates the given path and returns both the vertices and indices
 * @param path The path to triangulate
 * @returns A tuple of vectors for the vertices and indices
 */
std::tuple<std::vector<Vector2f>, std::vector<unsigned int>> Kale::triangulatePath(const std::vector<Vector2f>& path) {
	std::vector<double> coords;
	coords.reserve(path.size() * 2);
	for (const Vector2f& vec : path) {
		coords.push_back(static_cast<double>(vec.x));
		coords.push_back(static_cast<double>(vec.y));
	}

	delaunator::Delaunator d(coords);

	std::tuple<std::vector<Vector2f>, std::vector<unsigned int>> output;
	std::vector<Vector2f>& vertices = std::get<0>(output);
	std::vector<unsigned int>& elements = std::get<1>(output);

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
 * @param path The path to triangulate
 * @returns A tuple of vectors for the vertices and indices
 */
std::tuple<std::vector<float>, std::vector<unsigned int>> Kale::triangulatePathFloat(const std::vector<Vector2f>& path) {
	std::vector<double> coords;
	coords.reserve(path.size() * 2);
	for (const Vector2f& vec : path) {
		coords.push_back(static_cast<double>(vec.x));
		coords.push_back(static_cast<double>(vec.y));
	}

	delaunator::Delaunator d(coords);

	std::tuple<std::vector<float>, std::vector<unsigned int>> output;
	std::vector<float>& vertices = std::get<0>(output);
	std::vector<unsigned int>& elements = std::get<1>(output);

	elements.reserve(d.triangles.size());
	std::transform(d.triangles.begin(), d.triangles.end(), std::back_inserter(elements), [](size_t index) {
		return static_cast<unsigned int>(index);
	});

	vertices.reserve(d.coords.size() / 2);
	std::transform(d.coords.begin(), d.coords.end(), std::back_inserter(vertices), [](double value) {
		return static_cast<float>(value);
	});

	return output;
}
