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

#include "PointLight.hpp"

#include <Kale/Core/Application/Application.hpp>

#include <include/core/SkCanvas.h>
#include <include/core/SkPath.h>
#include <include/core/SkPaint.h>
#include <include/core/SkMaskFilter.h>

using namespace Kale;

/**
 * Renders the node
 * @param scene The scene being rendered on
 */
void PointLight::render(const Scene& scene) const {
	if (!bloom) return;

	SkCanvas& canvas = mainApp->getWindow().getCanvas();
	SkPaint paint(Light::color);
	paint.setBlendMode(SkBlendMode::kMultiply);
	paint.setMaskFilter(SkMaskFilter::MakeBlur(SkBlurStyle::kNormal_SkBlurStyle, intensity));
	canvas.drawCircle(position, radius, paint);
}

/**
 * Gets the clipping mask to use when shading
 * @returns The clipping mask
 */
SkPath PointLight::getShadingMask() const {
	SkPath path;
	path.addCircle(position.x, position.y, intensity);
	return path;
}

/**
 * Creates an empty point light object
 */
PointLight::PointLight() {
	// Empty Body
}

/**
 * Creates a white point light at a position given the radius
 * @param position The position of the light
 * @param radius The radius of the light
 * @param intensity The intensity of the light
 */
PointLight::PointLight(Vector2f position, float radius, float intensity) : position(position), radius(radius), intensity(intensity) {
	// Empty Body
}

/**
 * Creates a point light
 * @param position The position of the light
 * @param radius The radius of the light
 * @param intensity The intensity of the light
 * @param color The color of the light
 */
PointLight::PointLight(Vector2f position, float radius, float intensity, Color color) : position(position), radius(radius), intensity(intensity) {
	Light::color = color;
}
