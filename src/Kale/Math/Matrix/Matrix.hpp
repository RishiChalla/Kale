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

#include <array>
#include <algorithm>
#include <ostream>
#include <iomanip>
#include <type_traits>
#include <limits>

#ifdef KALE_DEBUG
#include <stdexcept>
#endif

#include <nlohmann/json.hpp>

namespace Kale {

	/**
	 * Javascript Standard Object Notation allows for saving and using permanent configuration files, via the nlohmann/json C++
	 * library.
	 */
	using JSON = nlohmann::json;
	
	/**
	 * Represents a single matrix
	 * @tparam c The width/columns of the matrix
	 * @tparam r The height/rows of the matrix
	 * @tparam T The number type stored within the matrix
	 */
	template <size_t w, size_t h, typename T>
	class Matrix {
	public:

		/**
		 * The matrix internal data
		 */
		std::array<T, w*h> data;
		
		/**
		 * Creates a 0 initialized matrix
		 */
		Matrix() {
			data.fill(static_cast<T>(0));
		}

		/**
		 * Creates a matrix from an array, the array must have a length of w*h and follow rows -> columns ie
		 * {
		 * 1, 2, 3,
		 * 4, 5, 6,
		 * 7, 8, 9
		 * }
		 * 
		 * @param arr The array
		 */
		Matrix(const std::array<T, w*h>& arr) : data(arr) {
			// Empty Body
		}

		/**
		 * Creates a matrix from an array, the array must have a length of w*h and follow rows -> columns ie
		 * {
		 * 1, 2, 3,
		 * 4, 5, 6,
		 * 7, 8, 9
		 * }
		 * 
		 * @param arr The array
		 */
		Matrix(std::array<T, w*h>&& arr) : data(std::move(arr)) {
			// Empty Body
		}

		/**
		 * Returns the width of the matrix
		 * @returns the width of the matrix
		 */
		inline size_t width() const {
			return w;
		}

		/**
		 * Returns the height of the matrix
		 * @returns the height of the matrix
		 */
		inline size_t height() const {
			return h;
		}

		/**
		 * Returns an element directly from the data array of the matrix. Elements are stored by
		 * (col, row) => row * width + col
		 * @param i The element index
		 * @returns The element
		 * @throws In debug mode only if the index is invalid, no release checks/throws are made. It will crash.
		 */
		inline T& operator[](size_t i) {
			#ifdef KALE_DEBUG
			if (i > w*h) throw std::runtime_error("Invalid matrix index");
			#endif
			return data[i];
		}

		/**
		 * Returns an element directly from the data array of the matrix. Elements are stored by
		 * (col, row) => row * width + col
		 * @param i The element index
		 * @returns The element
		 * @throws In debug mode only if the index is invalid, no release checks/throws are made. It will crash.
		 */
		inline const T& operator[](size_t i) const {
			#ifdef KALE_DEBUG
			if (i > w*h) throw std::runtime_error("Invalid matrix index");
			#endif
			return data[i];
		}

		/**
		 * Returns an element given the column and row
		 * @param col The column
		 * @param row The row
		 * @returns The element
		 * @throws In debug mode only if the col/row is invalid, no release checks/throws are made. It will crash.
		 */
		inline T& operator()(size_t col, size_t row) {
			#ifdef KALE_DEBUG
			if (col > w || row > h) throw std::runtime_error("Invalid matrix column/row");
			#endif
			return data[row * w + col];
		}

		/**
		 * Returns an element given the column and row
		 * @param col The column
		 * @param row The row
		 * @returns The element
		 * @throws In debug mode only if the col/row is invalid, no release checks/throws are made. It will crash.
		 */
		inline const T& operator()(size_t col, size_t row) const {
			#ifdef KALE_DEBUG
			if (col > w || row > h) throw std::runtime_error("Invalid matrix column/row");
			#endif
			return data[row * w + col];
		}

		/**
		 * Returns an element directly from the data array of the matrix. Elements are stored by
		 * (col, row) => row * width + col
		 * @param i The element index
		 * @returns The element
		 */
		inline T& get(size_t i) {
			return data[i];
		}

		/**
		 * Returns an element directly from the data array of the matrix. Elements are stored by
		 * (col, row) => row * width + col
		 * @param i The element index
		 * @returns The element
		 */
		inline const T& get(size_t i) const {
			return data[i];
		}

		/**
		 * Returns an element given the column and row
		 * @param col The column
		 * @param row The row
		 * @returns The element
		 */
		inline T& get(size_t col, size_t row) {
			return data[row * w + col];
		}

		/**
		 * Returns an element given the column and row
		 * @param col The column
		 * @param row The row
		 * @returns The element
		 */
		inline const T& get(size_t col, size_t row) const {
			return data[row * w + col];
		}

