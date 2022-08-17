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

#ifdef KALE_SDL
    #include "SDL/WindowOuter.hpp"
#elif defined(KALE_GLFW)
	#include "GLFW/WindowOuter.hpp"
#endif

#include <list>
#include <vector>

#include <gpu/GrDirectContext.h>
#include <core/SkSurface.h>
#include <core/SkCanvas.h>

namespace Kale {
	
	/**
	 * Forward declaration of the event handler class
	 */
	class EventHandler;
	
	class Window {
		
#ifdef KALE_SDL
		#include "SDL/WindowInner.hpp"
#elif defined(KALE_GLFW)
		#include "GLFW/WindowInner.hpp"
#endif
		
	private:

		/**
		 * A linked list of all event listeners for this window
		 */
		std::list<EventHandler*> eventHandlers;

		/**
		 * The skia context used for rendering
		 */
		GrDirectContext* skiaContext;

		/**
		 * The Skia surface connecting this window to the rendering API
		 */
		SkSurface* skiaSurface;
		
	protected:
		
		/**
		 * Creates a new window. Some Operating systems (IOS/Android/TvOS) only support the creation
		 * of a single window
		 * @param title The title of the window
		 */
		void create(const char* title);
		
		/**
		 * Updates the window
		 */
		void update();

		/**
		 * Uses the windowing API to swap the front and back buffers
		 */
		void swapBuffers() const noexcept;
		
		friend class Application;
		
	public:
		
		/**
		 * Initializes the lower level Windowing API
		 */
		Window();
		
		/**
		 * Frees resources of the window
		 */
		~Window();
		
		/**
		 * Thread safe method to check whether or not the window is currently open
		 * @returns Whether or not the window is open
		 */
		bool isOpen() const;
		
		/**
		 * Locks the cursor to allow for better gameplay
		 */
		void lockCursor();
		
		/**
		 * Unlocks the cursor for menus/etc
		 */
		void unlockCursor();

		/**
		 * Sets the window icon to the given image
		 * @param filePath The window icon to set to
		 */
		void setIcon(const char* filePath);

		/**
		 * Gets the frame buffer size for canvas creation
		 */
		Vector2ui getFramebufferSize() const;
		
		/**
		 * Gets the window size
		 * @returns The window size
		 */
		Vector2ui getSize() const;
		
		/**
		 * Gets the window size
		 * @returns The window size as a float vector
		 */
		Vector2f getSizeF() const;

		/**
		 * Gets the window title
		 * @returns The window title
		 */
		const char* getTitle() const;

		/**
		 * Recreates the skia surface (should be called on window resize)
		 */
		void recreateSkiaSurface();

		/**
		 * Gets the canvas to draw to
		 * @throws If the canvas does not currently exist
		 * @returns The canvas
		 */
		SkCanvas& getCanvas();
		
		/**
		 * Starts listening to events, override the functions provided in EventHandler to handle the events
		 * @param handler The handler to begin receiving events with
		 */
		void registerEvents(EventHandler* handler);
		
		/**
		 * Stops listening to events
		 * @param handler The handler to remove from receiving events
		 */
		void removeEvents(EventHandler* handler);
	};
}
