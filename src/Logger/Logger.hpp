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
#include <iostream>

namespace Islands {

	/**
	 * The main logger class for the game
	 */
	class Logger {
	private:

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
			std::cout << time << msg << "\n";
			logFile << time << msg << "\n";
		}

		/**
		 * Logs info to the console
		 * @param msg The message to log
		 */
		template <typename T> void info(T msg) {
			std::string time = getTimePrefix();
			std::cout << time << "[INFO] " << msg << "\n";
			logFile << time << "[INFO] " << msg << "\n";
		}

		/**
		 * Logs a warning to the console
		 * @param msg The message to log
		 */
		template <typename T> void warn(T msg) {
			std::string time = getTimePrefix();
			std::cout << time << "[WARNING] " << msg << "\n";
			logFile << time << "[WARNING] " << msg << "\n";
		}

		/**
		 * Logs an error to the console
		 * @param msg The message to log
		 */
		template <typename T> void error(T msg) {
			std::string time = getTimePrefix();
			std::cout << time << "[ERROR] " << msg << "\n";
			logFile << time << "[ERROR] " << msg << "\n";
		}

		/**
		 * Logs output to the console
		 * @param line The line number the log is being called from
		 * @param file The name of the file the log is being called from
		 * @param msg The message to log
		 */
		template <typename T> void log(unsigned int line, const char* file, T msg) {
			std::string time = getTimePrefix();
			std::cout << time << "[" << file << ":" << line << "] " << msg << "\n";
			logFile << time << "[" << file << ":" << line << "] " << msg << "\n";
		}

		/**
		 * Logs info to the console
		 * @param line The line number the log is being called from
		 * @param file The name of the file the log is being called from
		 * @param msg The message to log
		 */
		template <typename T> void info(unsigned int line, const char* file, T msg) {
			std::string time = getTimePrefix();
			std::cout << time << "[" << file << ":" << line << "] " << "[INFO] " << msg << "\n";
			logFile << time << "[" << file << ":" << line << "] " << "[INFO] " << msg << "\n";
		}

		/**
		 * Logs a warning to the console
		 * @param line The line number the log is being called from
		 * @param file The name of the file the log is being called from
		 * @param msg The message to log
		 */
		template <typename T> void warn(unsigned int line, const char* file, T msg) {
			std::string time = getTimePrefix();
			std::cout << time << "[" << file << ":" << line << "] " << "[WARNING] " << msg << "\n";
			logFile << time << "[" << file << ":" << line << "] " << "[WARNING] " << msg << "\n";
		}

		/**
		 * Logs an error to the console
		 * @param line The line number the log is being called from
		 * @param file The name of the file the log is being called from
		 * @param msg The message to log
		 */
		template <typename T> void error(unsigned int line, const char* file, T msg) {
			std::string time = getTimePrefix();
			std::cout << time << "[" << file << ":" << line << "] " << "[ERROR] " << msg << "\n";
			logFile << time << "[" << file << ":" << line << "] " << "[ERROR] " << msg << "\n";
		}
	};
}