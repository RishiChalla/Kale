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

#include <list>
#include <memory>
#include <tuple>
#include <SFML/Graphics.hpp>

namespace Islands {

	/**
	 * The main scene class for the engine, all other scenes must inherit from this class
	 */
	class Scene {

	private:

		/**
		 * A reference to the main window used for rendering this scene
		 */
		sf::RenderWindow& window;

		/**
		 * A linked list of all the nodes to be drawn
		 */
		std::list<std::tuple<unsigned int, std::shared_ptr<sf::Drawable>>> nodes;
		
		/**
		 * Draws/renders the scene in its entirety
		 */
		void render() const;

		/**
		 * Begins the rendering of the scene
		 */
		void begin();

		/**
		 * Updates the scene
		 * This method is thread safe, and is intended to be called from a separate update thread
		 */
		void update();

		/**
		 * Ends the rendering of the scene
		 */
		void end();

		friend class Application;
	
	protected:

		/**
		 * Represents the background color used for clearing the screen before drawing
		 */
		sf::Color bgColor = sf::Color::Black;

		/**
		 * Adds a node to the scene
		 * @param zIndex The rendering order of the element
		 * @param node The node to add
		 */
		void addNode(unsigned int zIndex, std::shared_ptr<sf::Drawable> node);

		/**
		 * Removes a node from the scene
		 * @param node The node to remove
		 */
		void removeNode(std::shared_ptr<sf::Drawable> node);

		// Virtual Methods

		/**
		 * Called every frame, don't do anything too heavy here
		 * Should be used for calculating player/enemy movement/etc
		 */
		virtual void onUpdate() {}

		/**
		 * Called when the scene is presented
		 * Do setup stuff here
		 */
		virtual void onBegin() {}

		/**
		 * Called when the scene is ended/moved away
		 * Destroy resources here
		 */
		virtual void onEnd() {}

	public:

		/**
		 * Creates a new scene instance
		 * @param window A reference to the window used for rendering
		 */
		Scene(sf::RenderWindow& window);
	};
}