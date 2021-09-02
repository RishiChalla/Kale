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
#include <Logger/Logger.hpp>

namespace Islands {

	/**
	 * The main Application class
	 */
	class Application {
	private:

		/**
		 * The main sfml window used for rendering for this application instance
		 */
		sf::RenderWindow window;

	public:
		/**
		 * The main logger for this application instance
		 */
		Logger log;

		/**
		 * Creates a new application instance
		 */
		Application();

		/**
		 * Frees resources and deletes the application
		 */
		~Application();

		// App class doesn't support copying/moving
		Application(const Application& other) = delete;
		Application(Application&& other) = delete;
		void operator=(const Application& other) = delete;
		void operator=(Application&& other) = delete;

		/**
		 * Runs the application
		 */
		void run();
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