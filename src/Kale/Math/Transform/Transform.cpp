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

#include "Transform.hpp"

#include <Kale/Math/Constants/Constants.hpp>
#include <Kale/Math/Utils/Utils.hpp>

#include <cmath>

using namespace Kale;

/**
 * Creates a transformation matrix with the identity matrix
 */
Transform::Transform() : Matrix3f({
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
}) {
	// Empty Body
}

/**
 * Creates a transformation matrix from a matrix
 * @param matrix The matrix
 */
Transform::Transform(const Matrix3f& matrix) : Matrix3f(matrix) {
	// Empty Body
}

/**
 * Creates a transformation matrix from a matrix
 * @param matrix The matrix
 */
Transform::Transform(Matrix3f&& matrix) : Matrix3f(matrix) {
	// Empty Body
}

/**
 * Creates a transformation matrix given an array
 * @param arr The array
 */
Transform::Transform(const std::array<float, 9>& arr) : Matrix3f(arr) {
	// Empty Body
}

/**
 * Creates a transformation matrix given an array
 * @param arr The array
 */
Transform::Transform(std::array<float, 9>&& arr) : Matrix3f(arr) {
	// Empty Body
}

/**
 * Creates a transformation matrix given the translation, rotation, and scale
 * @param translation The translation of the matrix
 * @param rotation The rotation of the matrix
 * @param scaleFactor The scale factor of the matrix
 * @param degrees Whether or not the rotation given is in degrees (false, default, means radians)
 */
Transform::Transform(const Vector2f& translation, float rotation, const Vector2f& scaleFactor, bool degrees) : Matrix3f({
	cos(degrees ? degToRad(rotation) : rotation) * scaleFactor.x, -sin(degrees ? degToRad(rotation) : rotation) * scaleFactor.y, translation.x,
	sin(degrees ? degToRad(rotation) : rotation) * scaleFactor.x, cos(degrees ? degToRad(rotation) : rotation) * scaleFactor.y, translation.y,
	0.0f, 0.0f, 1.0f
}) {
	// Empty Body
}

/**
 * Creates a transformation matrix given the translation, rotation, and scale
 * @param translateX The x translation of the matrix
 * @param translateY The y translation of the matrix
 * @param rotation The rotation of the matrix
 * @param scaleX The x scale factor of the matrix
 * @param scaleY The x scale factor of the matrix
 * @param degrees Whether or not the rotation given is in degrees (false, default, means radians)
 */
Transform::Transform(float translateX, float translateY, float rotation, float scaleX, float scaleY, bool degrees) : Matrix3f({
	cos(degrees ? degToRad(rotation) : rotation) * scaleX, -sin(degrees ? degToRad(rotation) : rotation) * scaleY, translateX,
	sin(degrees ? degToRad(rotation) : rotation) * scaleX, cos(degrees ? degToRad(rotation) : rotation) * scaleY, translateY,
	0.0f, 0.0f, 1.0f
}) {
	// Empty Body
}

/**
 * Sets this matrix to the identity matrix
 */
void Transform::setIdentity() {
	data = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};
}

/**
 * Scales the transformation matrix
 * @param vec The vector to scale by
 */
void Transform::scale(const Vector2f& vec) {
	data = std::move(Matrix3f::operator*(Matrix3f({
		vec.x, 0.0f, 0.0f,
		0.0f, vec.y, 0.0f,
		0.0f, 0.0f, 1.0f
	})).data);
}

/**
 * Scales the transformation matrix
 * @param x The x scale factor
 * @param y The y scale factor
 */
void Transform::scale(float x, float y) {
	data = std::move(Matrix3f::operator*(Matrix3f({
		x, 0.0f, 0.0f,
		0.0f, y, 0.0f,
		0.0f, 0.0f, 1.0f
	})).data);
}

/**
 * Scales the transformation matrix
 * @param factor The scale factor
 */
void Transform::scale(float factor) {
	data = std::move(Matrix3f::operator*(Matrix3f({
		factor, 0.0f, 0.0f,
		0.0f, factor, 0.0f,
		0.0f, 0.0f, 1.0f
	})).data);
}

/**
 * Translates the transformation matrix
 * @param vec The vector to translate by
 */
void Transform::translate(const Vector2f& vec) {
	data = std::move(Matrix3f::operator*(Matrix3f({
		1.0f, 0.0f, vec.x,
		0.0f, 1.0f, vec.y,
		0.0f, 0.0f, 1.0f
	})).data);
}

/**
 * Translates the transformation matrix
 * @param x The x displacement
 * @param y The y displacement
 */
void Transform::translate(float x, float y) {
	data = std::move(Matrix3f::operator*(Matrix3f({
		1.0f, 0.0f, x,
		0.0f, 1.0f, y,
		0.0f, 0.0f, 1.0f
	})).data);
}

/**
 * Rotates the transformation matrix using RADIANS
 * @param angle The angle to rotate by in RADIANS
 */