		/**
		 * Adds this matrix to another and returns the result
		 * @param other The matrix to add to
		 * @returns The result of the addition operation
		 */
		Matrix<w, h, T> operator+(const Matrix<w, h, T>& other) const {
			Matrix<w, h, T> mat;
			for (size_t i = 0; i < w*h; i++) mat.data[i] = data[i] + other.data[i];
			return mat;
		}

		/**
		 * Adds this matrix to another and returns the result
		 * @param other The matrix to add to
		 * @returns The result of the addition operation
		 */
		Matrix<w, h, T> operator+(Matrix<w, h, T>&& other) const {
			for (size_t i = 0; i < w*h; i++) other.data[i] += data[i];
			return Matrix<w, h, T>(std::move(other));
		}

		/**
		 * Adds this matrix to another and saves the result in this matrix
		 * @param other The matrix to add to
		 */
		void operator+=(const Matrix<w, h, T>& other) {
			for (size_t i = 0; i < w*h; i++) data[i] += other.data[i];
		}

		/**
		 * Subtracts another matrix from this matrix (this - other)
		 * @param other The matrix to subtract
		 * @returns The result of the subtraction operation
		 */
		Matrix<w, h, T> operator-(const Matrix<w, h, T>& other) const {
			Matrix<w, h, T> mat;
			for (size_t i = 0; i < w*h; i++) mat.data[i] = data[i] - other.data[i];
			return mat;
		}

		/**
		 * Subtracts another matrix from this matrix (this - other)
		 * @param other The matrix to subtract
		 * @returns The result of the subtraction operation
		 */
		Matrix<w, h, T> operator-(Matrix<w, h, T>&& other) const {
			for (size_t i = 0; i < w*h; i++) other.data[i] = data[i] - other.data[i];
			return Matrix<w, h, T>(std::move(other));
		}
		
		/**
		 * Subtracts another matrix from this matrix (this - other) and saves the result in this matrix
		 * @param other The matrix to subtract
		 */
		void operator-=(const Matrix<w, h, T>& other) {
			for (size_t i = 0; i < w*h; i++) data[i] -= other.data[i];
		}

		/**
		 * Scales the matrix by a scalar and returns the result
		 * @param scalar The scalar to scale by
		 * @returns The result of the scaling operation
		 */
		Matrix<w, h, T> operator*(T scalar) const {
			Matrix<w, h, T> mat;
			for (size_t i = 0; i < w*h; i++) mat.data[i] = data[i] * scalar;
			return mat;
		}

		/**
		 * Scales the matrix by a scalar and stores the result in this matrix
		 * @param scalar The scalar to scale by
		 */
		void operator*=(T scalar) {
			for (size_t i = 0; i < w*h; i++) data[i] *= scalar;
		}

		/**
		 * Performs Matrix Multiplication (this * other).
		 * @param other The matrix to multiply with
		 * @returns The resulting matrix from the multiplication
		 */
		template <size_t w2> Matrix<w2, h, T> operator*(const Matrix<w2, w, T>& other) const {
			Matrix<w2, h, T> mat;
			for (size_t i = 0; i < h; i++)
				for (size_t j = 0; j < w2; j++)
					for (size_t k = 0; k < w; k++)
						mat.get(j, i) += get(k, i) * other.get(j, k);
			return mat;
		}

		/**
		 * Transposes the matrix and returns the result of the operation
		 * @returns The transposed matrix
		 */
		Matrix<h, w, T> transpose() const {
			Matrix<h, w, T> mat;
			for (size_t c = 0; c < w; c++)
				for (size_t r = 0; r < h; r++)
					mat.get(r, c) = get(c, r);
			return mat;
		}

		// >------------------- Elementary Row Operations -------------------<

		/**
		 * Swaps 2 rows within the matrix
		 * @param row1 The first row
		 * @param row2 The second row
		 * @throws In debug mode only if the row is invalid, no release checks/throws are made. It will crash.
		 */
		void swapRows(size_t row1, size_t row2) {
			#ifdef KALE_DEBUG
			if (row1 > h || row2 > h) throw std::runtime_error("Invalid matrix row");
			#endif
			std::swap_ranges(data.begin() + row1 * w, data.begin() + row1 * w + w, data.begin() + row2 * w);
		}

		/**
		 * Scales a row within the matrix
		 * @param row The row to scale
		 * @param scalar The scalar to scale by
		 * @throws In debug mode only if the row is invalid, no release checks/throws are made. It will crash.
		 */
		void scaleRow(size_t row, T scalar) {
			#ifdef KALE_DEBUG
			if (row > h) throw std::runtime_error("Invalid matrix row");
			#endif
			for (size_t x = 0; x < w; x++) {
				get(x, row) *= scalar;
			}
		}

