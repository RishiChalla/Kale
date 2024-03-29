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

#ifdef KALE_GLFW

#include <Kale/Core/Application/Application.hpp>
#include <Kale/Core/Window/Window.hpp>
#include <Kale/Core/Events/Events.hpp>

#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

using namespace Kale;

static std::list<EventHandler*>* handlers = nullptr;
static Vector2ui oldWinSize(800, 450);

/**
 * A struct for internal usage
 */
struct _WinGamePad {
	unsigned int id;
	bool LeftJoystick, RightJoystick,
		DPadUp, DPadRight, DPadDown, DPadLeft,
		LeftMenu, RightMenu,
		A, B, X, Y,
		R1, L1;
	Vector2f lJoystick, rJoystick;
	float lHandle, rHandle;
	
	_WinGamePad(unsigned int id) : id(id) {}
};

static std::list<_WinGamePad> gamePads;

/**
 * Initializes GLFW
 */
Window::Window() {
	glfwInit();

#ifdef KALE_OPENGL

	// Mac drivers of OpenGL are stale and only support up to 4.1
#ifdef KALE_OSX
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#else
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
#endif

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef KALE_DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

#endif

#ifdef KALE_VULKAN
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif

	glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
}

/**
 * Frees resources of the window
 */
Window::~Window() {
	glfwDestroyWindow(window);
	glfwTerminate();
	handlers = nullptr;
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (handlers == nullptr) return;
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
	
	if (action == GLFW_PRESS) for (auto handler : *handlers) handler->onKeyPress(eKey);
	if (action == GLFW_RELEASE) for (auto handler : *handlers) handler->onKeyRelease(eKey);
}

static void cursorCallback(GLFWwindow* window, double x, double y) {
	if (handlers == nullptr) return;
	Vector2f pos = Vector2d(x, y).cast<float>();
	for (auto handler : *handlers) handler->onMouseMove(pos);
}

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (handlers == nullptr) return;
	switch (action) {
		case GLFW_PRESS:
			switch (button) {
				case GLFW_MOUSE_BUTTON_LEFT: for (auto handler : *handlers) handler->onLeftClick(); return;
				case GLFW_MOUSE_BUTTON_MIDDLE: for (auto handler : *handlers) handler->onMiddleClick(); return;
				case GLFW_MOUSE_BUTTON_RIGHT: for (auto handler : *handlers) handler->onRightClick(); return;
				default: return;
			}
		case GLFW_RELEASE:
			switch (button) {
				case GLFW_MOUSE_BUTTON_LEFT: for (auto handler : *handlers) handler->onLeftClickRelease(); return;
				case GLFW_MOUSE_BUTTON_MIDDLE: for (auto handler : *handlers) handler->onMiddleClickRelease(); return;
				case GLFW_MOUSE_BUTTON_RIGHT: for (auto handler : *handlers) handler->onRightClickRelease(); return;
				default: return;
			}
		default: return;
	}
}

static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	if (handlers == nullptr) return;
	for (auto handler : *handlers) handler->onMouseScroll(static_cast<float>(yoffset));
}

static void resizeCallback(GLFWwindow* window, int width, int height) {
	Vector2ui tmp_oldWinSize(oldWinSize);
	Vector2ui size = Vector2i(width, height).cast<unsigned int>();
	// Set the oldWinSize static variable here to avoid client calling getSize() and retrieving incorrect size
	oldWinSize = size;

	if (handlers == nullptr) return;
	for (auto handler : *handlers) handler->onWindowResize(tmp_oldWinSize, size);
}

static void focusCallback(GLFWwindow* window, int focused) {
	if (handlers == nullptr) return;
	if (focused) for (auto handler : *handlers) handler->onWindowGainedFocus();
	else for (auto handler : *handlers) handler->onWindowLostFocus();
}

