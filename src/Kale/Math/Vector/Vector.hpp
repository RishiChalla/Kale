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

#include <Kale/Math/Utils/Utils.hpp>

#include <cmath>
#include <algorithm>
#include <ostream>
#include <type_traits>
#include <limits>

#include <core/SkColor.h>
#include <core/SkPoint.h>
#include <core/SkPoint3.h>

namespace Kale {

	/**
	 * Represents a vector in a 2 dimensional space
	 */
	template <typename T>
	class Vector2 {
	public:
		T x, y;

		Vector2() : x(0), y(0) {}
		Vector2(T x, T y) : x(x), y(y) {}
		Vector2(T v[2]) : x(v[0]), y(v[1]) {}

		void operator=(T n) {
			x = n;
			y = n;
		}
		void operator=(T v[2]) {
			x = v[0];
			y = v[1];
		}

		void operator+=(Vector2<T> o) {
			x += o.x;
			y += o.y;
		}
		void operator-=(Vector2<T> o) {
			x -= o.x;
			y -= o.y;
		}
		void operator*=(Vector2<T> o) {
			x *= o.x;
			y *= o.y;
		}
		void operator/=(Vector2<T> o) {
			x /= o.x;
			y /= o.y;
		}

		void operator+=(T v) {
			x += v;
			y += v;
		}
		void operator-=(T v) {
			x -= v;
			y -= v;
		}
		void operator*=(T v) {
			x *= v;
			y *= v;
		}
		void operator/=(T v) {
			x /= v;
			y /= v;
		}

		Vector2<T> operator-() const { return {-x, -y}; };

		Vector2<T> operator+(Vector2<T> o) const { return Vector2<T>(x + o.x, y + o.y); }
		Vector2<T> operator-(Vector2<T> o) const { return Vector2<T>(x - o.x, y - o.y); }
		Vector2<T> operator*(Vector2<T> o) const { return Vector2<T>(x * o.x, y * o.y); }
		Vector2<T> operator/(Vector2<T> o) const { return Vector2<T>(x / o.x, y / o.y); }

		Vector2<T> operator+(T v) const { return Vector2<T>(x + v, y + v); }
		Vector2<T> operator-(T v) const { return Vector2<T>(x - v, y - v); }
		Vector2<T> operator*(T v) const { return Vector2<T>(x * v, y * v); }
		Vector2<T> operator/(T v) const { return Vector2<T>(x / v, y / v); }

		friend Vector2<T> operator+(float n, Vector2<T> v) { return Vector2<T>(n + v.x, n + v.y); }
		friend Vector2<T> operator-(float n, Vector2<T> v) { return Vector2<T>(n - v.x, n - v.y); }
		friend Vector2<T> operator*(float n, Vector2<T> v) { return Vector2<T>(n * v.x, n * v.y); }
		friend Vector2<T> operator/(float n, Vector2<T> v) { return Vector2<T>(n / v.x, n / v.y); }

		bool operator>(Vector2<T> o) const { return x > o.x && y > o.y; }
		bool operator<(Vector2<T> o) const { return x < o.x && y < o.y; }
		bool operator<=(Vector2<T> o) const { return x <= o.x && y <= o.y; }
		bool operator>=(Vector2<T> o) const { return x >= o.x && y >= o.y; }
		template <typename A = T> typename std::enable_if<not std::is_floating_point<A>::value, bool>::type operator==(Vector2<T> o) const {
			return x == o.x && y == o.y;
		}
		template <typename A = T> typename std::enable_if<std::is_floating_point<A>::value, bool>::type operator==(Vector2<T> o) const {
			return isFloatingEqual(x, o.x) && isFloatingEqual(y, o.y);
		}

		T dot(Vector2<T> o) const { return o.x * x + o.y * y; }
		T cross(Vector2<T> o) const { return x * o.y - y * o.x; }
		template <typename A = T> typename std::enable_if<std::is_floating_point<A>::value, T>::type magnitude() const {
			return sqrt(x * x + y * y);
		}

