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

#include "SpotLight.hpp"

#include <Kale/Core/Application/Application.hpp>
#include <Kale/Math/DrawingContext/DrawingContext.hpp>

#include <include/core/SkCanvas.h>
#include <include/core/SkPath.h>
#include <include/core/SkPaint.h>
#include <include/core/SkMaskFilter.h>

using namespace Kale;

/**
 * Creates an empty spot light object
 */
SpotLight::SpotLight() {
	// Empty Body
}

/**
 * Creates a white spot light
 * @param position The position of the spot light
 * @param radius The radius of the spot light
 * @param intensity The intensity of the spot light
 */
SpotLight::SpotLight(Vector2f position, float radius, float intensity) : transform(position, 0.0f, Vector2f::one(), AngleUnit::Radian),
	radius(radius), intensity(intensity) {
	// Empty Body
}

/**
 * Creates a white spot light
 * @param position The position of the spot light
 * @param radius The radius of the spot light
 * @param intensity The intensity of the spot light
 * @param start The start ray unit vector of the light
 * @param end The end ray unit vector of the light
 */
SpotLight::SpotLight(Vector2f position, float radius, float intensity, Vector2f start, Vector2f end) : radius(radius), intensity(intensity),
	start(start), end(end), transform(position, 0.0f, Vector2f::one(), AngleUnit::Radian) {
	// Empty Body
}

/**
 * Creates a white spot light
 * @param position The position of the spot light
 * @param radius The radius of the spot light
 * @param intensity The intensity of the spot light
 * @param start The starting angle of the light
 * @param end The ending angle of the light
 * @param unit The unit of the start and end angles
 */
SpotLight::SpotLight(Vector2f position, float radius, float intensity, float start, float end, AngleUnit unit) : radius(radius), intensity(intensity),
	transform(position, 0.0f, Vector2f::one(), AngleUnit::Radian) {
	
	if (unit == AngleUnit::Degree) {
		start = degToRad(start);
		end = degToRad(end);
	}

	this->start = {cos(start), sin(start)};
	this->end = {cos(end), sin(end)};
}

/**
 * Creates a spot light
 * @param position The position of the spot light
 * @param radius The radius of the spot light
 * @param intensity The intensity of the spot light
 * @param start The start ray unit vector of the light
 * @param end The end ray unit vector of the light
 * @param color The color of the light
 */
SpotLight::SpotLight(Vector2f position, float radius, float intensity, Vector2f start, Vector2f end, Color color) : radius(radius), intensity(intensity),
	start(start), end(end), transform(position, 0.0f, Vector2f::one(), AngleUnit::Radian) {
	
	Light::color = color;
}

/**
 * Creates a spot light
 * @param position The position of the spot light
 * @param radius The radius of the spot light
 * @param intensity The intensity of the spot light
 * @param start The starting angle of the light
 * @param end The ending angle of the light
 * @param unit The unit of the start and end angles
 * @param color The color of the light
 */
SpotLight::SpotLight(Vector2f position, float radius, float intensity, float start, float end, AngleUnit unit, Color color) : radius(radius), intensity(intensity),
	transform(position, 0.0f, Vector2f::one(), AngleUnit::Radian) {
	
	Light::color = color;

	if (unit == AngleUnit::Degree) {
		start = degToRad(start);
		end = degToRad(end);
	}

	this->start = {cos(start), sin(start)};
	this->end = {cos(end), sin(end)};
}

/**
 * Gets the clipping mask to use when shading
 * @returns The clipping mask
 */
SkPath SpotLight::getShadingMask() const {
	SkPath mask;
	mask.moveTo(transform.getTranslation());
	mask.lineTo(transform.transform(start * intensity));
	mask.lineTo(transform.transform(end * intensity));
	mask.close();
	return mask;
}

/**
 * Renders the node
 * @param scene The scene being rendered on
 */
void SpotLight::render(const Scene& scene) const {
	SkCanvas& canvas = mainApp->getWindow().getCanvas();
	SkPaint paint(Light::color);
	paint.setMaskFilter(SkMaskFilter::MakeBlur(SkBlurStyle::kNormal_SkBlurStyle, intensity));
	
	SkPath path;
	path.moveTo(0.0f, 0.0f);;
	path.lineTo(start * radius);
	path.lineTo(end * radius);
	path.close();
	
	DrawingContext ctx(transform);
	canvas.drawPath(path, paint);
}