static void joystickCallback(int jid, int action) {
	if (handlers == nullptr) return;
	switch (action) {
		case GLFW_CONNECTED:
			for (auto handler : *handlers) handler->onControllerConnect(jid);
			gamePads.emplace_back(jid);
			return;
		case GLFW_DISCONNECTED:
			for (auto handler : *handlers) handler->onControllerDisconnect(jid);
			gamePads.remove_if([=] (_WinGamePad& gamepad) {
				return gamepad.id == jid;
			});
			return;
		default:
			return;
	}
}

/**
 * Creates a new window. Some Operating systems (IOS/Android/TvOS) only support the creation
 * of a single window
 * @param title The title of the window
 */
void Window::create(const char* title) {
	this->title = title;
	window = glfwCreateWindow(oldWinSize.x, oldWinSize.y, title, nullptr, nullptr);

	if (!window) {
		glfwTerminate();
		console.error("Unable to create GLFW window");
		exit(0);
	}

	// Set the default window icon to the Kale icon
	setIcon(mainApp->getAssetFolderPath() + "textures/kale.png");

#ifdef KALE_OPENGL
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
#endif

	// Correct the oldWinSize static variable to the window size rather than framebuffer size
	Vector2i winSize;
	glfwGetWindowSize(window, &winSize.x, &winSize.y);
	oldWinSize = winSize.cast<unsigned int>();
	
	handlers = &eventHandlers;
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetWindowSizeCallback(window, resizeCallback);
	glfwSetWindowFocusCallback(window, focusCallback);
	glfwSetJoystickCallback(joystickCallback);
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
Vector2ui Window::getSize() const {
	return oldWinSize;
}

/**
 * Gets the window size
 */
Vector2f Window::getSizeF() const {
	return oldWinSize.cast<float>();
}

/**
 * Gets the frame buffer size for canvas creation
 */
Vector2ui Window::getFramebufferSize() const {
	Vector2i size;
	glfwGetFramebufferSize(window, &size.x, &size.y);
	return size.cast<unsigned int>();
}

/**
 * Updates the window
 */
void Window::update() {
	
	// Poll Events
	glfwPollEvents();
	
	// Gamepad input
	for (_WinGamePad& gamepad : gamePads) {
		GLFWgamepadstate state;
		if (!glfwGetGamepadState(gamepad.id, &state)) {
			gamePads.remove_if([&] (_WinGamePad& gamepad2) {
				return gamepad2.id == gamepad.id;
			});
			continue;
		}
		
		// Gamepad Buttons
		if (state.buttons[GLFW_GAMEPAD_BUTTON_A] == GLFW_PRESS && !gamepad.A) {
			for (auto handler : eventHandlers) handler->onControllerButtonPress(gamepad.id, ControllerButton::A);
			gamepad.A = true;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_B] == GLFW_PRESS && !gamepad.B) {
			for (auto handler : eventHandlers) handler->onControllerButtonPress(gamepad.id, ControllerButton::B);
			gamepad.B = true;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_X] == GLFW_PRESS && !gamepad.X) {
			for (auto handler : eventHandlers) handler->onControllerButtonPress(gamepad.id, ControllerButton::X);
			gamepad.X = true;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_Y] == GLFW_PRESS && !gamepad.Y) {
			for (auto handler : eventHandlers) handler->onControllerButtonPress(gamepad.id, ControllerButton::Y);
			gamepad.Y = true;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP] == GLFW_PRESS && !gamepad.DPadUp) {
			for (auto handler : eventHandlers) handler->onControllerButtonPress(gamepad.id, ControllerButton::DPadUp);
			gamepad.DPadUp = true;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] == GLFW_PRESS && !gamepad.DPadRight) {
			for (auto handler : eventHandlers) handler->onControllerButtonPress(gamepad.id, ControllerButton::DPadRight);
			gamepad.DPadRight = true;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] == GLFW_PRESS && !gamepad.DPadDown) {
			for (auto handler : eventHandlers) handler->onControllerButtonPress(gamepad.id, ControllerButton::DPadDown);
			gamepad.DPadDown = true;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] == GLFW_PRESS && !gamepad.DPadLeft) {
			for (auto handler : eventHandlers) handler->onControllerButtonPress(gamepad.id, ControllerButton::DPadLeft);
			gamepad.DPadLeft = true;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] == GLFW_PRESS && !gamepad.R1) {
			for (auto handler : eventHandlers) handler->onControllerButtonPress(gamepad.id, ControllerButton::R1);
			gamepad.R1 = true;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] == GLFW_PRESS && !gamepad.L1) {
			for (auto handler : eventHandlers) handler->onControllerButtonPress(gamepad.id, ControllerButton::L1);
			gamepad.L1 = true;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB] == GLFW_PRESS && !gamepad.RightJoystick) {
			for (auto handler : eventHandlers) handler->onControllerButtonPress(gamepad.id, ControllerButton::RightJoystick);
			gamepad.RightJoystick = true;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB] == GLFW_PRESS && !gamepad.LeftJoystick) {
			for (auto handler : eventHandlers) handler->onControllerButtonPress(gamepad.id, ControllerButton::LeftJoystick);
			gamepad.LeftJoystick = true;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_START] == GLFW_PRESS && !gamepad.RightMenu) {
			for (auto handler : eventHandlers) handler->onControllerButtonPress(gamepad.id, ControllerButton::RightMenu);
			gamepad.RightMenu = true;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_GUIDE] == GLFW_PRESS && !gamepad.LeftMenu) {
			for (auto handler : eventHandlers) handler->onControllerButtonPress(gamepad.id, ControllerButton::LeftMenu);
			gamepad.LeftMenu = true;
		}
		
		
		if (state.buttons[GLFW_GAMEPAD_BUTTON_A] == GLFW_RELEASE && gamepad.A) {
			for (auto handler : eventHandlers) handler->onControllerButtonRelease(gamepad.id, ControllerButton::A);
			gamepad.A = false;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_B] == GLFW_RELEASE && gamepad.B) {
			for (auto handler : eventHandlers) handler->onControllerButtonRelease(gamepad.id, ControllerButton::B);
			gamepad.B = false;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_X] == GLFW_RELEASE && gamepad.X) {
			for (auto handler : eventHandlers) handler->onControllerButtonRelease(gamepad.id, ControllerButton::X);
			gamepad.X = false;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_Y] == GLFW_RELEASE && gamepad.Y) {
			for (auto handler : eventHandlers) handler->onControllerButtonRelease(gamepad.id, ControllerButton::Y);
			gamepad.Y = false;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP] == GLFW_RELEASE && gamepad.DPadUp) {
			for (auto handler : eventHandlers) handler->onControllerButtonRelease(gamepad.id, ControllerButton::DPadUp);
			gamepad.DPadUp = false;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] == GLFW_RELEASE && gamepad.DPadRight) {
			for (auto handler : eventHandlers) handler->onControllerButtonRelease(gamepad.id, ControllerButton::DPadRight);
			gamepad.DPadRight = false;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] == GLFW_RELEASE && gamepad.DPadDown) {
			for (auto handler : eventHandlers) handler->onControllerButtonRelease(gamepad.id, ControllerButton::DPadDown);
			gamepad.DPadDown = false;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] == GLFW_RELEASE && gamepad.DPadLeft) {
			for (auto handler : eventHandlers) handler->onControllerButtonRelease(gamepad.id, ControllerButton::DPadLeft);
			gamepad.DPadLeft = false;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] == GLFW_RELEASE && gamepad.R1) {
			for (auto handler : eventHandlers) handler->onControllerButtonRelease(gamepad.id, ControllerButton::R1);
			gamepad.R1 = false;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] == GLFW_RELEASE && gamepad.L1) {
			for (auto handler : eventHandlers) handler->onControllerButtonRelease(gamepad.id, ControllerButton::L1);
			gamepad.L1 = false;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB] == GLFW_RELEASE && gamepad.RightJoystick) {
			for (auto handler : eventHandlers) handler->onControllerButtonRelease(gamepad.id, ControllerButton::RightJoystick);
			gamepad.RightJoystick = false;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB] == GLFW_RELEASE && gamepad.LeftJoystick) {
			for (auto handler : eventHandlers) handler->onControllerButtonRelease(gamepad.id, ControllerButton::LeftJoystick);
			gamepad.LeftJoystick = false;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_START] == GLFW_RELEASE && gamepad.RightMenu) {
			for (auto handler : eventHandlers) handler->onControllerButtonRelease(gamepad.id, ControllerButton::RightMenu);
			gamepad.RightMenu = false;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_GUIDE] == GLFW_RELEASE && gamepad.LeftMenu) {
			for (auto handler : eventHandlers) handler->onControllerButtonRelease(gamepad.id, ControllerButton::LeftMenu);
			gamepad.LeftMenu = false;
		}
		
		// Handles
		if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] != gamepad.lHandle) {
			gamepad.lHandle = state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
			for (auto handler : eventHandlers) handler->onControllerHandle(gamepad.id, ControllerAxis::Left, gamepad.lHandle);
		}
		if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] != gamepad.rHandle) {
			gamepad.lHandle = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];
			for (auto handler : eventHandlers) handler->onControllerHandle(gamepad.id, ControllerAxis::Right, gamepad.rHandle);
		}
		
		// Joysticks
		if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] != gamepad.lJoystick.x || state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] != gamepad.lJoystick.y) {
			gamepad.lJoystick.x = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
			gamepad.lJoystick.y = state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
			for (auto handler : eventHandlers) handler->onControllerJoystick(gamepad.id, ControllerAxis::Left, gamepad.lJoystick);
		}
		if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] != gamepad.rJoystick.x || state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] != gamepad.rJoystick.y) {
			gamepad.rJoystick.x = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
			gamepad.rJoystick.y = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];
			for (auto handler : eventHandlers) handler->onControllerJoystick(gamepad.id, ControllerAxis::Right, gamepad.rJoystick);
		}
	}
}

