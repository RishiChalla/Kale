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

#include "../Window.hpp"
#include <Kale/Logger/Logger.hpp>
#include <exception>

using namespace Kale;

/**
 * Cleans vulkan objects before the application closes
 */
void Window::cleanupVulkan() {
	try {
		#ifdef ISLANDS_DEBUG
		destroyDebugMessageCallback();
		#endif
		
		vulkanLogicalDevice.destroy();
		vulkanInstance.destroy();
	}
	catch (const std::exception& e) {
		console.error(e.what());
		exit(0);
	}
}