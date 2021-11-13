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

#include "Logger.hpp"

#include <Kale/Core/Application/Application.hpp>

#include <filesystem>
#include <date/date.h>
#include <date/tz.h>

#ifdef KALE_WINDOWS
#include <Windows.h>
#endif

using namespace Kale;

/**
 * The main console used for printing to the output
 */
Logger Kale::console;

/**
 * Creates the logger object, this will not be initialized until load is called
 */
Logger::Logger() {
	// Empty Body
}

/**
 * Loads and sets up the console
 * @param applicationName The name of the application
 */
void Logger::load(const std::string& applicationName) {

	// Create the folder for this application if it doesn't already exist
	if (!std::filesystem::exists("." + applicationName))
		std::filesystem::create_directory("." + applicationName);
	if (!std::filesystem::exists("." + applicationName + "/logs"))
		std::filesystem::create_directory("." + applicationName + "/logs");

	// Create/open the log file in the correct folder
	logFile.open("." + applicationName + "/logs/" + date::format("%F--%H-%M", std::chrono::system_clock::now()) + ".log");

	#ifdef KALE_WINDOWS
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE) {
		return;
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode)) {
		return;
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode)) {
		return;
	}
	#endif
}

/**
 * Gets the time prefix for logging
 * @returns the time prefix
 */
std::string Logger::getTimePrefix() {
	#ifndef KALE_WINDOWS
	return date::format("%I:%M %p", date::make_zoned(date::current_zone(), std::chrono::system_clock::now()));
	#else 
	// Windows doesn't support current timezone so instead we'll need to use UTC
	return date::format("%I:%M %p", std::chrono::system_clock::now());
	#endif
}
