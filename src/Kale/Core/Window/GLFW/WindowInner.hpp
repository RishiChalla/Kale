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

// NEVER INCLUDE THIS FILE DIRECTLY - THIS IS ONLY MEANT TO BE INCLUDED FROM WINDOW.HPP

#ifdef KALE_GLFW

private:

/**
 * The main GLFW Window pointer
 */
GLFWwindow* window;

/**
 * The title of the window
 */
const char* title;

#endif
