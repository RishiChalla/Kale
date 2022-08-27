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

#ifdef KALE_OPENGL

#include <Kale/Core/Application/Application.hpp>
#include <Kale/Core/Events/Events.hpp>

#include <mutex>

namespace Kale::OpenGL {

	/**
	 * The core class of the OpenGL renderer which handles setting up
	 */
	class Core {
	private:

		/**
		 * Handles resizing events and resizes the opengl viewport
		 */
		class ResizeHandler : public EventHandler {
		public:

			/**
			 * Called when the event is fired
			 */
			void onWindowResize(Vector2ui oldSize, Vector2ui newSize) override;
		};

		/**
		 * Handler for resizing of windows
		 */
		inline static ResizeHandler* resizeHandler = nullptr;

	protected:

		/**
		 * Sets up the core renderer
		 */
		static void setupCore() noexcept;
		
		/**
		 * Cleans up the core renderer
		 */
		static void cleanupCore() noexcept;

		/**
		 * Clears the screen with a given color
		 */
		static void clearScreen(const Vector4f& color) noexcept;

		/**
		 * Swaps the front and back buffers to display the render changes
		 */
		static void swapBuffers() noexcept;

		friend class Kale::Application;
		friend class Kale::Scene;

	public:
	
	
	};

}

#endif
