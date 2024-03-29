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

#include <fstream>
#include <string>
#include <string.h>
#include <mutex>

#ifdef KALE_DEBUG

#ifdef KALE_WINDOWS
#define TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES
#endif

#include <termcolor/termcolor.hpp>
#include <iostream>

#endif

namespace Kale {

	/**
	 * The main logger class for the game
	 */
	class Logger {
	private:

		// Colors for console in Debug Mode
		#ifdef KALE_DEBUG

		/**
		 * Reset Color
		 */
		constexpr static auto rc = termcolor::reset;
		
		/**
		 * Time Color
		 */
		constexpr static auto tc = termcolor::color<87>;
		
		/**
		 * Log Color
		 */
		constexpr static auto lc = termcolor::color<249>;
		
		/**
		 * Info Color
		 */
		constexpr static auto ic = termcolor::color<116>;
		
		/**
		 * Warn Color
		 */
		constexpr static auto wc = termcolor::color<220>;
		
		/**
		 * Error Color
		 */
		constexpr static auto ec = termcolor::color<198>;
		
		/**
		 * File Color
		 */
		constexpr static auto fc = termcolor::color<85>;
		
		/**
		 * Line Color
		 */
		constexpr static auto lic = termcolor::color<86>;
		
		/**
		 * Default Color
		 */
		constexpr static auto dc = termcolor::color<241>;

		#endif

		/**
		 * The main log file for logging output to
		 */
		std::ofstream logFile;

		/**
		 * The mutex used for keeping the log file thread safe
		 */
		std::mutex mutex;

		/**
		 * Gets the time prefix for logging
		 * @returns The time prefix
		 */
		std::string getTimePrefix();
	
	protected:

		/**
		 * Loads and sets up the console
		 * @param applicationName The name of the application
		 */
		void load(const std::string& applicationName);

		friend class Application;

	public:

		/**
		 * Creates the logger object, this will not be initialized until load is called
		 */
		Logger();
		
		/**
		 * Logger does not support copying/moving
		 */
		Logger(const Logger& other) = delete;
		
		/**
		 * Logger does not support copying/moving
		 */
		Logger(Logger&& other) = delete;
		
		/**
		 * Logger does not support copying/moving
		 */
		void operator=(const Logger& other) = delete;
		
		/**
		 * Logger does not support copying/moving
		 */
		void operator=(Logger&& other) = delete;

		/**
		 * Logs output to the console
		 * @param msg The message to log
		 */
		template <typename T> void log(T msg) {
			std::string time = getTimePrefix();

			std::lock_guard<std::mutex> guard(mutex);
			#ifdef KALE_DEBUG
			std::cout << dc << "[" << tc << time << dc <<  "] " << // [HH:MM AM]
				lc << msg << "\n" << rc; // Msg
			#endif

			logFile << "[" << time << "] " << msg << std::endl;
		}

		/**
		 * Logs info to the console
		 * @param msg The message to log
		 */
		template <typename T> void info(T msg) {
			std::string time = getTimePrefix();

			std::lock_guard<std::mutex> guard(mutex);
			#ifdef KALE_DEBUG
			std::cout << dc << "[" << tc << time << dc <<  "] [" << // [HH:MM AM]
				ic << "INFO" << dc << "] " << ic << msg << "\n" << rc; // [Info] Msg
			#endif

			logFile << "[" << time << "] " << "[INFO] " << msg << std::endl;
		}

		/**
		 * Logs a warning to the console
		 * @param msg The message to log
		 */
		template <typename T> void warn(T msg) {
			std::string time = getTimePrefix();

			std::lock_guard<std::mutex> guard(mutex);
			#ifdef KALE_DEBUG
			std::cout << dc << "[" << tc << time << dc <<  "] [" << // [HH:MM AM]
				wc << "WARNING" << dc << "] " << wc << msg << "\n" << rc; // [Warning] Msg
			#endif

			logFile << "[" << time << "] " << "[WARNING] " << msg << std::endl;
		}

		/**
		 * Logs an error to the console
		 * @param msg The message to log
		 */
		template <typename T> void error(T msg) {
			std::string time = getTimePrefix();

			std::lock_guard<std::mutex> guard(mutex);
			#ifdef KALE_DEBUG
			std::cout << dc << "[" << tc << time << dc <<  "] [" << // [HH:MM AM]
				ec << "ERROR" << dc << "] " << ec << msg << "\n" << rc; // [Error] Msg
			#endif

			logFile << "[" << time << "] " << "[ERROR] " << msg << std::endl;
		}

