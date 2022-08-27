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

#include <chrono>

namespace Kale {
	
	/**
	 * Calculates the time used to render a frame/stack code execution
	 */
	class Clock {
	
	private:
		
		/**
		 * The second point of time for reference used internally
		 */
		std::chrono::system_clock::time_point a;
		
		/**
		 * The second point of time for reference used internally
		 */
		std::chrono::system_clock::time_point b;
		
	public:
		
		/**
		 * Creates a new clock instance and sets the first time point to now
		 */
		Clock();
		
		/**
		 * Causes the thread to sleep if the call time exceeds the min call time
		 * @param minMSPC The minimum milliseconds per cycle
		 * @returns The cps of the thread
		 */
		float sleep(double minMSPC);
	};
}
