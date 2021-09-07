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

#include "Application_fwd.hpp"
#include <Engine/Assets/Assets.hpp>

namespace Islands {

	/**
	 * Creates a new application instance
	 * @param fontsList The fonts to load
	 * @param texturesList The textures to load
	 * @param shadersList The shaders to load
	 */
	template <size_t numFonts, size_t numTextures, size_t numShaders>
	Application::Application(std::array<std::tuple<Font, const char*>, numFonts> fontsList,
		std::array<std::tuple<Texture, const char*>, numTextures> texturesList,
		std::array<std::tuple<Shader, const char*, const char*>, numShaders> shadersList) {

		// Init app
		events = new Events(window);
		assets = new Assets(fontsList, texturesList, shadersList);
	}
}