/**
 * Gets the extensions required for VKCreateInfo depending on the windowing API
 * @returns The required extensions for the lower level windowing API
 */
std::vector<const char*> Window::getInstanceExtensions() const {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	std::vector<const char*> requiredExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	return requiredExtensions;
}

#ifdef KALE_VULKAN

/**
 * Creates a vulkan window surface given the instance and the surface references
 * @param instance The instance reference
 * @param surface The surface reference
 * @throws If the surface creation failed
 */
void Window::createWindowSurface(const vk::UniqueInstance& instance, vk::UniqueSurfaceKHR& surface) const {
	VkSurfaceKHR tmpSurface;
	if (glfwCreateWindowSurface(instance.get(), window, nullptr, &tmpSurface) != VK_SUCCESS)
		throw std::runtime_error("Unable to create window surface");

	surface = vk::UniqueSurfaceKHR(tmpSurface, instance.get());
}

#endif

#ifdef KALE_OPENGL

/**
 * Sets up Glad
 * @throws If setup fails
 */
void Window::setupGlad() const {
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
		throw std::runtime_error("Unable to setup GLAD");
}

/**
 * Uses the windowing API to swap the front and back buffers
 */
void Window::swapBuffers() const noexcept {
	glfwSwapBuffers(window);
}

#endif

/**
 * Gets the window title
 */
const char* Window::getTitle() const {
	return title;
}

/**
 * Sets the window icon to the given image
 * @param filePath The window icon to set to
 */
void Window::setIcon(const std::string& filePath) {

#ifndef KALE_OSX
	GLFWimage image;
	image.pixels = stbi_load(filePath.c_str(), &image.width, &image.height, nullptr, STBI_rgb_alpha);
	glfwSetWindowIcon(window, 1, &image);
	stbi_image_free(image.pixels);
#else
	console.warn("Cannot set icon to " + filePath + " - OSX does not support glfwSetWindowIcon");
#endif

}

#endif