		Vector2<T> clamp(T minX, T maxX, T minY, T maxY) const {
			return Vector2<T>(std::clamp(x, minX, maxX), std::clamp(y, minY, maxY));
		}
		Vector2<T> clamp(Vector2<T> min, Vector2<T> max) const {
			return Vector2<T>(std::clamp(x, min.x, max.x), std::clamp(y, min.y, max.y));
		}

		void clampTo(T minX, T maxX, T minY, T maxY) {
			x = std::clamp(x, minX, maxX);
			y = std::clamp(y, minY, maxY);
		}
		void clampTo(Vector2<T> min, Vector2<T> max) {
			x = std::clamp(x, min.x, max.x);
			y = std::clamp(y, min.y, max.y);
		}

		template <typename A>
		Vector2<A> cast() const {
			return Vector2<A>(static_cast<A>(x), static_cast<A>(y));
		}

		template <typename A = T> typename std::enable_if<std::is_floating_point<A>::value, Vector2<T>>::type unit() const {
			return *this / magnitude();
		}

		Vector2<T> perpendicular(Vector2<T> o) const {
			return {x + o.y - y, y - o.x + x};
		}
		Vector2<T> project(Vector2<T> o) {
			float k = dot(o) / o.dot(o);
			return {k * o.x, k * o.y};
		}

		template<typename U = T, typename = typename std::enable_if<std::is_same<U, float>::value>::type>
		inline operator SkPoint() const {
			return SkPoint::Make(x, y);
		}

		Vector2<T> xy() const { return {x, y}; }
		Vector2<T> yx() const { return {y, x}; }

		static Vector2<T> one() { return {1, 1}; }
		static Vector2<T> zero() { return {0, 0}; }
		static Vector2<T> right() { return {1, 0}; }
		static Vector2<T> up() { return {0, 1}; }
		static Vector2<T> max() { return {std::numeric_limits<T>::max(), std::numeric_limits<T>::max()}; }
		static Vector2<T> min() { return {std::numeric_limits<T>::min(), std::numeric_limits<T>::min()}; }
	};

	/**
	 * Represents a vector in a 3 dimensional space
	 */
	template <typename T>
	class Vector3 {
	public:
		T x, y, z;

		Vector3() : x(0), y(0), z(0) {}
		Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
		Vector3(T v[3]) : x(v[0]), y(v[1]), z(v[2]) {}

		Vector3(Vector2<T> v, T z) : x(v.x), y(v.y), z(z) {}
		Vector3(T x, Vector2<T> v) : x(x), y(v.x), z(v.y) {}

		void operator=(T n) {
			x = n;
			y = n;
			z = n;
		}
		void operator=(T v[3]) {
			x = v[0];
			y = v[1];
			z = v[2];
		}

		void operator+=(Vector3<T> o) {
			x += o.x;
			y += o.y;
			z += o.z;
		}
		void operator-=(Vector3<T> o) {
			x -= o.x;
			y -= o.y;
			z -= o.z;
		}
		void operator*=(Vector3<T> o) {
			x *= o.x;
			y *= o.y;
			z *= o.z;
		}
		void operator/=(Vector3<T> o) {
			x /= o.x;
			y /= o.y;
			z /= o.z;
		}

		void operator+=(T v) {
			x += v;
			y += v;
			z += v;
		}
		void operator-=(T v) {
			x -= v;
			y -= v;
			z -= v;
		}
		void operator*=(T v) {
			x *= v;
			y *= v;
			z *= v;
		}
		void operator/=(T v) {
			x /= v;
			y /= v;
			z /= v;
		}

		Vector3<T> operator-() const { return {-x, -y, -z}; };

		Vector3<T> operator+(Vector3<T> o) const { return Vector3<T>(x + o.x, y + o.y, z + o.z); }
		Vector3<T> operator-(Vector3<T> o) const { return Vector3<T>(x - o.x, y - o.y, z - o.z); }
		Vector3<T> operator*(Vector3<T> o) const { return Vector3<T>(x * o.x, y * o.y, z * o.z); }
		Vector3<T> operator/(Vector3<T> o) const { return Vector3<T>(x / o.x, y / o.y, z / o.z); }
		
