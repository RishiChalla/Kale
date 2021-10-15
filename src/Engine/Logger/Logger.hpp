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

#include <fstream>

#ifdef ISLANDS_DEBUG

#ifdef ISLANDS_WINDOWS
#define TERMCOLOR_USE_ANSI_ESCAPE_SEQUENCES
#endif

#include <termcolor/termcolor.hpp>
#include <iostream>
#endif



namespace Islands {

	/**
	 * The main logger class for the game
	 */
	class Logger {
	private:

		// Colors for console in Debug Mode
		#ifdef ISLANDS_DEBUG

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
		 * Gets the time prefix for logging
		 * @returns The time prefix
		 */
		std::string getTimePrefix();

	protected:
	
		/**
		 * Creates a new logger instance
		 */
		Logger();

		friend class Application;

	public:

		/**
		 * Logs output to the console
		 * @param msg The message to log
		 */
		template <typename T> void log(T msg) {
			std::string time = getTimePrefix();

			#ifdef ISLANDS_DEBUG
			std::cout << dc << "[" << tc << time << dc <<  "]" << // [HH:MM AM]
				lc << msg << "\n" << rc; // Msg
			#endif

			logFile << "[" << time << "] " << msg << "\n";
		}

		/**
		 * Logs info to the console
		 * @param msg The message to log
		 */
		template <typename T> void info(T msg) {
			std::string time = getTimePrefix();

			#ifdef ISLANDS_DEBUG
			std::cout << dc << "[" << tc << time << dc <<  "] [" << // [HH:MM AM]
				ic << "INFO" << dc << "] " << ic << msg << "\n" << rc; // [Info] Msg
			#endif

			logFile << "[" << time << "] " << "[INFO] " << msg << "\n";
		}

		/**
		 * Logs a warning to the console
		 * @param msg The message to log
		 */
		template <typename T> void warn(T msg) {
			std::string time = getTimePrefix();

			#ifdef ISLANDS_DEBUG
			std::cout << dc << "[" << tc << time << dc <<  "] [" << // [HH:MM AM]
				wc << "WARNING" << dc << "] " << wc << msg << "\n" << rc; // [Warning] Msg
			#endif

			logFile << "[" << time << "] " << "[WARNING] " << msg << "\n";
		}

		/**
		 * Logs an error to the console
		 * @param msg The message to log
		 */
		template <typename T> void error(T msg) {
			std::string time = getTimePrefix();

			#ifdef ISLANDS_DEBUG
			std::cout << dc << "[" << tc << time << dc <<  "] [" << // [HH:MM AM]
				ec << "ERROR" << dc << "] " << ec << msg << "\n" << rc; // [Error] Msg
			#endif

			logFile << "[" << time << "] " << "[ERROR] " << msg << "\n";
		}

		/**
		 * Logs output to the console
		 * @param line The line number the log is being called from
		 * @param file The name of the file the log is being called from
		 * @param msg The message to log
		 */
		template <typename T> void log(unsigned int line, const char* file, T msg) {
			std::string time = getTimePrefix();

			#ifdef ISLANDS_DEBUG
			std::cout << dc << "[" << tc << time << dc <<  "] [" << // [HH:MM AM]
				fc << file << dc << ":" << lic << line << dc << "] " << // [File:Line]
				lc << msg << "\n" << rc; // Msg
			#endif

			logFile << "[" << time << "] " << "[" << file << ":" << line << "] " << msg << "\n";
		}

		/**
		 * Logs info to the console
		 * @param line The line number the log is being called from
		 * @param file The name of the file the log is being called from
		 * @param msg The message to log
		 */
		template <typename T> void info(unsigned int line, const char* file, T msg) {
			std::string time = getTimePrefix();

			#ifdef ISLANDS_DEBUG
			std::cout << dc << "[" << tc << time << dc <<  "] [" << // [HH:MM AM]
				fc << file << dc << ":" << lic << line << dc << "] [" << // [File:Line]
				ic << "INFO" << dc << "] " << // [Info]
				ic << msg << "\n" << rc; // Msg
			#endif

			logFile << "[" << time << "] " << "[" << file << ":" << line << "] " << "[INFO] " << msg << "\n";
		}

		/**
		 * Logs a warning to the console
		 * @param line The line number the log is being called from
		 * @param file The name of the file the log is being called from
		 * @param msg The message to log
		 */
		template <typename T> void warn(unsigned int line, const char* file, T msg) {
			std::string time = getTimePrefix();

			#ifdef ISLANDS_DEBUG
			std::cout << dc << "[" << tc << time << dc <<  "] [" << // [HH:MM AM]
				fc << file << dc << ":" << lic << line << dc << "] [" << // [File:Line]
				wc << "WARNING" << dc << "] " << // [Warning]
				wc << msg << "\n" << rc; // Msg
			#endif

			logFile << "[" << time << "] " << "[" << file << ":" << line << "] " << "[WARNING] " << msg << "\n";
		}

		/**
		 * Logs an error to the console
		 * @param line The line number the log is being called from
		 * @param file The name of the file the log is being called from
		 * @param msg The message to log
		 */
		template <typename T> void error(unsigned int line, const char* file, T msg) {
			std::string time = getTimePrefix();

			#ifdef ISLANDS_DEBUG
			std::cout << dc << "[" << tc << time << dc <<  "] [" << // [HH:MM AM]
				fc << file << dc << ":" << lic << line << dc << "] [" << // [File:Line]
				ec << "ERROR" << dc << "] " << // [Error]
				ec << msg << "\n" << rc; // Msg
			#endif

			logFile << "[" << time << "] " << "[" << file << ":" << line << "] " << "[ERROR] " << msg << "\n";
		}
	};
}