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

#pragma once

#include <Kale/Core/Window/Window.hpp>
#include <Kale/Core/Logger/Logger.hpp>
#include <Kale/Core/Scene/Scene.hpp>

#include <string>
#include <memory>
#include <thread>
#include <list>
#include <vector>
#include <mutex>
#include <condition_variable>

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
		 * Used for thread synchronization
		 */
		std::mutex threadSyncMutex;

		/**
		 * Used for thread synchronization
		 */
		std::condition_variable threadSyncCondVar;

		/**
		 * Used for thread synchronization
		 */
		std::condition_variable renderSyncCondVar;

		/**
		 * Used for thread synchronization
		 */
		size_t numThreadsUpdated;

		/**
		 * Used for thread synchronization
		 */
		bool updatingFinished;

		/**
		 * Used for thread synchronization
		 */
		bool renderingFinished;

		/**
		 * The time taken to update and render each frame, set at the end of every render
		 */
		float deltaTime;

		/**
		 * Handles updating the application in a separate thread
		 * @param threadNum the index of this thread, ranged 0 - numUpdateThreads
		 */
		void update(size_t threadNum) noexcept;

		/**
		 * A pointer to the current scene to render
		 */
		std::shared_ptr<Scene> presentedScene;

		/**
		 * A pointer to the scene to be presented
		 */
		std::shared_ptr<Scene> sceneToPresent;
	
	protected:

		/**
		 * The main window used for rendering for this application instance
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
		void run() noexcept;

		/**
		 * Creates a new application instance
		 * @param applicationName The name of your application
		 */
		explicit Application(const char* applicationName) noexcept;

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
		Window& getWindow() noexcept;

		/**
		 * Gets the currently presented scene
		 * @returns The currently presented scene pointer
		 */
		[[nodiscard]] std::shared_ptr<Scene> getPresentedScene() noexcept;

		/**
		 * Gets the window
		 * @returns a pointer to the window
		 */
		[[nodiscard]] const Window& getWindow() const noexcept;

		/**
		 * Gets the currently presented scene
		 * @returns The currently presented scene pointer
		 */
		[[nodiscard]] std::shared_ptr<const Scene> getPresentedScene() const noexcept;

		/**
		 * Presents a given scene
		 * @param scene The scene to present
		 */
		void presentScene(const std::shared_ptr<Scene>& scene);

		/**
		 * Gets the number of threads currently being used to update
		 * @returns The number of threads used for updating
		 */
		[[nodiscard]] size_t getNumUpdateThreads() const noexcept;
	};

	/**
	 * The main application instance
	 */
	inline Application* mainApp = nullptr;
}

/**
 * Create your inherited application here and heap allocate it.
 * Do not worry about its destruction, the engine will take care of it.
 */
extern Kale::Application* createApplication();
