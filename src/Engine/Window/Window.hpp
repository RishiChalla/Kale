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

#ifdef ISLANDS_SDL
    #include "SDL/WindowOuter.hpp"
#elif defined(ISLANDS_GLFW)
	#include "GLFW/WindowOuter.hpp"
#endif

#include <list>
#include <vulkan/vulkan.hpp>
#include <Engine/Math/Vector/Vector.hpp>

namespace Islands {
	
	/**
	 * Forward declaration of the event handler class
	 */
	class EventHandler;
	
	class Window {
		
#ifdef ISLANDS_SDL
		#include "SDL/WindowInner.hpp"
#elif defined(ISLANDS_GLFW)
		#include "GLFW/WindowInner.hpp"
#endif
		
	private:
	
		/**
		 * A linked list of all event listeners for this window
		 */
		std::list<EventHandler*> eventHandlers;

		/**
		 * The vulkan instance used for this window
		 */
		vk::Instance vulkanInstance;

		/**
		 * Creates the vulkan instance for this window
		 */
		void createVulkanInstance();

		/**
		 * Initializes vulkan for use with both windowing APIs
		 */
		void initVulkan();
		
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
		
		friend class Application;
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
		 * Gets the window size
		 */
		Vector2ui getSize();
		
		/**
		 * Gets the window size
		 */
		Vector2f getSizeF();
		
		/**
		 * Starts listening to events, override the functions provided in EventHandler to handle the events
		 */
		void registerEvents(EventHandler* handler);
		
		/**
		 * Stops listening to events
		 */
		void removeEvents(EventHandler* handler);
	};
}
