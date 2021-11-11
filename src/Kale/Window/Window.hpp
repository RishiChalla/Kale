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

#include <Kale/Math/Vector/Vector.hpp>

#ifdef KALE_SDL
    #include "SDL/WindowOuter.hpp"
#elif defined(KALE_GLFW)
	#include "GLFW/WindowOuter.hpp"
#endif

#include <list>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace Kale {

	/**
	 * Forward declaration of vulkan namespace
	 */
	namespace Vulkan {

		/**
		 * Forward declaration of vulkan renderer class
		 */
		class Renderer;
	}
	
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
		
	protected:
		
		/**
		 * Creates a new window. Some Operating systems (IOS/Android/TvOS) only support the creation
		 * of a single window
		 * @param title The title of the window
		 */
		void create(const char* title);
		
		/**
		 * Clears the screen for rendering the next frame
		 */
		void clear();
		
		/**
		 * Renders everything placed in the frame buffer
		 */
		void render();
		
		/**
		 * Updates the window
		 */
		void update();

		/**
		 * Gets the extensions required for VKCreateInfo depending on the windowing API
		 * @returns The required extensions for the lower level windowing API
		 */
		std::vector<const char*> getInstanceExtensions() const;

		/**
		 * Creates a vuklan window surface given the instance and the surface references
		 * @param instance The instance reference
		 * @param surface The surface reference
		 * @throws If the surface creation failed
		 */
		void createWindowSurface(const vk::UniqueInstance& instance, vk::UniqueSurfaceKHR& surface) const;
		
		friend class Application;
		friend class Vulkan::Renderer;
		friend class Scene;
		
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
