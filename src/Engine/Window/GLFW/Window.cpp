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
#include <Engine/Window/Window.hpp>
#include <Engine/Events/Events.hpp>

using namespace Islands;

/**
 * Initializes GLFW
 */
Window::Window() {
	glfwInit();
}

/**
 * Frees resources of the window
 */
Window::~Window() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Key eKey;
	
	switch (key) {
		case GLFW_KEY_GRAVE_ACCENT: eKey = Key::Backtick; break;
		case GLFW_KEY_1: eKey = Key::One; break;
		case GLFW_KEY_2: eKey = Key::Two; break;
		case GLFW_KEY_3: eKey = Key::Three; break;
		case GLFW_KEY_4: eKey = Key::Four; break;
		case GLFW_KEY_5: eKey = Key::Five; break;
		case GLFW_KEY_6: eKey = Key::Six; break;
		case GLFW_KEY_7: eKey = Key::Seven; break;
		case GLFW_KEY_8: eKey = Key::Eight; break;
		case GLFW_KEY_9: eKey = Key::Nine; break;
		case GLFW_KEY_0: eKey = Key::Zero; break;
		case GLFW_KEY_MINUS: eKey = Key::Minus; break;
		case GLFW_KEY_EQUAL: eKey = Key::Equals; break;
		case GLFW_KEY_BACKSPACE: case GLFW_KEY_DELETE: eKey = Key::Backspace; break;
		case GLFW_KEY_TAB: eKey = Key::Tab; break;
		case GLFW_KEY_LEFT_BRACKET: eKey = Key::LeftBracket; break;
		case GLFW_KEY_RIGHT_BRACKET: eKey = Key::RightBracket; break;
		case GLFW_KEY_BACKSLASH: eKey = Key::Backslash; break;
		case GLFW_KEY_ENTER: eKey = Key::Enter; break;
		case GLFW_KEY_LEFT_SHIFT: eKey = Key::LeftShift; break;
		case GLFW_KEY_COMMA: eKey = Key::Comma; break;
		case GLFW_KEY_PERIOD: eKey = Key::Period; break;
		case GLFW_KEY_SLASH: eKey = Key::Slash; break;
		case GLFW_KEY_RIGHT_SHIFT: eKey = Key::RightShift; break;
		case GLFW_KEY_LEFT_CONTROL: case GLFW_KEY_RIGHT_CONTROL: eKey = Key::Control; break;
		case GLFW_KEY_LEFT_ALT: case GLFW_KEY_RIGHT_ALT: eKey = Key::Alt; break;
		case GLFW_KEY_SPACE: eKey = Key::Space; break;
		case GLFW_KEY_UP: eKey = Key::Up; break;
		case GLFW_KEY_DOWN: eKey = Key::Down; break;
		case GLFW_KEY_LEFT: eKey = Key::Left; break;
		case GLFW_KEY_RIGHT: eKey = Key::Right; break;
		case GLFW_KEY_Q: eKey = Key::Q; break;
		case GLFW_KEY_W: eKey = Key::W; break;
		case GLFW_KEY_E: eKey = Key::E; break;
		case GLFW_KEY_R: eKey = Key::R; break;
		case GLFW_KEY_T: eKey = Key::T; break;
		case GLFW_KEY_Y: eKey = Key::Y; break;
		case GLFW_KEY_U: eKey = Key::U; break;
		case GLFW_KEY_I: eKey = Key::I; break;
		case GLFW_KEY_O: eKey = Key::O; break;
		case GLFW_KEY_P: eKey = Key::P; break;
		case GLFW_KEY_A: eKey = Key::A; break;
		case GLFW_KEY_S: eKey = Key::S; break;
		case GLFW_KEY_D: eKey = Key::D; break;
		case GLFW_KEY_F: eKey = Key::F; break;
		case GLFW_KEY_G: eKey = Key::G; break;
		case GLFW_KEY_H: eKey = Key::H; break;
		case GLFW_KEY_J: eKey = Key::J; break;
		case GLFW_KEY_K: eKey = Key::K; break;
		case GLFW_KEY_L: eKey = Key::L; break;
		case GLFW_KEY_Z: eKey = Key::Z; break;
		case GLFW_KEY_X: eKey = Key::X; break;
		case GLFW_KEY_C: eKey = Key::C; break;
		case GLFW_KEY_V: eKey = Key::V; break;
		case GLFW_KEY_B: eKey = Key::B; break;
		case GLFW_KEY_N: eKey = Key::N; break;
		case GLFW_KEY_M: eKey = Key::M; break;
		default: return;
	}
	
	std::list<EventHandler*> handlers = *static_cast<std::list<EventHandler*>*>(glfwGetWindowUserPointer(window));
	
	if (action == GLFW_PRESS) for (auto handler : handlers) handler->onKeyPress(eKey);
	if (action == GLFW_RELEASE) for (auto handler : handlers) handler->onKeyRelease(eKey);
}