		Vector3<T> operator+(T v) const { return Vector3<T>(x + v, y + v, z + v); }
		Vector3<T> operator-(T v) const { return Vector3<T>(x - v, y - v, z - v); }
		Vector3<T> operator*(T v) const { return Vector3<T>(x * v, y * v, z * v); }
		Vector3<T> operator/(T v) const { return Vector3<T>(x / v, y / v, z / v); }

		friend Vector3<T> operator+(float n, Vector3<T> v) { return Vector3<T>(n + v.x, n + v.y, n + v.z); }
		friend Vector3<T> operator-(float n, Vector3<T> v) { return Vector3<T>(n - v.x, n - v.y, n - v.z); }
		friend Vector3<T> operator*(float n, Vector3<T> v) { return Vector3<T>(n * v.x, n * v.y, n * v.z); }
		friend Vector3<T> operator/(float n, Vector3<T> v) { return Vector3<T>(n / v.x, n / v.y, n / v.z); }

		bool operator>(Vector3<T> o) const { return x > o.x && y > o.y && z > o.z; }
		bool operator<(Vector3<T> o) const { return x < o.x && y < o.y && z < o.z; }
		bool operator>=(Vector3<T> o) const { return x >= o.x && y >= o.y && z >= o.z; }
		bool operator<=(Vector3<T> o) const { return x <= o.x && y <= o.y && z <= o.z; }
		template <typename A = T> typename std::enable_if<not std::is_floating_point<A>::value, bool>::type operator==(Vector3<T> o) const {
			return x == o.x && y == o.y && z == o.z;
		}
		template <typename A = T> typename std::enable_if<std::is_floating_point<A>::value, bool>::type operator==(Vector3<T> o) const {
			return isFloatingEqual(x, o.x) && isFloatingEqual(y, o.y) && isFloatingEqual(z, o.z);
		}

		T dot(Vector3<T> o) const { return o.x * x + o.y * y + o.z * z; }
		template <typename A = T> typename std::enable_if<std::is_floating_point<A>::value, T>::type magnitude() const {
			return sqrt(x * x + y * y + z * z);
		}

		Vector3<T> clamp(T minX, T maxX, T minY, T maxY, T minZ, T maxZ) const {
			return Vector3<T>(std::clamp(x, minX, maxX), std::clamp(y, minY, maxY), std::clamp(z, minZ, maxZ));
		}
		Vector3<T> clamp(Vector3<T> min, Vector3<T> max) const {
			return Vector3<T>(std::clamp(x, min.x, max.x), std::clamp(y, min.y, max.y), std::clamp(z, min.z, max.z));
		}

		void clampTo(T minX, T maxX, T minY, T maxY, T minZ, T maxZ) {
			x = std::clamp(x, minX, maxX);
			y = std::clamp(y, minY, maxY);
			z = std::clamp(z, minZ, maxZ);
		}
		void clampTo(Vector3<T> min, Vector3<T> max) {
			x = std::clamp(x, min.x, max.x);
			y = std::clamp(y, min.y, max.y);
			z = std::clamp(z, min.z, max.z);
		}

		template <typename A> Vector3<A> cast() const {
			return Vector3<A>(static_cast<A>(x), static_cast<A>(y), static_cast<A>(z));
		}

		template <typename A = T> typename std::enable_if<std::is_floating_point<A>::value, Vector3<T>>::type unit() const {
			return *this / magnitude();
		}

		template<typename U = T, typename = typename std::enable_if<std::is_same<U, float>::value>::type>
		operator SkColor4f() const {
			SkColor4f color;
			color.fR = x;
			color.fG = y;
			color.fB = z;
			color.fA = 1.0f;
			return color;
		}

		template<typename U = T, typename = typename std::enable_if<std::is_same<U, float>::value>::type>
		inline operator SkPoint3() const {
			return SkPoint3::Make(x, y, z);
		}

