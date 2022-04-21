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
#include <Kale/Engine/Node/Node.hpp>
#include <Kale/Engine/AnimatableNode/AnimatableNode.hpp>
#include <Kale/Core/Application/Application.hpp>

#include <include/core/SkPath.h>
#include <effects/SkRuntimeEffect.h>

#include <stdexcept>
#include <limits>
#include <sstream>
#include <fstream>

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
		 * Converts the path to a skia path
		 * @param camera The camera to transform with
		 * @returns The skia path
		 */
		SkPath toSkia(const Camera& camera) const;
	};

	/**
	 * The shader to use for drawing path nodes
	 */
	inline sk_sp<SkRuntimeEffect> pathNodeShader = nullptr;

	/**
	 * Perform any initial setup before any scene loads or renders
	 */
	void pathNodeShaderSetup();

	/**
	 * Represents a single path of cubic bezier curves
	 * @tparam A enum class of animation states
	 */
	template <typename T>
	class PathNode : public AnimatableNode<T, Path> {
	protected:

		/**
		 * Gets the current path based on the state composition for this frame
		 * @returns The path
		 */
		Path getCurrentPath() const {
			std::vector<std::pair<T, float>> composition = AnimatableNode<T, Path>::getStateComposition();
			Path path = Path(AnimatableNode<T, Path>::structures.at(composition[0].first).beziers.size());
			for (const std::pair<T, float>& comp : composition) {
				path += AnimatableNode<T, Path>::structures.at(comp.first) * comp.second;
			}
			return path;
		}

		/**
		 * Renders the node
		 * @param camera The camera to render with
		 */
		void render(const Camera& camera) const override {
			SkCanvas& canvas = mainApp->getWindow().getCanvas();
			SkPath path = getCurrentPath().toSkia(camera);

			SkRuntimeShaderBuilder builder(pathNodeShader);
			builder.uniform("color") = SkV4{color.x, color.y, color.z, color.w};
			SkPaint paint;
			paint.setShader(builder.makeShader());

			canvas.drawPath(path, paint);
		}

		/**
		 * Updates the node
		 * @param deltaTime The amount of microseconds since the last update
		 * @param lights The lights to update
		 */
		void update(float deltaTime, const std::unordered_set<std::shared_ptr<Light>>& lights) override {
			AnimatableNode<T, Path>::update(deltaTime, lights);
		}

	public:

		/**
		 * The transform of the path, all beziers are relative to this point
		 */
		Transform transform;

		/**
		 * The color of the node
		 */
		Color color;

		/**
		 * Gets a bounding box for this geometry to check for quick and inaccurate collisions
		 * @return The bounding box
		 */
		Rect getBoundingBox() const override {
			Vector2f topLeft(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
			Vector2f bottomRight(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
			
			for (const CubicBezier& bezier : getCurrentPath().beziers) {
				std::array<Vector2f, 4> points = {bezier.start, bezier.controlPoint1, bezier.controlPoint2, bezier.end};
				for (const Vector2f& point : points) {
					if (point.x < topLeft.x) topLeft.x = point.x;
					if (point.y < topLeft.y) topLeft.x = point.y;
					if (point.x > bottomRight.x) bottomRight.x = point.x;
					if (point.y > bottomRight.y) bottomRight.y = point.y;
				}
			}

			return {topLeft, bottomRight};
		}

	};
}
