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

#include "Clock.hpp"

#include <thread>

using namespace Kale;

/**
 * Creates a new clock instance and sets the first time point to now
 */
Clock::Clock() : a(std::chrono::system_clock::now()), b(std::chrono::system_clock::now()) {
	// Empty Body
}

/**
 * Causes the thread to sleep if the call time exceeds the min call time
 * @param minMSPC The minimum milliseconds per cycle
 * @returns The cps of the thread
 */
float Clock::sleep(double minMSPC) {
	minMSPC /= 2;
	a = std::chrono::system_clock::now();
	std::chrono::duration<double, std::milli> work_time = a - b;
	
	if (work_time.count() < minMSPC) {
		std::chrono::duration<double, std::milli> delta_ms(minMSPC - work_time.count());
		auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
		std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
	}
	
	b = std::chrono::system_clock::now();
	std::chrono::duration<double, std::milli> sleep_time = b - a;
	return 1000.0f / static_cast<float>((sleep_time + work_time).count());
}
