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

#include <Kale/Math/Transform/Transform.hpp>

namespace Kale {

	/**
	 * Represents a context which applies a transformation matrix to all skia drawing commands performed for the duration
	 * of the class life time
	 */
	class DrawingContext {
	public:

		/**
		 * Creates a drawing context given a transformation matrix/camera
		 */
		DrawingContext(Camera camera);

		/**
		 * Destroys the drawing context and undoes the transformation matrix/camera perspective
		 */
		~DrawingContext();

		// Drawing contexts dont support moving/copying
		DrawingContext(const DrawingContext&) = delete;
		DrawingContext(DrawingContext&&) = delete;
		void operator=(const DrawingContext&) = delete;
		void operator=(DrawingContext&&) = delete;
	};

}
