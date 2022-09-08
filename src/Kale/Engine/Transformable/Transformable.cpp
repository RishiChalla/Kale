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

#include "Transformable.hpp"

using namespace Kale;

/**
 * Thread safe method to get the transformation matrix
 * @returns The transformation matrix of this object
 */
Transform Transformable::getTransform() const {
	return transform;
}

/**
 * Thread safe method to set the transformation matrix
 * @param transform The transformation matrix to set the transform of this object to
 */
void Transformable::setTransform(const Transform& transform) {
	std::lock_guard lock(mutex);
	this->transform = transform;
}
