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

#include <Kale/Window/Window.hpp>
#include <Kale/Logger/Logger.hpp>
#include <Kale/Scene/Scene.hpp>

#include <string>
#include <memory>
#include <thread>
#include <list>

/**
 * The entry point function/main function of the program
 */
int main();

namespace Kale {

	/**
	 * The main Application class
	 */
	class Application {

	private:
		
		/**
		 * A list of the update threads
		 */
		std::list<std::thread> updateThreads;

		/**
		 * A list of the rendering threads
		 */
		std::list<std::thread> renderThreads;

		/**
		 * Handles updating the application in a separate thread
		 * @param threadNum the index of this thread, ranged 0 - numUpdateThreads
		 */
		void update(size_t threadNum);

		/**
		 * Handles rendering the application in a separate thread
		 * @param threadNum the index of this thread, ranged 0 - numRenderThreads
		 */
		void render(size_t threadNum);

		/**
		 * A pointer to the current scene to render
		 */
		std::shared_ptr<Scene> presentedScene;
	
	protected:

		/**
		 * The main sfml window used for rendering for this application instance
		 */
		Window window;

		/**
		 * Called when the application begins, just before the window is run.
		 */
		virtual void onBegin() {}

		/**
		 * Called just before the application is ended
		 */
		virtual void onEnd() {}

		/**
		 * Runs the application
		 */
		void run();

		/**
		 * Creates a new application instance
		 * @param applicationName The name of your application
		 */
		Application(const char* applicationName);

		/**
		 * Frees resources and deletes the application
		 */
		~Application();

		friend int ::main();

	public:

		/**
		 * The name of the application the engine is being used for
		 */
		const std::string applicationName;

		// App class doesn't support copying/moving
		Application(const Application& other) = delete;
		Application(Application&& other) = delete;
		void operator=(const Application& other) = delete;
		void operator=(Application&& other) = delete;
		
		/**
		 * Gets the window
		 * @returns a pointer to the window
		 */
		Window& getWindow();

		/**
		 * Gets the currently presented scene
		 * @returns The currently presented scene pointer
		 */
		std::shared_ptr<Scene> getPresentedScene();

		/**
		 * Gets the window
		 * @returns a pointer to the window
		 */
		const Window& getWindow() const;

		/**
		 * Gets the currently presented scene
		 * @returns The currently presented scene pointer
		 */
		const std::shared_ptr<Scene> getPresentedScene() const;

		/**
		 * Presents a given scene
		 * @param scene The scene to present
		 */
		void presentScene(std::shared_ptr<Scene> scene);

		/**
		 * Gets the number of threads currently being used to render
		 * @returns The number of threads used for rendering
		 */
		size_t getNumRenderThreads() const;

		/**
		 * Gets the number of threads currently being used to update
		 * @returns The number of threads used for updating
		 */
		size_t getNumUpdateThreads() const;
	};

	/**
	 * The main application instance
	 */
	extern Application* mainApp;
}

/**
 * Create your inherited application here and heap allocate it.
 * Do not worry about its destruction, the engine will take care of it.
 */
extern Kale::Application* createApplication();