		Vector2<T> xy() const { return {x, y}; }
		Vector2<T> xz() const { return {x, z}; }
		Vector2<T> yx() const { return {y, x}; }
		Vector2<T> yz() const { return {y, z}; }
		Vector2<T> zx() const { return {z, x}; }
		Vector2<T> zy() const { return {z, y}; }

		Vector3<T> xyz() const { return {x, y, z}; }
		Vector3<T> xzy() const { return {x, z, y}; }
		Vector3<T> yxz() const { return {y, x, z}; }
		Vector3<T> yzx() const { return {y, z, x}; }
		Vector3<T> zxy() const { return {z, x, y}; }
		Vector3<T> zyx() const { return {z, y, x}; }

		static Vector3<T> zero() { return {0, 0, 0}; }
		static Vector3<T> one() { return {1, 1, 1}; }
		static Vector3<T> right() { return {1, 0, 0}; }
		static Vector3<T> up() { return {0, 1, 0}; }
		static Vector3<T> front() { return {0, 0, 1}; }
		static Vector3<T> max() { return {std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max()}; }
		static Vector3<T> min() { return {std::numeric_limits<T>::min(), std::numeric_limits<T>::min(), std::numeric_limits<T>::min()}; }
	};

	/**
	 * Represents a vector in a 3 dimensional space
	 */
	template <typename T>
	class Vector4 {
	public:
		T x, y, z, w;

		template<typename U = T, typename = typename std::enable_if<std::is_same<U, float>::value>::type>
		Vector4(float r, float g, float b) : x(r/255.0f), y(g/255.0f), z(b/255.0f), w(1.0f) {}

		template<typename U = T, typename = typename std::enable_if<std::is_same<U, float>::value>::type>
		Vector4(int hex) : w(1.0f) {
			x = ((hex >> 16) & 0xFF) / 255.0f;
			y = ((hex >> 8) & 0xFF) / 255.0f;
			z = (hex & 0xFF) / 255.0f;
		}

		template<typename U = T, typename = typename std::enable_if<std::is_same<U, float>::value>::type>
		Vector4(int hex, float alpha) : w(alpha) {
			x = ((hex >> 16) & 0xFF) / 255.0f;
			y = ((hex >> 8) & 0xFF) / 255.0f;
			z = (hex & 0xFF) / 255.0f;
		}

		Vector4() : x(0), y(0), z(0), w(0) {}
		Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
		Vector4(T v[4]) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}

		Vector4(Vector2<T> v, T z, T w) : x(v.x), y(v.y), z(z), w(w) {}
		Vector4(T x, Vector2<T> v, T w) : x(x), y(v.x), z(v.y), w(w) {}
		Vector4(T x, T y, Vector2<T> v) : x(x), y(y), z(v.x), w(v.y) {}

		Vector4(Vector3<T> v, T w) : x(v.x), y(v.y), z(v.z), w(w) {}
		Vector4(T x, Vector3<T> v) : x(x), y(v.x), z(v.y), w(v.z) {}

		void operator=(T n) {
			x = n;
			y = n;
			z = n;
			w = n;
		}
		void operator=(T v[4]) {
			x = v[0];
			y = v[1];
			z = v[2];
			w = v[3];
		}

		void operator+=(Vector4<T> o) {
			x += o.x;
			y += o.y;
			z += o.z;
			w += o.w;
		}
		void operator-=(Vector4<T> o) {
			x -= o.x;
			y -= o.y;
			z -= o.z;
			w -= o.w;
		}
		void operator*=(Vector4<T> o) {
			x *= o.x;
			y *= o.y;
			z *= o.z;
			w *= o.w;
		}
		void operator/=(Vector4<T> o) {
			x /= o.x;
			y /= o.y;
			z /= o.z;
			w /= o.w;
		}

		void operator+=(T v) {
			x += v;
			y += v;
			z += v;
			w += v;
		}
		void operator-=(T v) {
			x -= v;
			y -= v;
			z -= v;
			w -= v;
		}
		void operator*=(T v) {
			x *= v;
			y *= v;
			z *= v;
			w *= v;
		}
		void operator/=(T v) {
			x /= v;
			y /= v;
			z /= v;
			w /= v;
		}

