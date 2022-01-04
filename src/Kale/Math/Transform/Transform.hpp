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

#pragma once

#include <Kale/Math/Matrix/Matrix.hpp>

namespace Kale {

	/**
	 * Represents a single transformation matrix.
	 */
	class Transform : public Matrix3f {
	public:

		/**
		 * Creates a transformation matrix with the identity matrix
		 */
		Transform();

		/**
		 * Creates a transformation matrix given an array
		 * @param arr The array
		 */
		Transform(const std::array<float, 9>& arr);

		/**
		 * Creates a transformation matrix given an array
		 * @param arr The array
		 */
		Transform(std::array<float, 9>&& arr);

		/**
		 * Creates a transformation matrix given the translation, rotation, and scale
		 * @param translation The translation of the matrix
		 * @param rotation The rotation of the matrix
		 * @param scaleFactor The scale factor of the matrix
		 * @param degrees Whether or not the rotation given is in degrees (false, default, means radians)
		 */
		Transform(const Vector2f& translation, float rotation, const Vector2f& scaleFactor, bool degrees);

		/**
		 * Creates a transformation matrix given the translation, rotation, and scale
		 * @param translateX The x translation of the matrix
		 * @param translateY The y translation of the matrix
		 * @param rotation The rotation of the matrix
		 * @param scaleX The x scale factor of the matrix
		 * @param scaleY The x scale factor of the matrix
		 * @param degrees Whether or not the rotation given is in degrees (false, default, means radians)
		 */
		Transform(float translateX, float translateY, float rotation, float scaleX, float scaleY, bool degrees);

		/**
		 * Scales the transformation matrix
		 * @param vec The vector to scale by
		 */
		void scale(const Vector2f& vec);

		/**
		 * Scales the transformation matrix
		 * @param x The x scale factor
		 * @param y The y scale factor
		 */
		void scale(float x, float y);

		/**
		 * Translates the transformation matrix
		 * @param vec The vector to translate by
		 */
		void translate(const Vector2f& vec);

		/**
		 * Translates the transformation matrix
		 * @param x The x displacement
		 * @param y The y displacement
		 */
		void translate(float x, float y);

		/**
		 * Rotates the transformation matrix using RADIANS
		 * @param angle The angle to rotate by in RADIANS
		 */
		void rotate(float angle);

		/**
		 * Rotates the transformation matrix using DEGREES
		 * @param angle The angle to rotate by in DEGREES
		 */
		void rotateDeg(float angle);

		/**
		 * Transforms a vector using this transformation matrix
		 * @param vec The vector to transform
		 * @returns The transformed vector
		 */
		Vector2f transform(const Vector2f& vec) const;

		/**
		 * Transforms a vector using this transformation matrix
		 * @param vec The vector to transform
		 */
		void transformInplace(Vector2f& vec) const;

		/**
		 * Inverse transforms a vector using this transformation matrix
		 * (Returns a vector transformed by this matrix to its original)
		 * @param vec The vector to transform
		 * @returns The transformed vector
		 */
		Vector2f inverseTransform(const Vector2f& vec) const;

		/**
		 * Transforms a vector using this transformation matrix
		 * (Returns a vector transformed by this matrix to its original)
		 * @param vec The vector to transform
		 */
		void inverseTransformInplace(Vector2f& vec) const;
	};

}