		/**
		 * Adds a row scaled to another row and saves it
		 * @param row1 The row to add to (this row is modified)
		 * @param row2 The row to scale by and add from
		 * @param scalar The scalar to scale row2 by
		 * @throws In debug mode only if the row is invalid, no release checks/throws are made. It will crash.
		 */
		void addScaledRow(size_t row1, size_t row2, T scalar) {
			#ifdef KALE_DEBUG
			if (row1 > h || row2 > h) throw std::runtime_error("Invalid matrix row");
			#endif
			for (size_t x = 0; x < w; x++) {
				get(x, row1) += scalar * get(x, row2);
			}
		}

		// >------------------- Template Specializations -------------------<

		/**
		 * Transforms the vector by this matrix
		 * @param vec The vector to transform
		 * @returns The transformed vector
		 */
		template <size_t W = w, size_t H = h> typename std::enable_if<W == 2 && H == 2, Vector2<T>>::type
		operator*(const Vector2<T>& vec) const {
			return Vector2<T>(data[0] * vec.x + data[1] * vec.y, data[2] * vec.x + data[2] * vec.y);
		}

		/**
		 * Transforms the vector by this matrix
		 * @param vec The vector to transform
		 * @returns The transformed vector
		 */
		template <size_t W = w, size_t H = h> typename std::enable_if<W == 2 && H == 2, Vector2<T>>::type
		operator*(Vector2<T>&& vec) const {
			vec.x = data[0] * vec.x + data[1] * vec.y;
			vec.y = data[2] * vec.x + data[2] * vec.y;
			return Vector2<T>(std::move(vec));
		}

		/**
		 * Transforms the vector by this matrix
		 * @param vec The vector to transform
		 * @returns The transformed vector
		 */
		template <size_t W = w, size_t H = h> typename std::enable_if<W == 3 && H == 3, Vector3<T>>::type
		operator*(const Vector3<T>& vec) const {
			return Vector3<T>(
				data[0] * vec.x + data[1] * vec.y + data[2] * vec.z,
				data[3] * vec.x + data[4] * vec.y + data[5] * vec.z,
				data[6] * vec.x + data[7] * vec.y + data[8] * vec.z
			);
		}

		/**
		 * Transforms the vector by this matrix
		 * @param vec The vector to transform
		 * @returns The transformed vector
		 */
		template <size_t W = w, size_t H = h> typename std::enable_if<W == 3 && H == 3, Vector3<T>>::type
		operator*(Vector3<T>&& vec) const {
			vec.x = data[0] * vec.x + data[1] * vec.y + data[2] * vec.z;
			vec.y = data[3] * vec.x + data[4] * vec.y + data[5] * vec.z;
			vec.z = data[6] * vec.x + data[7] * vec.y + data[8] * vec.z;
			return Vector3<T>(std::move(vec));
		}

		/**
		 * Transforms the vector by this matrix
		 * @param vec The vector to transform
		 * @returns The transformed vector
		 */
		template <size_t W = w, size_t H = h> typename std::enable_if<W == 4 && H == 4, Vector4<T>>::type
		operator*(const Vector4<T>& vec) const {
			return Vector4<T>(
				data[0] * vec.x + data[1] * vec.y + data[2] * vec.z + data[3] * vec.w,
				data[4] * vec.x + data[5] * vec.y + data[6] * vec.z + data[7] * vec.w,
				data[8] * vec.x + data[9] * vec.y + data[10] * vec.z + data[11] * vec.w,
				data[12] * vec.x + data[13] * vec.y + data[14] * vec.z + data[15] * vec.w
			);
		}

		/**
		 * Transforms the vector by this matrix
		 * @param vec The vector to transform
		 * @returns The transformed vector
		 */
		template <size_t W = w, size_t H = h> typename std::enable_if<W == 4 && H == 4, Vector4<T>>::type
		operator*(Vector4<T>&& vec) const {
			vec.x = data[0] * vec.x + data[1] * vec.y + data[2] * vec.z + data[3] * vec.w;
			vec.y = data[4] * vec.x + data[5] * vec.y + data[6] * vec.z + data[7] * vec.w;
			vec.z = data[8] * vec.x + data[9] * vec.y + data[10] * vec.z + data[11] * vec.w;
			vec.w = data[12] * vec.x + data[13] * vec.y + data[14] * vec.z + data[15] * vec.w;
			return Vector4<T>(std::move(vec));
		}