		Vector4<T> operator-() const { return {-x, -y, -z, -w}; };

		Vector4<T> operator+(Vector4<T> o) const { return Vector4<T>(x + o.x, y + o.y, z + o.z, w + o.w); }
		Vector4<T> operator-(Vector4<T> o) const { return Vector4<T>(x - o.x, y - o.y, z - o.z, w - o.w); }
		Vector4<T> operator*(Vector4<T> o) const { return Vector4<T>(x * o.x, y * o.y, z * o.z, w * o.w); }
		Vector4<T> operator/(Vector4<T> o) const { return Vector4<T>(x / o.x, y / o.y, z / o.z, w / o.w); }

		Vector4<T> operator+(T v) const { return Vector4<T>(x + v, y + v, z + v, w + v); }
		Vector4<T> operator-(T v) const { return Vector4<T>(x - v, y - v, z - v, w - v); }
		Vector4<T> operator*(T v) const { return Vector4<T>(x * v, y * v, z * v, w * v); }
		Vector4<T> operator/(T v) const { return Vector4<T>(x / v, y / v, z / v, w / v); }

		friend Vector4<T> operator+(float n, Vector4<T> v) { return Vector4<T>(n + v.x, n + v.y, n + v.z, n + v.w); }
		friend Vector4<T> operator-(float n, Vector4<T> v) { return Vector4<T>(n - v.x, n - v.y, n - v.z, n - v.w); }
		friend Vector4<T> operator*(float n, Vector4<T> v) { return Vector4<T>(n * v.x, n * v.y, n * v.z, n * v.w); }
		friend Vector4<T> operator/(float n, Vector4<T> v) { return Vector4<T>(n / v.x, n / v.y, n / v.z, n / v.w); }

		bool operator>(Vector4<T> o) const { return x > o.x && y > o.y && z > o.z && w > o.w; }
		bool operator<(Vector4<T> o) const { return x < o.x && y < o.y && z < o.z && w < o.w; }
		bool operator>=(Vector4<T> o) const { return x >= o.x && y >= o.y && z >= o.z && w >= o.w; }
		bool operator<=(Vector4<T> o) const { return x <= o.x && y <= o.y && z <= o.z && w <= o.w; }
		template <typename A = T> typename std::enable_if<not std::is_floating_point<A>::value, bool>::type operator==(Vector4<T> o) const {
			return x == o.x && y == o.y && z == o.z && w == o.w;
		}
		template <typename A = T> typename std::enable_if<std::is_floating_point<A>::value, bool>::type operator==(Vector4<T> o) const {
			return isFloatingEqual(x, o.x) && isFloatingEqual(y, o.y) && isFloatingEqual(z, o.z) && isFloatingEqual(w, o.w);
		}

		T dot(Vector4<T> o) const { return o.x * x + o.y * y + o.z * z + o.w * w; }
		template <typename A = T> typename std::enable_if<std::is_floating_point<A>::value, T>::type magnitude() const {
			return sqrt(x * x + y * y + z * z + w * w);
		}

		Vector4<T> clamp(T minX, T maxX, T minY, T maxY, T minZ, T maxZ, T minW, T maxW) const {
			return Vector4<T>(std::clamp(x, minX, maxX), std::clamp(y, minY, maxY),
				std::clamp(z, minZ, maxZ), std::clamp(w, minW, maxW));
		}
		Vector4<T> clamp(Vector4<T> min, Vector4<T> max) const {
			return Vector4<T>(std::clamp(x, min.x, max.x), std::clamp(y, min.y, max.y),
				std::clamp(z, min.z, max.z), std::clamp(w, min.w, max.w));
		}

		void clampTo(T minX, T maxX, T minY, T maxY, T minZ, T maxZ, T minW, T maxW) {
			x = std::clamp(x, minX, maxX);
			y = std::clamp(y, minY, maxY);
			z = std::clamp(z, minZ, maxZ);
			w = std::clamp(w, minW, maxW);
		}
		void clampTo(Vector4<T> min, Vector4<T> max) {
			x = std::clamp(x, min.x, max.x);
			y = std::clamp(y, min.y, max.y);
			z = std::clamp(z, min.z, max.z);
			w = std::clamp(w, min.w, max.w);
		}

