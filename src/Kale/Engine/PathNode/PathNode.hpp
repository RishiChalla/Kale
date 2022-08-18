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
#include <Kale/Math/Path/Path.hpp>
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
			// Get canvas, convert path, & draw main path
			SkCanvas& canvas = mainApp->getWindow().getCanvas();
			SkPath path = currentPath.toSkia(transform);
			SkPaint paint(color);
			paint.setAntiAlias(true);
			canvas.drawPath(path, paint);

			// End if no border required
			if (!hasBorder && !lightShading) return;

			// Setup border paint
			SkPaint borderPaint(borderColor);
			borderPaint.setAntiAlias(true);
			borderPaint.setStyle(SkPaint::Style::kStroke_Style);
			borderPaint.setStrokeCap(SkPaint::Cap::kRound_Cap);
			borderPaint.setStrokeJoin(SkPaint::Join::kRound_Join);
			borderPaint.setStrokeWidth(borderSize * 2.0f);

			// Draw border if applicable
			if (hasBorder) {
				canvas.save();
				canvas.clipPath(path);
				canvas.drawPath(path, borderPaint);
				canvas.restore();
			}

			// Draw light shading if applicable
			if (lightShading) {
				for (const std::shared_ptr<Light>& light : scene.getLights()) {
					if (!light->applyShading) continue;
					if (!light->getBoundingBox().rectCollision(Node::boundingBox)) continue;
					canvas.save();
					canvas.clipPath(light->getShadingMask());
					borderPaint.setColor(borderColor * 0.8f + light->color * 0.2f);
					canvas.drawPath(path, borderPaint);
					canvas.restore();
				}
			}
		}

		/**
		 * Called prior to update, perfect place to do things such as updating the bounding box, etc
		 * @param deltaTime The amount of microseconds since the last update
		 * @param scene The scene being updated to
		 */
		void preUpdate(float deltaTime, const Scene& scene) override {

			if (!transitioning) return;

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
		float borderSize = 10.0f;

		/**
		 * Whether to apply light shading on this path's border, can be applied even if border is off
		 */
		bool lightShading = true;

	};
}
