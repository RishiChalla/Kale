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

#include <cmath>
#include <algorithm>
#include <ostream>

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

		Vector2<T> operator+(Vector2<T> o) const { return Vector2<T>(x + o.x, y + o.y); }
		Vector2<T> operator-(Vector2<T> o) const { return Vector2<T>(x - o.x, y - o.y); }
		Vector2<T> operator*(Vector2<T> o) const { return Vector2<T>(x * o.x, y * o.y); }
		Vector2<T> operator/(Vector2<T> o) const { return Vector2<T>(x / o.x, y / o.y); }

		T dot(Vector2<T> o) const { return o.x * x + o.y * y; }
		double magnitude() const { return sqrt(static_cast<double>(x * x + y * y)); }

		Vector2<T> clamp(T minX, T maxX, T minY, T maxY) const {
			return Vector2<T>(std::clamp(x, minX, maxX), std::clamp(y, minY, maxY));
		}

		void clampTo(T minX, T maxX, T minY, T maxY) {
			x = std::clamp(x, minX, maxX);
			y = std::clamp(y, minY, maxY);
		}

		template <typename A>
		Vector2<A> cast() const {
			return Vector2<A>(static_cast<A>(x), static_cast<A>(y));
		}
	};

	/**
	 * Represents a vector in a 3 dimensional space
	 */
	template <typename T>
	class Vector3
	{
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

		Vector3<T> operator+(Vector3<T> o) const { return Vector3<T>(x + o.x, y + o.y, z + o.z); }
		Vector3<T> operator-(Vector3<T> o) const { return Vector3<T>(x - o.x, y - o.y, z - o.z); }
		Vector3<T> operator*(Vector3<T> o) const { return Vector3<T>(x * o.x, y * o.y, z * o.z); }
		Vector3<T> operator/(Vector3<T> o) const { return Vector3<T>(x / o.x, y / o.y, z / o.z); }

		T dot(Vector3<T> o) const { return o.x * x + o.y * y + o.z * z; }
		double magnitude() const { return sqrt(static_cast<double>(x * x + y * y + z * z)); }

		Vector3<T> clamp(T minX, T maxX, T minY, T maxY, T minZ, T maxZ) const {
			return Vector3<T>(std::clamp(x, minX, maxX), std::clamp(y, minY, maxY), std::clamp(z, minZ, maxZ));
		}

		void clampTo(T minX, T maxX, T minY, T maxY, T minZ, T maxZ) {
			x = std::clamp(x, minX, maxX);
			y = std::clamp(y, minY, maxY);
			z = std::clamp(z, minZ, maxZ);
		}

		template <typename A>
		Vector3<A> cast() const {
			return Vector3<A>(static_cast<A>(x), static_cast<A>(y), static_cast<A>(z));
		}
	};

	/**
	 * Represents a vector in a 3 dimensional space
	 */
	template <typename T>
	class Vector4
	{
	public:
		T x, y, z, w;

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

		Vector4<T> operator+(Vector4<T> o) const { return Vector4<T>(x + o.x, y + o.y, z + o.z, w + o.w); }
		Vector4<T> operator-(Vector4<T> o) const { return Vector4<T>(x - o.x, y - o.y, z - o.z, w - o.w); }
		Vector4<T> operator*(Vector4<T> o) const { return Vector4<T>(x * o.x, y * o.y, z * o.z, w * o.w); }
		Vector4<T> operator/(Vector4<T> o) const { return Vector4<T>(x / o.x, y / o.y, z / o.z, w / o.w); }

		T dot(Vector4<T> o) const { return o.x * x + o.y * y + o.z * z + o.w * w; }
		double magnitude() const { return sqrt(static_cast<double>(x * x + y * y + z * z + w * w)); }

		Vector4<T> clamp(T minX, T maxX, T minY, T maxY, T minZ, T maxZ, T minW, T maxW) const {
			return Vector4<T>(std::clamp(x, minX, maxX), std::clamp(y, minY, maxY),
				std::clamp(z, minZ, maxZ), std::clamp(w, minW, maxW));
		}

		void clampTo(T minX, T maxX, T minY, T maxY, T minZ, T maxZ, T minW, T maxW) {
			x = std::clamp(x, minX, maxX);
			y = std::clamp(y, minY, maxY);
			z = std::clamp(z, minZ, maxZ);
			w = std::clamp(w, minW, maxW);
		}

		template <typename A> Vector4<A> cast() const {
			return Vector4<A>(static_cast<A>(x), static_cast<A>(y), static_cast<A>(z), static_cast<A>(w));
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
