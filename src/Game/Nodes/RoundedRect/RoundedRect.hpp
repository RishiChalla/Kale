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

#include <SFML/Graphics.hpp>
#include <Engine/Math/Vector/Vector.hpp>

namespace Islands {

	/**
	 * Stores the type of rounded rects based on the uniforms given to the shaders
	 */
	enum class RoundedRectType {
		LeftCenter, RightCenter, TopCenter, BottomCenter,
		HorizontalCenters, VerticalCenters
	};

	/**
	 * This class contains a node that can be used for creating 
	 * a rounded rectangle of any kind (including inverse and mixed)
	 */
	class RoundedRect : public sf::Drawable, sf::Transformable {
	private:

		/**
		 * The vertices used for storing this entity
		 */
		sf::VertexArray vertices;

		/**
		 * The shader used for drawing this node
		 */
		sf::Shader* shader;

		/**
		 * Used for storing the left/right centers for passing to shaders
		 */
		sf::Vector2f* centers = nullptr;

		/**
		 * The type of rounded rect used for passing uniforms to shaders
		 */
		RoundedRectType type;

		/**
		 * The radius of the semi circles on the caps
		 */
		float threshold;

		/**
		 * Checks how many centers this rounded rectangle has
		 * @returns The number of centers
		 */
		inline size_t numCenters() const {
			if (type == RoundedRectType::HorizontalCenters ||
				type == RoundedRectType::VerticalCenters)
				return 2;
			else
				return 1;
		}

		/**
		 * heap allocates and calculates the centers
		 */
		void calculateCenters(const Vector2f& scaledPos, const Vector2f& scaledSize);

	protected:

		/**
		 * Draws the node to the window
		 * @param target The target to draw to
		 * @param states The context/states data regarding drawing
		 */
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	public:

		/**
		 * Creates a new rounded rectangle instance
		 * @param type The type of rounded rect to create
		 * @param shader The shader to use for rendering this rounded rect
		 * @param pos The position of the rounded rectangle (normalized to be multiplied by the window size)
		 * @param size The size of the rounded rectangle (normalized to be multiplied by the window size)
		 * @param color The color of the rounded rectangle
		 */
		RoundedRect(RoundedRectType type, sf::Shader* shader, const Vector2f& pos,
			const Vector2f& size, const sf::Color& color);
		
		/**
		 * Copies the rounded rectangle
		 * @param other The rounded rectangle to copy from
		 */
		RoundedRect(const RoundedRect& other);

		/**
		 * Copies the rounded rectangle
		 * @param other The rounded rectangle to copy from
		 */
		void operator=(const RoundedRect& other);

		// Rounded rectangles dont support moving
		RoundedRect(RoundedRect&& other) = delete;
		void operator=(RoundedRect&& other) = delete;
		
		/**
		 * Frees resources
		 */
		~RoundedRect();

		/**
		 * Repositions the node based on the window size, should be called on resize
		 */
		void rePosition(const Vector2ui& oldSize, const Vector2ui& newSize);
	};
}