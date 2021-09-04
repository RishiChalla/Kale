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

#include <map>
#include <tuple>
#include <array>
#include <SFML/Graphics.hpp>

namespace Islands {

	/**
	 * Game defined enum of all the required assets
	 */
	enum class Font;
	
	/**
	 * Game defined enum of all the required assets
	 */
	enum class Texture;
	
	/**
	 * Game defined enum of all the required assets
	 */
	enum class Shader;

	/**
	 * The main assets/resource manager for the game
	 */
	class Assets {
	private:
		
		/**
		 * Maps all required resources/assets
		 */
		std::map<Font, sf::Font> fontMap;
		
		/**
		 * Maps all required resources/assets
		 */
		std::map<Texture, sf::Texture> textureMap;
		
		/**
		 * Maps all required resources/assets
		 */
		std::map<Shader, sf::Shader> shaderMap;

	protected:

		/**
		 * Creates a new assets instance
		 * @param fontsList The fonts to load
		 * @param texturesList The textures to load
		 * @param shadersList The shaders to load
		 */
		template <int numFonts, int numTextures, int numShaders>
		void create(std::array<std::tuple<Font, const char*>, numFonts> fontsList,
			std::array<std::tuple<Texture, const char*>, numTextures> texturesList,
			std::array<std::tuple<Shader, sf::Shader::Type, const char*>, numShaders> shadersList) {
			
			// Loading of assets
			for (auto fontTuple : fontsList) {
				sf::Font& font = fontMap[std::get<0>(fontTuple)];
				font.loadFromFile(std::get<1>(fontTuple));
			}

			for (auto textureTuple : texturesList) {
				sf::Texture& texture = textureMap[std::get<0>(textureTuple)];
				texture.loadFromFile(std::get<1>(textureTuple));
			}

			for (auto shaderTuple : shadersList) {
				sf::Shader& shader = shaderMap[std::get<0>(shaderTuple)];
				shader.loadFromFile(std::get<2>(shaderTuple), std::get<1>(shaderTuple));
			}
		}

		friend class Application;

	public:

		/**
		 * Gets an asset
		 * @param font The asset to get
		 * @returns The asset
		 */
		const sf::Font& operator[](Font font) const;
		
		/**
		 * Gets an asset
		 * @param texture The asset to get
		 * @returns The asset
		 */
		const sf::Texture& operator[](Texture texture) const;
		
		/**
		 * Gets an asset
		 * @param shader The asset to get
		 * @returns The asset
		 */
		const sf::Shader& operator[](Shader shader) const;
	};

	/**
	 * The main instance of the asset manager
	 */
	extern Assets assets;
}