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
	 * Represents a single path of cubic bezier curves
	 * @tparam A enum class of animation states
	 */
	template <typename T>
	class PathNode : public AnimatableNode<T, Path> {
	protected:

		/**
		 * The current path of this node
		 */
		Path currentPath;

		/**
		 * Renders the node
		 * @param scene The scene being rendered on
		 */
		void render(const Scene& scene) const override {
			SkCanvas& canvas = mainApp->getWindow().getCanvas();
			SkPath path = currentPath.toSkia(transform);
			SkPaint paint(color);
			paint.setAntiAlias(true);
			canvas.drawPath(path, paint);
			if (!hasBorder) return;

			SkPaint borderPaint(borderColor);
			borderPaint.setAntiAlias(true);
			borderPaint.setStyle(SkPaint::Style::kStroke_Style);
			borderPaint.setStrokeCap(SkPaint::Cap::kRound_Cap);
			borderPaint.setStrokeJoin(SkPaint::Join::kRound_Join);
			borderPaint.setStrokeWidth(borderSize);
			canvas.drawPath(path, borderPaint);
		}

		/**
		 * Called prior to update, perfect place to do things such as updating the bounding box, etc
		 * @param deltaTime The amount of microseconds since the last update
		 * @param scene The scene being updated to
		 */
		void preUpdate(float deltaTime, const Scene& scene) override {

			// Update Animatable Node
			AnimatableNode<T, Path>::preUpdate(deltaTime, scene);

			// Update Current Path
			std::vector<std::pair<T, float>> composition = AnimatableNode<T, Path>::getStateComposition();
			currentPath = Path(AnimatableNode<T, Path>::structures.at(composition[0].first).beziers.size());
			for (const std::pair<T, float>& comp : composition) {
				currentPath += AnimatableNode<T, Path>::structures.at(comp.first) * comp.second;
			}

			// Update bounding box
			Vector2f topLeft(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
			Vector2f bottomRight(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
			
			for (const CubicBezier& bezier : currentPath.beziers) {
				for (const Vector2f& point : {bezier.start, bezier.controlPoint1, bezier.controlPoint2, bezier.end}) {
					if (point.x < topLeft.x) topLeft.x = point.x;
					if (point.y < topLeft.y) topLeft.y = point.y;
					if (point.x > bottomRight.x) bottomRight.x = point.x;
					if (point.y > bottomRight.y) bottomRight.y = point.y;
				}
			}

			Node::boundingBox = {topLeft, bottomRight};
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
		 * Whether or not to draw a border around this path
		 */
		bool hasBorder = false;

		/**
		 * The color of this path's border
		 */
		Color borderColor;

		/**
		 * The size of this path's border
		 */
		float borderSize = 2.0f;

		/**
		 * Whether to apply light shading on this path's border **only applies if hasBorder is true**
		 */
		bool lightShading = true;

	};
}
