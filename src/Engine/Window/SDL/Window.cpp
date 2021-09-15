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

#ifdef ISLANDS_SDL

#include <SDL.h>

#include "Window.hpp"

using namespace Islands;

/**
 * Initializes SDL
 */
Window::Window() {
	SDL_Init(SDL_INIT_EVERYTHING);
}

/**
 * Frees resources of the window
 */
Window::~Window() {
    SDL_DestroyWindow(window);
	SDL_Quit();
}

/**
 * Creates a new window. Some Operating systems (IOS/Android/TvOS) only support the creation
 * of a single window
 * @param title The title of the window
 */
void Window::create(const char* title) {
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	open = true;
}

/**
 * Thread safe method to check whether or not the window is currently open
 * @returns Whether or not the window is open
 */
bool Window::isOpen() const {
    return open;
}

/**
 * Clears the screen for rendering the next frame
 */
void Window::clear() {
	
}

/**
 * Renders everything placed in the frame buffer
 */
void Window::render() {
    
}

#endif
