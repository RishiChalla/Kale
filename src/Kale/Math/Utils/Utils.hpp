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

#include <limits>

namespace Kale {

	/**
	 * Converts an angle in degrees to radians
	 * @param deg The angle in degrees
	 * @returns The angle in radians
	 */
	float degToRad(float deg);

	/**
	 * Converts an angle in radians to degrees
	 * @param rad The angle in radians
	 * @returns The angle in degrees
	 */
	float radToDeg(float rad);

	/**
	 * Checks if a number is extremely close to 0 to account for floating point error
	 * @param num The number to check for
	 * @param epsilon The epsilon (distance threshold) value to use. Default is std::numeric_limits<T>::epsilon()
	 * @returns Whether or not the number is extremely close to 0
	 */
	template <typename T> bool isFloating0(T num, T epsilon = std::numeric_limits<T>::epsilon()) {
		return num <= epsilon && num >= -epsilon;
	}

	/**
	 * Checks if two numbers are roughly equivalent accounting for floating point error
	 * @param num1 The first number to compare
	 * @param num2 The second number to compare
	 * @param epsilon The epsilon (distance threshold) value to use. Default is std::numeric_limits<T>::epsilon()
	 * @returns Whether or not the two numbers are roughly equivalent
	 */
	template <typename T> bool isFloatingEqual(T num1, T num2, T epsilon = std::numeric_limits<T>::epsilon()) {
		T tmp = num1 - num2;
		return tmp <= epsilon && tmp >= -epsilon;
	}

	/**
	 * Returns the sign of the number, -1 for negatives, 0 for 0, 1 for positives
	 * @param val The number to return the sign of
	 * @return The sign of the number
	 */
	template <typename T> int sign(T val) {
		return (T(0) < val) - (val < T(0));
	}

}
