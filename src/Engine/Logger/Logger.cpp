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
#include <filesystem>
#include <date/date.h>
#include <date/tz.h>

using namespace Islands;

/**
 * Creates a new logger instance
 */
Logger::Logger() {

	// Create the folder for this application if it doesn't already exist
	if (!std::filesystem::exists(".Islands"))
		std::filesystem::create_directory(".Islands");
	if (!std::filesystem::exists(".Islands/logs"))
		std::filesystem::create_directory(".Islands/logs");

	// Create/open the log file in the correct folder
	logFile.open(".Islands/logs/" + date::format("%F--%H-%M", std::chrono::system_clock::now()) + ".log");
}

/**
 * Gets the time prefix for logging
 * @returns the time prefix
 */
std::string Logger::getTimePrefix() {
	return "[" + date::format("%I:%M %p", date::make_zoned(date::current_zone(), std::chrono::system_clock::now())) + "] ";
}
