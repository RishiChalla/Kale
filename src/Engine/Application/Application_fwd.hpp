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

#include <SFML/Graphics.hpp>
#include <memory>

#include <Engine/Logger/Logger.hpp>
#include <Engine/Scene/Scene.hpp>
#include <Engine/Events/Events.hpp>
#include <Engine/Math/Vector/Vector.hpp>

/**
 * The entry point function/main function of the program
 */
int main();

namespace Islands {

	/**
	 * Game defined enum of all the required assets
	 */
	enum class Font;
	
	/**
	 * Game defined enum of all the required assets
	 */
	enum class Texture;
	
	/**
	 * Game defined enum of all the required assets
	 */
	enum class Shader;

	/**
	 * The main Application class
	 */
	class Application {
	private:

		/**
		 * the currently rendered scene
		 */
		std::shared_ptr<Scene> currentScene;

		/**
		 * Handles updating the application in a separate thread
		 */
		void update();
	
	protected:

		/**
		 * The main sfml window used for rendering for this application instance
		 */
		sf::RenderWindow window;

		/**
		 * Called when the scene is changed from one to another
		 * @param fromScene The scene changed from
		 * @param toScene The scene changed to
		 */
		virtual void onSceneChange(std::shared_ptr<Scene> fromScene, std::shared_ptr<Scene> toScene) {}

		/**
		 * Called on every update frame, must be thread safe.
		 * Do not do anything heavy here, it will cause lag
		 */
		virtual void onUpdate() {}

		/**
		 * Called when the application begins, just before the window is run.
		 */
		virtual void onBegin() {}

		/**
		 * Called just before the application is ended
		 */
		virtual void onEnd() {}

		/**
		 * Called when the event is fired
		 */
		virtual void onWindowResize(const Vector2ui& newSize);

		/**
		 * Called when the event is fired
		 */
		virtual void onWindowLostFocus();

		/**
		 * Called when the event is fired
		 */
		virtual void onWindowGainedFocus();

		/**
		 * Called when the event is fired
		 */
		virtual void onControllerConnect(unsigned int controller);

		/**
		 * Called when the event is fired
		 */
		virtual void onControllerDisconnect(unsigned int controller);

		/**
		 * Called when the event is fired
		 */
		virtual void onControllerButtonPress(unsigned int controller, unsigned int button);

		/**
		 * Called when the event is fired
		 */
		virtual void onControllerButtonRelease(unsigned int controller, unsigned int button);

		/**
		 * Called when the event is fired
		 */
		virtual void onControllerHandle(unsigned int controller, Axis handle, float position);

		/**
		 * Called when the event is fired
		 */
		virtual void onKeyPress(Key key);

		/**
		 * Called when the event is fired
		 */
		virtual void onKeyRelease(Key key);

		/**
		 * Called when the event is fired
		 */
		virtual void onMouseMove(const Vector2ui& pos);

		/**
		 * Called when the event is fired
		 */
		virtual void onMouseScroll(int scroll);

		/**
		 * Called when the event is fired
		 */
		virtual void onLeftClick();

		/**
		 * Called when the event is fired
		 */
		virtual void onMiddleClick();

		/**
		 * Called when the event is fired
		 */
		virtual void onRightClick();

		/**
		 * Called when the event is fired
		 */
		virtual void onLeftClickRelease();

		/**
		 * Called when the event is fired
		 */
		virtual void onMiddleClickRelease();

		/**
		 * Called when the event is fired
		 */
		virtual void onRightClickRelease();

		/**
		 * Called when the event is fired
		 */
		virtual void onTouchBegin(unsigned int touch, const Vector2ui& pos);

		/**
		 * Called when the event is fired
		 */
		virtual void onTouchMove(unsigned int touch, const Vector2ui& pos);

		/**
		 * Called when the event is fired
		 */
		virtual void onTouchEnd(unsigned int touch);

		/**
		 * Creates a new application instance
		 * @param fontsList The fonts to load
		 * @param texturesList The textures to load
		 * @param shadersList The shaders to load
		 */
		template <size_t numFonts, size_t numTextures, size_t numShaders>
		Application(std::array<std::tuple<Font, const char*>, numFonts> fontsList,
			std::array<std::tuple<Texture, const char*>, numTextures> texturesList,
			std::array<std::tuple<Shader, const char*, const char*>, numShaders> shadersList);

		/**
		 * Frees resources and deletes the application
		 */
		~Application();

		friend int ::main();
		friend class Events;

	public:
		/**
		 * The main logger for this application instance
		 */
		Logger log;

		// App class doesn't support copying/moving
		Application(const Application& other) = delete;
		Application(Application&& other) = delete;
		void operator=(const Application& other) = delete;
		void operator=(Application&& other) = delete;

		/**
		 * Runs the application
		 */
		void run();

		/**
		 * Starts displaying and updating a scene
		 * @param scene The scene to display
		 */
		void displayScene(std::shared_ptr<Scene> scene);
	};

	/**
	 * The main application instance
	 */
	extern Application* mainApp;
}

/**
 * Prints output to the console/log file
 */
#define print(x) Islands::mainApp->log.log(__LINE__, FILE_BASENAME, x);

/**
 * Prints info to the console/log file
 */
#define info(x) Islands::mainApp->log.info(__LINE__, FILE_BASENAME, x);

/**
 * Prints a warning to the console/log file
 */
#define warn(x) Islands::mainApp->log.warn(__LINE__, FILE_BASENAME, x);

/**
 * Prints an error to the console/log file
 */
#define error(x) Islands::mainApp->log.error(__LINE__, FILE_BASENAME, x);