		/**
		 * Logs output to the console
		 * @param line The line number the log is being called from
		 * @param file The name of the file the log is being called from
		 * @param msg The message to log
		 */
		template <typename T> void log(unsigned int line, const char* file, T msg) {
			std::string time = getTimePrefix();

			std::lock_guard<std::mutex> guard(mutex);
			#ifdef KALE_DEBUG
			std::cout << dc << "[" << tc << time << dc <<  "] [" << // [HH:MM AM]
				fc << file << dc << ":" << lic << line << dc << "] " << // [File:Line]
				lc << msg << "\n" << rc; // Msg
			#endif

			logFile << "[" << time << "] " << "[" << file << ":" << line << "] " << msg << std::endl;
		}

		/**
		 * Logs info to the console
		 * @param line The line number the log is being called from
		 * @param file The name of the file the log is being called from
		 * @param msg The message to log
		 */
		template <typename T> void info(unsigned int line, const char* file, T msg) {
			std::string time = getTimePrefix();

			std::lock_guard<std::mutex> guard(mutex);
			#ifdef KALE_DEBUG
			std::cout << dc << "[" << tc << time << dc <<  "] [" << // [HH:MM AM]
				fc << file << dc << ":" << lic << line << dc << "] [" << // [File:Line]
				ic << "INFO" << dc << "] " << // [Info]
				ic << msg << "\n" << rc; // Msg
			#endif

			logFile << "[" << time << "] " << "[" << file << ":" << line << "] " << "[INFO] " << msg << std::endl;
		}

		/**
		 * Logs a warning to the console
		 * @param line The line number the log is being called from
		 * @param file The name of the file the log is being called from
		 * @param msg The message to log
		 */
		template <typename T> void warn(unsigned int line, const char* file, T msg) {
			std::string time = getTimePrefix();

			std::lock_guard<std::mutex> guard(mutex);
			#ifdef KALE_DEBUG
			std::cout << dc << "[" << tc << time << dc <<  "] [" << // [HH:MM AM]
				fc << file << dc << ":" << lic << line << dc << "] [" << // [File:Line]
				wc << "WARNING" << dc << "] " << // [Warning]
				wc << msg << "\n" << rc; // Msg
			#endif

			logFile << "[" << time << "] " << "[" << file << ":" << line << "] " << "[WARNING] " << msg << std::endl;
		}

		/**
		 * Logs an error to the console
		 * @param line The line number the log is being called from
		 * @param file The name of the file the log is being called from
		 * @param msg The message to log
		 */
		template <typename T> void error(unsigned int line, const char* file, T msg) {
			std::string time = getTimePrefix();

			std::lock_guard<std::mutex> guard(mutex);
			#ifdef KALE_DEBUG
			std::cout << dc << "[" << tc << time << dc <<  "] [" << // [HH:MM AM]
				fc << file << dc << ":" << lic << line << dc << "] [" << // [File:Line]
				ec << "ERROR" << dc << "] " << // [Error]
				ec << msg << "\n" << rc; // Msg
			#endif

			logFile << "[" << time << "] " << "[" << file << ":" << line << "] " << "[ERROR] " << msg << std::endl;
		}

		/**
		 * Logs an assertion to the console
		 * @param line The line number the log is being called from
		 * @param file The name of the file the log is being called from
		 * @param assertion The message to log
		 */
		template <typename T> void assertCheck(unsigned int line, const char* file, T assertion) {
			if (assertion) return;
			error(line, file, "Assertion Failed");
		}

		/**
		 * Logs an assertion to the console
		 * @param line The line number the log is being called from
		 * @param file The name of the file the log is being called from
		 * @param assertion The assertion to check
		 * @param msg The message to log if the assertion fails
		 */
		template <typename T, typename STR> void assertCheck(unsigned int line, const char* file, T assertion, STR msg) {
			using namespace std::string_literals;
			if (assertion) return;
			error(line, file, "Assertion Failed - "s + msg);
		}
	};

	/**
	 * The main console used for printing to the output
	 */
	inline Logger console;
}

/**
 * The base filename/relative file path without the source path
 */
#ifdef KALE_WINDOWS
#define __FILENAME__ strrchr("\\" __FILE__, '\\') + 1
#else
#define __FILENAME__ strrchr("/" __FILE__, '/') + 1
#endif

/**
 * Prints output to the console/log file
 */
#define klPrint(x) Kale::console.log(__LINE__, __FILENAME__, x)

/**
 * Prints info to the console/log file
 */
#define klInfo(x) Kale::console.info(__LINE__, __FILENAME__, x)

/**
 * Prints a warning to the console/log file
 */
#define klWarn(x) Kale::console.warn(__LINE__, __FILENAME__, x)

/**
 * Prints an error to the console/log file
 */
#define klError(x) Kale::console.error(__LINE__, __FILENAME__, x)

/**
 * Prints an assertion to the console/log file if the assertion is false
 */
#define klAssert(x) Kale::console.assertCheck(__LINE__, __FILENAME__, x)

/**
 * Prints an assertion to the console/log file if the assertion is false
 */
#define klAssertMsg(x, y) Kale::console.assertCheck(__LINE__, __FILENAME__, x, y)
