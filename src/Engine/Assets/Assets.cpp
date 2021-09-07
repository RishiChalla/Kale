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

#include "Assets.hpp"

using namespace Islands;

/**
 * The main instance of the asset manager
 */
Assets* Islands::assets = nullptr;

/**
 * Gets an asset
 * @param font The asset to get
 * @returns The asset
 */
sf::Font& Assets::get(Font font) {
	return fontMap.at(font);
}

/**
 * Gets an asset
 * @param texture The asset to get
 * @returns The asset
 */
sf::Texture& Assets::get(Texture texture) {
	return textureMap.at(texture);
}

/**
 * Gets an asset
 * @param shader The asset to get
 * @returns The asset
 */
sf::Shader& Assets::get(Shader shader) {
	return shaderMap.at(shader);
}