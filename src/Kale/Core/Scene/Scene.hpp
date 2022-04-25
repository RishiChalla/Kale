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

#include <Kale/Core/Events/Events.hpp>
#include <Kale/Math/Transform/Transform.hpp>
#include <Kale/Engine/Node/Node.hpp>
#include <Kale/Engine/Light/Light.hpp>

#include <unordered_set>
#include <list>
#include <utility>
#include <memory>

namespace Kale {
	
	/**
	 * The main scene class for the game
	 * This class handles rendering and nodes management
	 */
	class Scene : public EventHandler {
	private:

		/**
		 * A list of all the nodes to be presented in the current scene
		 */
		std::list<std::shared_ptr<Node>> nodes;

		/**
		 * A list of the lights affecting the current scene
		 */
		std::unordered_set<std::shared_ptr<Light>> lights;

		/**
		 * The world to screen transformation matrix. Used internally for rendering and converting
		 * vertices from world points to screen points.
		 */
		Transform worldToScreen;

		/**
		 * Renders the current scene
		 */
		void render();

		/**
		 * Updates the current scene
		 * @param deltaTime The amount of microseconds since the last update
		 */
		void update(float deltaTime);

		friend class Application;

	protected:

		/**
		 * The background color of the scene to use for clearing the screen
		 */
		Color bgColor = Color(0xFFFFFF);

		/**
		 * The ambient light color, alpha is color intensity
		 */
		Color ambient = Color(0xFFFFFF, 0.0f);

		/**
		 * The intensity of the ambient light
		 */
		float ambientIntesity = 1.0;

		/**
		 * The scene's camera for viewing
		 */
		Camera camera;
		
		/**
		 * The viewport of the world
		 * (the width and height of the screen in the world's coordinates)
		 * This always has a height of 1080p and the width is dependant on the screen's aspect ratio.
		 * It is recommended to code all nodes based off of the 1080p height and let the engine do the scaling
		 */
		Vector2f viewport;

		/**
		 * Adds a node to the scene to render/update
		 * @param node The node to add
		 */
		void addNode(std::shared_ptr<Node>& node);

		/**
		 * Adds a node to the scene to render/update
		 * @param node The node to add
		 */
		template <typename T> void addNode(std::shared_ptr<T>& node) {
			std::shared_ptr<Kale::Node> nodePtr = std::dynamic_pointer_cast<Kale::Node>(node);
			addNode(nodePtr);
		}

		/**
		 * Removes a node from the scene
		 * @param node The node to remove
		 */
		void removeNode(std::shared_ptr<Node>& node);

		/**
		 * Adds a light to the scene
		 * @param light The light to add to the scene
		 */
		void addLight(std::shared_ptr<Light>& light);

		/**
		 * Removes a light from the scene
		 * @param light The light to remove from the scene
		 */
		void removeLight(std::shared_ptr<Light>& light);

		/**
		 * Called when the current scene is presented
		 */
		virtual void onPresent();

		/**
		 * Called when the scene is about to be changed
		 */
		virtual void onSceneChange();

		/**
		 * Called when the event is fired
		 */
		void onWindowResize(Vector2ui oldSize, Vector2ui newSize) override;

		friend class Application;

	public:

		/**
		 * Constructs a new scene
		 */
		Scene();

		/**
		 * Gets the ndoes within the scene
		 * @returns The nodes
		 */
		const std::list<std::shared_ptr<Node>>& getNodes() const;

		/**
		 * Gets the lights within the scene
		 * @returns The lights
		 */
		const std::unordered_set<std::shared_ptr<Light>>& getLights() const;

		/**
		 * Gets the background color of the scene
		 * @returns The background color
		 */
		Color getBgColor() const;

		/**
		 * Gets the ambient color and intensity of the scene
		 * @returns a pair of the ambient color (where alpha is color intensity), and light intensity
		 */
		std::pair<Color, float> getAmbient() const;

		/**
		 * Gets the camera used to render this scene
		 * @returns The camera
		 */
		const Camera& getCamera() const;

		/**
		 * Gets the current viewport of the scene
		 * @returns The viewport
		 */
		const Vector2f getViewport() const;
	};
}
