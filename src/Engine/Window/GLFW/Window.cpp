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

#ifdef ISLANDS_GLFW

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Engine/Application/Application.hpp>
#include "Window.hpp"

using namespace Islands;

/**
 * Initializes SDL
 */
Window::Window() {
	
}

/**
 * Frees resources of the window
 */
Window::~Window() {
    
}

/**
 * Creates a new window. Some Operating systems (IOS/Android/TvOS) only support the creation
 * of a single window
 * @param title The title of the window
 */
void Window::create(const char* title) {
    
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