void Transform::rotate(float angle) {
	float c = cos(angle);
	float s = sin(angle);
	data = std::move(Matrix3f::operator*(Matrix3f({
		c, -s, 0.0f,
		s, c, 0.0f,
		0.0f, 0.0f, 1.0f
	})).data);
}

/**
 * Rotates the transformation matrix using DEGREES
 * @param angle The angle to rotate by in DEGREES
 */
void Transform::rotateDeg(float angle) {
	float rad = angle * PI / 180;
	float c = cos(rad);
	float s = sin(rad);
	data = std::move(Matrix3f::operator*(Matrix3f({
		c, -s, 0.0f,
		s, c, 0.0f,
		0.0f, 0.0f, 1.0f
	})).data);
}

/**
 * Sets the translation of the matrix
 * @param x The x component
 * @param y The y component
 */
void Transform::setTranslation(float x, float y) {
	data[2] = x;
	data[5] = y;
}

/**
 * Sets the translation of the matrix
 * @param vec The translation
 */
void Transform::setTranslation(const Vector2f& vec) {
	data[2] = vec.x;
	data[5] = vec.y;
}

/**
 * Gets the translation
 * @param The translation
 */
Vector2f Transform::getTranslation() const {
	return Vector2f(data[2], data[5]);
}

/**
 * Sets the rotation of the matrix
 * @param angle The angle of the rotation
 * @param degrees Whether the angle is in degrees or radians (true = degrees)
 */
void Transform::setRotation(float angle, bool degrees) {
	if (degrees) angle = degToRad(angle);
	Vector2f scale = getScale();
	float c = cos(angle);
	float s = sin(angle);
	data[0] = scale.x * c;
	data[1] = scale.y * -s;
	data[4] = scale.x * s;
	data[5] = scale.y * c;
}

/**
 * Gets the rotation in RADIANS
 * @returns the rotation in RADIANS
 */
float Transform::getRotation() const {
	return atan2(data[3], data[4]);
}

/**
 * Gets the rotation in DEGREES
 * @returns the rotation in DEGREES
 */
float Transform::getRotationDeg() const {
	return radToDeg(getRotation());
}

/**
 * Sets the scale of the matrix
 * @param scaleX The x component
 * @param scaleY The y component
 */
void Transform::setScale(float scaleX, float scaleY) {
	float c = cos(getRotation());
	data[0] = scaleX * c;
	data[4] = scaleY * c;
}

/**
 * Sets the scale of the matrix
 * @param scale The scale factor (applies for both horizontal/vertical scaling)
 */
void Transform::setScale(float scale) {
	float c = cos(getRotation());
	data[0] = scale * c;
	data[4] = scale * c;
}

/**
 * Gets the scale of the matrix
 * @returns the scale of the matrix
 */
Vector2f Transform::getScale() const {
	return Vector2f(
		sqrt(data[3] * data[3] + data[4] * data[4]),
		sqrt(data[0] * data[0] + data[1] * data[1])
	);
}

/**
 * Transforms a vector using this transformation matrix
 * @param vec The vector to transform
 * @returns The transformed vector
 */
Vector2f Transform::transform(const Vector2f& vec) const {
	return Vector2f(vec.x * data[0] + vec.y * data[1] + data[2], vec.x * data[3] + vec.y * data[4] + data[5]);
}

/**
 * Transforms a vector using this transformation matrix
 * @param vec The vector to transform
 */
void Transform::transformInplace(Vector2f& vec) const {
	vec.x = vec.x * data[0] + vec.y * data[1] + data[2];
	vec.y = vec.x * data[3] + vec.y * data[4] + data[5];
}

/**
 * Inverse transforms a vector using this transformation matrix
 * (Returns a vector transformed by this matrix to its original)
 * @param vec The vector to transform
 * @returns The transformed vector
 */
Vector2f Transform::inverseTransform(const Vector2f& vec) const {
	float den = data[0] * data[4] - data[1] * data[3];
	return Vector2f(
		(data[1] * data[5] - data[1] * vec.y - data[2] * data[4] + data[4] * vec.x) / den,
		(-data[0] * data[5] + data[0] * vec.y + data[2] * data[3] - data[3] * vec.x) / den
	);
	// Below system of equations represents transformation where k, j = vec.x, vec.y
	// x * n0 + y * n1 + n2 = k
	// x * n3 + y * n4 + n5 = j
	// Solve the above for x & y ->
	// x = (n1*n5 - n1*j - n2*n4 + n4*k) / (n0*n4 - n1*n3)
	// y = (-n0*n5 + n0*j + n2*n3 - n3*k) / (n0*n4 - n1*n3)
}

/**
 * Transforms a vector using this transformation matrix
 * (Returns a vector transformed by this matrix to its original)
 * @param vec The vector to transform
 */
void Transform::inverseTransformInplace(Vector2f& vec) const {
	float den = data[0] * data[4] - data[1] * data[3];
	vec.x = (data[1] * data[5] - data[1] * vec.y - data[2] * data[4] + data[4] * vec.x) / den;
	vec.y = (-data[0] * data[5] + data[0] * vec.y + data[2] * data[3] - data[3] * vec.x) / den;
}
