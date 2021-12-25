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

#ifdef KALE_OPENGL

#include "Core.hpp"

#include <string>

#include <glad/glad.h>

using namespace Kale;
using namespace Kale::OpenGL;

/**
 * Called when the event is fired
 */
void Core::ResizeHandler::onWindowResize(Vector2ui oldSize, Vector2ui newSize) {
	glViewport(0, 0, newSize.x, newSize.y);
}

/**
 * Sets up the core renderer
 */
void Core::setupCore() noexcept {
	try {
		mainApp->getWindow().setupGlad();
		glViewport(0, 0, mainApp->getWindow().getSize().x, mainApp->getWindow().getSize().y);
		resizeHandler = new ResizeHandler();
		mainApp->getWindow().registerEvents(dynamic_cast<EventHandler*>(resizeHandler));
	}
	catch (const std::exception& e) {
		console.error(std::string("Unable to setup OpenGL/Glad - ") + e.what());
		exit(0);
	}
}

/**
 * Clears the screen with a given color
 */
void Core::clearScreen(const Vector4f& color) noexcept {
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

/**
 * Swaps the front and back buffers to display the render changes
 */
void Core::swapBuffers() noexcept {
	mainApp->getWindow().swapBuffers();
}

/**
 * Cleans up the core renderer
 */
void Core::cleanupCore() noexcept {
	mainApp->getWindow().removeEvents(dynamic_cast<EventHandler*>(resizeHandler));
	delete resizeHandler;
}

#endif