		template <typename A> Vector4<A> cast() const {
			return Vector4<A>(static_cast<A>(x), static_cast<A>(y), static_cast<A>(z), static_cast<A>(w));
		}

		template <typename A = T> typename std::enable_if<std::is_floating_point<A>::value, Vector4<T>>::type unit() const {
			return *this / magnitude();
		}

		template<typename U = T, typename = typename std::enable_if<std::is_same<U, float>::value>::type>
		operator SkColor4f() const {
			SkColor4f color;
			color.fR = x;
			color.fG = y;
			color.fB = z;
			color.fA = w;
			return color;
		}

		Vector2<T> xy() const { return {x, y}; }
		Vector2<T> xz() const { return {x, z}; }
		Vector2<T> xw() const { return {x, w}; }
		Vector2<T> yx() const { return {y, x}; }
		Vector2<T> yz() const { return {y, z}; }
		Vector2<T> yw() const { return {y, w}; }
		Vector2<T> zx() const { return {z, x}; }
		Vector2<T> zy() const { return {z, y}; }
		Vector2<T> zw() const { return {z, w}; }
		Vector2<T> wx() const { return {w, x}; }
		Vector2<T> wy() const { return {w, y}; }
		Vector2<T> wz() const { return {w, z}; }

		Vector3<T> xyz() const { return {x, y, z}; }
		Vector3<T> xyw() const { return {x, y, w}; }
		Vector3<T> xzy() const { return {x, z, y}; }
		Vector3<T> xzw() const { return {x, z, w}; }
		Vector3<T> xwy() const { return {x, w, y}; }
		Vector3<T> xwz() const { return {x, w, z}; }
		Vector3<T> yxz() const { return {y, x, z}; }
		Vector3<T> yxw() const { return {y, x, w}; }
		Vector3<T> yzx() const { return {y, z, x}; }
		Vector3<T> yzw() const { return {y, z, w}; }
		Vector3<T> ywx() const { return {y, w, x}; }
		Vector3<T> ywz() const { return {y, w, z}; }
		Vector3<T> zxy() const { return {z, x, y}; }
		Vector3<T> zxw() const { return {z, x, w}; }
		Vector3<T> zyx() const { return {z, y, x}; }
		Vector3<T> zyw() const { return {z, y, w}; }
		Vector3<T> zwx() const { return {z, w, x}; }
		Vector3<T> zwy() const { return {z, w, y}; }
		Vector3<T> wxy() const { return {w, x, y}; }
		Vector3<T> wxz() const { return {w, x, z}; }
		Vector3<T> wyx() const { return {w, y, x}; }
		Vector3<T> wyz() const { return {w, y, z}; }
		Vector3<T> wzx() const { return {w, z, x}; }
		Vector3<T> wzy() const { return {w, z, y}; }

		Vector4<T> wzyx() const { return {w, z, y, x}; }
		Vector4<T> zwyx() const { return {z, w, y, x}; }
		Vector4<T> wyzx() const { return {w, y, z, x}; }
		Vector4<T> ywzx() const { return {y, w, z, x}; }
		Vector4<T> zywx() const { return {z, y, w, x}; }
		Vector4<T> yzwx() const { return {y, z, w, x}; }
		Vector4<T> wzxy() const { return {w, z, x, y}; }
		Vector4<T> zwxy() const { return {z, w, x, y}; }
		Vector4<T> wxzy() const { return {w, x, z, y}; }
		Vector4<T> xwzy() const { return {x, w, z, y}; }
		Vector4<T> zxwy() const { return {z, x, w, y}; }
		Vector4<T> xzwy() const { return {x, z, w, y}; }
		Vector4<T> wyxz() const { return {w, y, x, z}; }
		Vector4<T> ywxz() const { return {y, w, x, z}; }
		Vector4<T> wxyz() const { return {w, x, y, z}; }
		Vector4<T> xwyz() const { return {x, w, y, z}; }
		Vector4<T> yxwz() const { return {y, x, w, z}; }
		Vector4<T> xywz() const { return {x, y, w, z}; }
		Vector4<T> zyxw() const { return {z, y, x, w}; }
		Vector4<T> yzxw() const { return {y, z, x, w}; }
		Vector4<T> zxyw() const { return {z, x, y, w}; }
		Vector4<T> xzyw() const { return {x, z, y, w}; }
		Vector4<T> yxzw() const { return {y, x, z, w}; }
		Vector4<T> xyzw() const { return {x, y, z, w}; }

