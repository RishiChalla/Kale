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

#include "Window.hpp"

using namespace Kale;

/**
 * Starts listening to events, override the functions provided in EventHandler to handle the events
 */
void Window::registerEvents(EventHandler* handler) {
	eventHandlers.push_back(handler);
}

/**
 * Stops listening to events
 */
void Window::removeEvents(EventHandler* handler) {
	eventHandlers.remove(handler);
}
