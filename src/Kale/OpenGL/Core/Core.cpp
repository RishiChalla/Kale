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

#ifdef KALE_OPENGL

#include "Core.hpp"

#include <Kale/OpenGL/Utils/Utils.hpp>

#include <string>
#include <sstream>

#include <glad/glad.h>

using namespace Kale;
using namespace Kale::OpenGL;

/**
 * Called when the event is fired
 */
void Core::ResizeHandler::onWindowResize(Vector2ui oldSize, Vector2ui newSize) {
	Vector2ui size = mainApp->getWindow().getFramebufferSize();
	glViewport(0, 0, size.x, size.y);
}

/**
 * Sets up the core renderer
 */
void Core::setupCore() noexcept {
	try {
		mainApp->getWindow().setupGlad();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		Vector2ui size = mainApp->getWindow().getFramebufferSize();
		glViewport(0, 0, size.x, size.y);
		resizeHandler = new ResizeHandler();
		mainApp->getWindow().registerEvents(dynamic_cast<EventHandler*>(resizeHandler));

#ifdef KALE_DEBUG

#ifdef KALE_OSX

		console.warn("OpenGL Debug Callback not available on OSX. Errors, warnings, and undefined behavior will not be reported.");

#else

		glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
            const void *userParam) -> void {

			std::stringstream outMessage;
			
			outMessage << "(" << enumStringValueMap.at(source) << ") Sent the following message -\n(Type: " <<
				enumStringValueMap.at(type) << ")\n";
			
			// String isn't null terminated
			if (length < 0) outMessage.write(message, length);
			else outMessage << message;
			
			switch (severity) {
#ifdef KALE_VERBOSE
				case GL_DEBUG_SEVERITY_NOTIFICATION:
					console.log(outMessage.str());
					break;
				case GL_DEBUG_SEVERITY_LOW:
					console.info(outMessage.str());
					break;
#endif
				case GL_DEBUG_SEVERITY_MEDIUM:
					console.warn(outMessage.str());
					break;
				case GL_DEBUG_SEVERITY_HIGH:
					console.error(outMessage.str());
					break;
				default:
					return;
			}
			
		}, nullptr);

		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

#endif

#endif

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