		static Vector4<T> zero() { return {0, 0, 0, 0}; }
		static Vector4<T> one() { return {1, 1, 1, 1}; }
		static Vector4<T> max() {
			return {std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max()};
		}
		static Vector4<T> min() {
			return {std::numeric_limits<T>::min(), std::numeric_limits<T>::min(), std::numeric_limits<T>::min(), std::numeric_limits<T>::min()};
		}
	};

	typedef Vector2<char> Vector2c;
	typedef Vector2<unsigned char> Vector2uc;
	typedef Vector2<int> Vector2i;
	typedef Vector2<unsigned int> Vector2ui;
	typedef Vector2<short> Vector2s;
	typedef Vector2<unsigned short> Vector2us;
	typedef Vector2<long> Vector2l;
	typedef Vector2<unsigned long> Vector2ul;
	typedef Vector2<long long> Vector2ll;
	typedef Vector2<unsigned long long> Vector2ull;
	typedef Vector2<size_t> Vector2st;
	typedef Vector2<uint32_t> Vector2ui32;
	typedef Vector2<float> Vector2f;
	typedef Vector2<double> Vector2d;

	typedef Vector3<char> Vector3c;
	typedef Vector3<unsigned char> Vector3uc;
	typedef Vector3<int> Vector3i;
	typedef Vector3<unsigned int> Vector3ui;
	typedef Vector3<short> Vector3s;
	typedef Vector3<unsigned short> Vector3us;
	typedef Vector3<long> Vector3l;
	typedef Vector3<unsigned long> Vector3ul;
	typedef Vector3<long long> Vector3ll;
	typedef Vector3<unsigned long long> Vector3ull;
	typedef Vector3<size_t> Vector3st;
	typedef Vector3<uint32_t> Vector3ui32;
	typedef Vector3<float> Vector3f;
	typedef Vector3<double> Vector3d;

	typedef Vector4<char> Vector4c;
	typedef Vector4<unsigned char> Vector4uc;
	typedef Vector4<int> Vector4i;
	typedef Vector4<unsigned int> Vector4ui;
	typedef Vector4<short> Vector4s;
	typedef Vector4<unsigned short> Vector4us;
	typedef Vector4<long> Vector4l;
	typedef Vector4<unsigned long> Vector4ul;
	typedef Vector4<long long> Vector4ll;
	typedef Vector4<unsigned long long> Vector4ull;
	typedef Vector4<size_t> Vector4st;
	typedef Vector4<uint32_t> Vector4ui32;
	typedef Vector4<float> Vector4f;
	typedef Vector4<double> Vector4d;

	typedef Vector4f Color;

	/**
	 * Prints a vector to an output stream
	 */
	template <typename T>
	std::ostream& operator<<(std::ostream& os, const Kale::Vector2<T>& vec) {
		os << "Vec2(" << vec.x << ", " << vec.y << ")";
		return os;
	}

	/**
	 * Prints a vector to an output stream
	 */
	template <typename T>
	std::ostream& operator<<(std::ostream& os, const Kale::Vector3<T>& vec) {
		os << "Vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return os;
	}

	/**
	 * Prints a vector to an output stream
	 */
	template <typename T>
	std::ostream& operator<<(std::ostream& os, const Kale::Vector4<T>& vec) {
		os << "Vec4(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
		return os;
	}

}