		/**
		 * Retrieves the determinant for any square matrix
		 * @tparam R the return type to use (larger matrices may require a large type for determinants)
		 * @returns The determinant of the matrix
		 */
		template <typename R, size_t W = w, size_t H = h>
		typename std::enable_if<W == 2 && H == 2, R>::type det() const {
			
			return static_cast<R>(data[0] * data[3] - data[1] * data[2]);
		}

		/**
		 * Retrieves the determinant for any square matrix
		 * @tparam R the return type to use (larger matrices may require a large type for determinants)
		 * @returns The determinant of the matrix
		 */
		template <typename R, size_t W = w, size_t H = h>
		typename std::enable_if<W == 3 && H == 3, R>::type det() const {
			return static_cast<R>(
				data[0] * data[4] * data[8] +
				data[1] * data[5] * data[6] +
				data[2] * data[3] * data[7] -
				data[6] * data[4] * data[2] -
				data[7] * data[5] * data[0] -
				data[8] * data[3] * data[1]
			);
		}

		/**
		 * Retrieves the determinant for any square matrix
		 * @tparam R the return type to use (larger matrices may require a large type for determinants)
		 * @returns The determinant of the matrix
		 */
		template <typename R, size_t W = w, size_t H = h>
		typename std::enable_if<W == H && (W > 3) && std::is_signed<R>::value, R>::type det() const {
			throw std::runtime_error("Matrix Determinants are Unimplemented");
		}

		/**
		 * Calculates the inverse of the matrix for a square matrix
		 * @returns The inverted matrix
		 */
		template <size_t W = w, size_t H = h>
		typename std::enable_if<W == 2 && H == 2, Matrix<W, H, T>>::type inverse() {
			return Matrix<W, H, T>({
				data[3], -data[1],
				-data[2], data[0]
			}) * (static_cast<T>(1)/det<T>());
		}

		/**
		 * Calculates the inverse of the matrix for a square matrix
		 * @returns The inverted matrix
		 */
		template <size_t W = w, size_t H = h>
		typename std::enable_if<W == 3 && H == 3, Matrix<W, H, T>>::type inverse() {
			auto calc = [&](size_t a, size_t b, size_t c, size_t d) -> T {
				return Matrix<2, 2, T>({data[a], data[b], data[c], data[d]}).template det<T>();
			};
			return Matrix<3, 3, T>({
				calc(4, 5, 7, 8), -calc(1, 2, 7, 8), calc(1, 2, 4, 5),
				-calc(3, 5, 6, 8), calc(0, 2, 6, 8), -calc(0, 2, 3, 5),
				calc(3, 4, 6, 7), -calc(0, 1, 6, 7), calc(0, 1, 3, 4)
			}) * (static_cast<T>(1)/det<T>());
		}
	};

	typedef Matrix<2, 2, int> Matrix2i;
	typedef Matrix<2, 2, long> Matrix2l;
	typedef Matrix<2, 2, float> Matrix2f;
	typedef Matrix<2, 2, double> Matrix2d;

	typedef Matrix<3, 3, int> Matrix3i;
	typedef Matrix<3, 3, long> Matrix3l;
	typedef Matrix<3, 3, float> Matrix3f;
	typedef Matrix<3, 3, double> Matrix3d;

	typedef Matrix<4, 4, int> Matrix4i;
	typedef Matrix<4, 4, long> Matrix4l;
	typedef Matrix<4, 4, float> Matrix4f;
	typedef Matrix<4, 4, double> Matrix4d;

	template <size_t w, size_t h> using MatrixI = Matrix<w, h, int>;
	template <size_t w, size_t h> using MatrixL = Matrix<w, h, long>;
	template <size_t w, size_t h> using MatrixF = Matrix<w, h, float>;
	template <size_t w, size_t h> using MatrixD = Matrix<w, h, double>;

	/**
	 * Prints a matrix to an output stream
	 */
	template <size_t w, size_t h, typename T>
	std::ostream& operator<<(std::ostream& os, const Kale::Matrix<w, h, T>& mat) {
		os << "Mat(Width: " << w << ", Height: " << h << ") :";
		for (size_t y = 0; y < h; y++) {
			os << "\n";
			for (size_t x = 0; x < w; x++) {
				os << std::setw(5) << std::setfill(' ') << mat(x, y) << " ";
			}
		}
		return os;
	}

	/**
	 * Creates a matrix from a json
	 */
	template <size_t w, size_t h, typename T>
	void from_json(const JSON& j, Matrix<w, h, T>& p) {
		p = j.get<std::array<T, w*h>>();
	}

	/**
	 * Populates a json from a matrix
	 */
	template <size_t w, size_t h, typename T>
	void to_json(JSON& j, const Matrix<w, h, T>& p) {
		j = JSON(p.data);
	}

}
