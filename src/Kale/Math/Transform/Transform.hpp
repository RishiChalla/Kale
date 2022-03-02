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

#include <Kale/Math/Matrix/Matrix.hpp>
#include <Kale/Math/RotatedRect/RotatedRect.hpp>
#include <Kale/Math/Rect/Rect.hpp>
#include <Kale/Math/Ray/Ray.hpp>
#include <Kale/Math/Line/Line.hpp>
#include <Kale/Math/Path/Path.hpp>
#include <Kale/Math/Circle/Circle.hpp>

#include <type_traits>

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
		 * Creates a transformation matrix from a matrix
		 * @param matrix The matrix
		 */
		Transform(const Matrix3f& matrix);

		/**
		 * Creates a transformation matrix from a matrix
		 * @param matrix The matrix
		 */
		Transform(Matrix3f&& matrix);

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
		 * Sets this matrix to the identity matrix
		 */
		void setIdentity();

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
		 * Scales the transformation matrix
		 * @param factor The scale factor
		 */
		void scale(float factor);

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
		 * Sets the translation of the matrix
		 * @param x The x component
		 * @param y The y component
		 */
		void setTranslation(float x, float y);

		/**
		 * Sets the translation of the matrix
		 * @param vec The translation
		 */
		void setTranslation(const Vector2f& vec);

		/**
		 * Gets the translation
		 * @param The translation
		 */
		Vector2f getTranslation() const;

		/**
		 * Sets the rotation of the matrix
		 * @param angle The angle of the rotation
		 * @param degrees Whether the angle is in degrees or radians (true = degrees)
		 */
		void setRotation(float angle, bool degrees);

		/**
		 * Gets the rotation in RADIANS
		 * @returns the rotation in RADIANS
		 */
		float getRotation() const;
		
		/**
		 * Gets the rotation in DEGREES
		 * @returns the rotation in DEGREES
		 */
		float getRotationDeg() const;

		/**
		 * Sets the scale of the matrix
		 * @param scaleX The x component
		 * @param scaleY The y component
		 */
		void setScale(float scaleX, float scaleY);

		/**
		 * Sets the scale of the matrix
		 * @param scale The scale factor (applies for both horizontal/vertical scaling)
		 */
		void setScale(float scale);

		/**
		 * Gets the scale of the matrix
		 * @returns the scale of the matrix
		 */
		Vector2f getScale() const;

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

		/**
		 * Transforms a rect using this transformation matrix
		 * @param rect The rect to transform
		 * @returns The transformed rect as a path
		 */
		Path transform(const RotatedRect& rect) const;
		
		/**
		 * Inverse transforms a rect using this transformation matrix
		 * (Returns a rect transformed by this matrix to its original)
		 * @param rect The rect to transform
		 * @returns The transformed rect as a path
		 */
		Path inverseTransform(const RotatedRect& rect) const;

		/**
		 * Transforms a rect using this transformation matrix
		 * @param rect The rect to transform
		 * @returns The transformed rect as a path
		 */
		Path transform(const Rect& rect) const;

		/**
		 * Inverse transforms a rect using this transformation matrix
		 * (Returns a rect transformed by this matrix to its original)
		 * @param rect The rect to transform
		 * @returns The transformed rect as a path
		 */
		Path inverseTransform(const Rect& rect) const;
		
		/**
		 * Transforms a ray using this transformation matrix
		 * @param ray The ray to transform
		 * @returns The transformed ray
		 */
		Ray transform(const Ray& ray) const;
		
		/**
		 * Inverse transforms a ray using this transformation matrix
		 * (Returns a ray transformed by this matrix to its original)
		 * @param ray The ray to transform
		 * @returns The transformed ray
		 */
		Ray inverseTransform(const Ray& ray) const;
		
		/**
		 * Transforms a line using this transformation matrix
		 * @param line The line to transform
		 * @returns The transformed line
		 */
		Line transform(const Line& line) const;
		
		/**
		 * Inverse transforms a line using this transformation matrix
		 * (Returns a line transformed by this matrix to its original)
		 * @param line The line to transform
		 * @returns The transformed line
		 */
		Line inverseTransform(const Line& line) const;
		
		/**
		 * Transforms a path using this transformation matrix
		 * @param path The path to transform
		 * @returns The transformed path
		 */
		Path transform(const Path& path) const;
		
		/**
		 * Transforms a path using this transformation matrix
		 * @param path The path to transform
		 * @returns The transformed path
		 */
		Path transform(Path&& path) const;
		
		/**
		 * Inverse transforms a path using this transformation matrix
		 * (Returns a path transformed by this matrix to its original)
		 * @param path The path to transform
		 * @returns The transformed path
		 */
		Path inverseTransform(const Path& path) const;
		
		/**
		 * Inverse transforms a path using this transformation matrix
		 * (Returns a path transformed by this matrix to its original)
		 * @param path The path to transform
		 * @returns The transformed path
		 */
		Path inverseTransform(Path&& path) const;
		
		/**
		 * Transforms a circle using this transformation matrix
		 * @param circle The circle to transform
		 * @returns The transformed circle
		 */
		Circle transform(const Circle& circle) const;
		
		/**
		 * Inverse transforms a circle using this transformation matrix
		 * (Returns a circle transformed by this matrix to its original)
		 * @param circle The circle to transform
		 * @returns The transformed circle
		 */
		Circle inverseTransform(const Circle& circle) const;

		/**
		 * Checks whether or not a geometry is within the view of this camera/transform
		 * @param geometry The geometry to check for
		 * @param viewport The width/height of the unscaled camera
		 * @returns Whether or not the geometry is within the view
		 */
		template <typename T> typename std::enable_if<std::is_base_of<Geometry, T>::value, bool>::type
		isInView(const T& geometry, const Vector2f& viewport) {
			return geometry.pathCollision(transform(Rect({0, 0}, viewport)));
		}

	};

	/**
	 * Represents a camera for viewing scenes (This is just a typedef of Transform)
	 */
	typedef Transform Camera;

}
