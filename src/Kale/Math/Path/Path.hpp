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
#include <ostream>
#include <utility>

#include <nlohmann/json.hpp>

namespace Kale {

	/**
	 * Javascript Standard Object Notation allows for saving and using permanent configuration files, via the nlohmann/json C++
	 * library.
	 */
	using JSON = nlohmann::json;

	/**
	 * Represents a single cubic bezier
	 */
	struct CubicBezier {
		Vector2f start, controlPoint1, controlPoint2, end;
	};

	/**
	 * Represents a path of beziers
	 */
	class Path {
	private:

		/**
		 * Creates a path with rounded corners
		 * @param lines The lines to create the path from
		 */
		void roundCorners(const std::vector<std::pair<Vector2f, float>>& lines);

	public:

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
		 * Creates a path given beziers
		 * @param beziers The beziers
		 */
		Path(const std::vector<CubicBezier>& beziers);
		
		/**
		 * Creates a path given a closed shape made of lines and a corner radius to use to round each corner of the shape
		 * @param lines A vector of the lines, each element has a line drawn to the next and the last element has a line drawn to the first
		 * @param cornerRadius The corner radius to use to round the corners of the shape
		 */
		Path(const std::vector<Vector2f>& lines, float cornerRadius);
		
		/**
		 * Creates a path given a closed shape of lines, and a corner radius to use to round each corner of the shape
		 * @param lines A vector of the lines, each element has a line drawn to the next and the last element has a line drawn to the first.
		 * The second item in the pair is the corner radius to the corner created by the specific point, you can use a negative number or 0 to
		 * denote no corner rounding.
		 */
		Path(const std::vector<std::pair<Vector2f, float>>& lines);

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

		/**
		 * Moves the starting point of the path to a certain point.
		 * @note This function must be used when there are no beziers in the path,
		 * and other path modification methods must be used directly after.
		 * @param pos The position to move to
		 */
		void moveTo(Vector2f pos);

		/**
		 * Moves the path to a new position while drawing a line to the new position.
		 * @note Do not add beziers manually when using path modification methods
		 * @param pos The position to draw a line to
		 */
		void lineTo(Vector2f pos);

		/**
		 * Moves the path to a new position while drawing a cubic bezier curve to the new position.
		 * @note Do not add beziers manually when using path modification methods
		 * @param controlPoint1 The first control point of the bezier
		 * @param controlPoint2 The second control point of the bezier
		 * @param end The ending point of the bezier
		 */
		void bezierTo(Vector2f controlPoint1, Vector2f controlPoint2, Vector2f end);

		/**
		 * Creates an arc (a segment of a circle's circumference) to a point given a center
		 * @note Do not add beziers manually when using path modification methods
		 * @param end The ending point where the arc ends
		 * @param center The center of the circle of which the arc is drawn to
		 * @param clockwise Whether or not to draw the arc clockwise or counter clockwise to the ending position
		 */
		void arcTo(Vector2f end, Vector2f center, bool clockwise);

		/**
		 * Closes the path allowing for rendering
		 * @note Do not add beziers manually when using path modification methods, this method closes off the path making it suitable for rendering
		 */
		void closePath();

	};

	/**
	 * Prints a bezier to an output stream
	 */
	std::ostream& operator<<(std::ostream& os, const CubicBezier& bezier);

	/**
	 * Prints a path to an output stream
	 */
	std::ostream& operator<<(std::ostream& os, const Path& path);

	/**
	 * Creates a bezier from a json
	 */
	inline void from_json(const JSON& j, CubicBezier& p) {
		p.start = j["P0"].get<Vector2f>();
		p.controlPoint1 = j["P1"].get<Vector2f>();
		p.controlPoint2 = j["P2"].get<Vector2f>();
		p.end = j["P3"].get<Vector2f>();
	}

	/**
	 * Populates a json from a bezier
	 */
	inline void to_json(JSON& j, const CubicBezier& p) {
		j = {{"P0", p.start}, {"P1", p.controlPoint1}, {"P2", p.controlPoint2}, {"P3", p.end}};
	}

	/**
	 * Creates a path from a json
	 */
	inline void from_json(const JSON& j, Path& p) {
		p.beziers = j.get<std::vector<CubicBezier>>();
	}

	/**
	 * Populates a json from a path
	 */
	inline void to_json(JSON& j, const Path& p) {
		j = JSON(p.beziers);
	}

}