static void cursorCallback(GLFWwindow* window, double x, double y) {
	std::list<EventHandler*> handlers = *static_cast<std::list<EventHandler*>*>(glfwGetWindowUserPointer(window));
	Vector2f pos = Vector2d(x, y).cast<float>();
	for (auto handler : handlers) handler->onMouseMove(pos);
}

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	std::list<EventHandler*> handlers = *static_cast<std::list<EventHandler*>*>(glfwGetWindowUserPointer(window));
	
	switch (action) {
		case GLFW_PRESS:
			switch (button) {
				case GLFW_MOUSE_BUTTON_LEFT: for (auto handler : handlers) handler->onLeftClick(); break;
				case GLFW_MOUSE_BUTTON_MIDDLE: for (auto handler : handlers) handler->onMiddleClick(); break;
				case GLFW_MOUSE_BUTTON_RIGHT: for (auto handler : handlers) handler->onRightClick(); break;
				default: return;
			}
			break;
		case GLFW_RELEASE:
			switch (button) {
				case GLFW_MOUSE_BUTTON_LEFT: for (auto handler : handlers) handler->onLeftClickRelease(); break;
				case GLFW_MOUSE_BUTTON_MIDDLE: for (auto handler : handlers) handler->onMiddleClickRelease(); break;
				case GLFW_MOUSE_BUTTON_RIGHT: for (auto handler : handlers) handler->onRightClickRelease(); break;
				default: return;
			}
			break;
		default: return;
	}
}

static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	std::list<EventHandler*> handlers = *static_cast<std::list<EventHandler*>*>(glfwGetWindowUserPointer(window));
	for (auto handler : handlers) handler->onMouseScroll(yoffset);
}

static Vector2ui oldWinSize(800, 600);

static void resizeCallback(GLFWwindow* window, int width, int height) {
	std::list<EventHandler*> handlers = *static_cast<std::list<EventHandler*>*>(glfwGetWindowUserPointer(window));
	Vector2ui size = Vector2i(width, height).cast<unsigned int>();
	for (auto handler : handlers) handler->onWindowResize(oldWinSize, size);
	oldWinSize = size;
}

static void focusCallback(GLFWwindow* window, int focused) {
	std::list<EventHandler*> handlers = *static_cast<std::list<EventHandler*>*>(glfwGetWindowUserPointer(window));
	if (focused) for (auto handler : handlers) handler->onWindowGainedFocus();
	else for (auto handler : handlers) handler->onWindowLostFocus();
}

/**
 * Creates a new window. Some Operating systems (IOS/Android/TvOS) only support the creation
 * of a single window
 * @param title The title of the window
 */
void Window::create(const char* title) {
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(800, 600, title, nullptr, nullptr);
	
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	
	info("Vulkan Extensions Supported:");
	info(extensionCount);
	
	glfwSetWindowUserPointer(window, &eventHandlers);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetFramebufferSizeCallback(window, resizeCallback);
	glfwSetWindowFocusCallback(window, focusCallback);
//	glfwSetJoystickCallback(<#GLFWjoystickfun callback#>)
}

/**
 * Thread safe method to check whether or not the window is currently open
 * @returns Whether or not the window is open
 */
bool Window::isOpen() const {
    return !glfwWindowShouldClose(window);
}

/**
 * Locks the cursor to allow for better gameplay
 */
void Window::lockCursor() {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

/**
 * Unlocks the cursor for menus/etc
 */
void Window::unlockCursor() {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

/**
 * Gets the window size
 */
Vector2ui Window::getSize() {
	return oldWinSize;
}

/**
 * Gets the window size
 */
Vector2f Window::getSizeF() {
	return oldWinSize.cast<float>();
}

/**
 * Updates the window
 */
void Window::update() {
	glfwPollEvents();
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
