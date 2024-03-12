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

#include "Logger.hpp"

#include <Kale/Core/Application/Application.hpp>

#include <filesystem>

// Apple-clang doesn't support C++20's std::format properly
#if defined(KALE_OSX) || defined(KALE_IOS)

#include <date/date.h>
#include <date/tz.h>

#else

#include <format>
#include <chrono>

#endif

using namespace Kale;

/**
 * Creates the logger object, this will not be initialized until load is called
 */
Logger::Logger() {
#ifdef KALE_DEBUG
	std::cout.setf(std::ios::fixed, std::ios::floatfield);
	std::cout.setf(std::ios::showpoint);
#endif
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
	#ifdef KALE_OSX
	logFile.open("." + applicationName + "/logs/" + date::format("%F--%H-%M", std::chrono::system_clock::now()) + ".log");
	#else
	auto timestamp = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());
	logFile.open("." + applicationName + "/logs/" + std::format("{:%F--%H-%M}", std::chrono::zoned_time{std::chrono::current_zone(), timestamp}) + ".log");
	#endif

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
	#ifdef KALE_OSX
	// OSX Doesn't support C++ 20's std::format and std::chrono::zoned_time
	return date::format("%I:%M %p", date::make_zoned(date::current_zone(), std::chrono::system_clock::now()));
	#else
	auto timestamp = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());
	return std::format("{:%I:%M %p}", std::chrono::zoned_time{std::chrono::current_zone(), timestamp});
	#endif
}
