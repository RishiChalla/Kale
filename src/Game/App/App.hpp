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

#include <Engine/Application/Application.hpp>

namespace Islands {

	/**
	 * All required fonts for the game 
	 */
	enum class Font {
		Future
	};
	
	/**
	 * All required textures for the game 
	 */
	enum class Texture {
		
	};
	
	/**
	 * All required shaders for the game 
	 */
	enum class Shader {

	};

	/**
	 * Represents the main application class
	 */
	class App : public Application {

	protected:
		
		/**
		 * Called when the application begins
		 */
		void onBegin() override;
	
	public:

		/**
		 * Creates a new app instance
		 */
		App();
	};
}