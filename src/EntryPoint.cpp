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

#include <Application/Application.hpp>

using namespace Kale;

/**
 * Create your inherited application here and heap allocate it.
 * Do not worry about its destruction, the engine will take care of it.
 */
extern Application* createApplication();

/**
 * The main function/entry point of the program
 */
int main() {
	// Heap allocate the main app instance
	mainApp = createApplication();

	// Run the app
	mainApp->run();

	// Delete the app/free the resources
	delete mainApp;

	// End the program
	return 0;
}