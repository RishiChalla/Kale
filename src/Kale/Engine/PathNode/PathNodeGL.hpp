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

#ifdef KALE_OPENGL

#include <Kale/Core/Scene/Scene.hpp>
#include <Kale/Engine/Node/Node.hpp>
#include <Kale/Engine/StateAnimatable/StateAnimatable.hpp>
#include <Kale/Engine/SkeletalAnimatable/SkeletalAnimatable.hpp>
#include <Kale/Engine/Collidable/Collidable.hpp>
#include <Kale/Engine/Transformable/Transformable.hpp>
#include <Kale/Math/Path/Path.hpp>
#include <Kale/OpenGL/VertexArray/VertexArray.hpp>
#include <Kale/OpenGL/Shader/Shader.hpp>

#include <memory>
#include <optional>
#include <vector>
#include <array>
#include <utility>

#include <nlohmann/json.hpp>

namespace Kale {

	/**
	 * Javascript Standard Object Notation allows for saving and using permanent configuration files, via the nlohmann/json C++
	 * library.
	 */
	using JSON = nlohmann::json;

	/**
	 * Used for rendering filled bezier paths
	 */
	class PathNode : public Node, public Transformable, public Collidable {
	public:

		/**
		 * Styles of rendering strokes on a path
		 * @note Inside and Outside only work when the path is closed. For an open path, this is undefined behavior.
		 */
		enum class StrokeStyle {
			Neither = 0, Both = 1, Inside = 2, Outside = 3
		};

		/**
		 * Contains the weights required for skinning/skeletal rigging a single cubic bezier curve
		 */
		struct BezierWeights {
			
			/**
			 * An array of pairs with the bone index (-1 if none) and the significance of the bone. All weights must add up to 1.
			 */
			std::array<std::pair<int, float>, 4> startWeight;
			
			/**
			 * An array of pairs with the bone index (-1 if none) and the significance of the bone. All weights must add up to 1.
			 */
			std::array<std::pair<int, float>, 4> controlPoint1Weight;
			
			/**
			 * An array of pairs with the bone index (-1 if none) and the significance of the bone. All weights must add up to 1.
			 */
			std::array<std::pair<int, float>, 4> controlPoint2Weight;
			
			/**
			 * An array of pairs with the bone index (-1 if none) and the significance of the bone. All weights must add up to 1.
			 */
			std::array<std::pair<int, float>, 4> endWeight;
		};

	private:

		/**
		 * The vertex array used for rendering
		 */
		std::unique_ptr<OpenGL::VertexArray<Vector2f, 2>> vertexArray;

		/**
		 * The path being rendered
		 */
		Path path;

		/**
		 * The shader used for rendering
		 */
		static inline std::unique_ptr<const OpenGL::Shader> shader = nullptr;

		/**
		 * The location of the uniform within the shader for rendering this node
		 */
		inline static unsigned int cameraUniform, localUniform, vertexColorUniform, zPositionUniform,
			beziersUniform, numBeziersUniform, strokeUniform, fillUniform, strokeRadiusUniform, strokeColorUniform;
		
		/**
		 * The location of the attribute within the shader for rendering this node
		 */
		inline static unsigned int posAttribute;
		
		/**
		 * Creates and compiles shaders
		 */
		static void setup();

		/**
		 * Deletes shaders/cleans up
		 */
		static void cleanup();

		/**
		 * Updates the bounding box accounting for stroke
		 */
		void updateBoundingBox();

		friend class Application;

	protected:

		/**
		 * Called when the node is added to the scene, guaranteed to be called before any updates & renders
		 * and from the main thread.
		 * @param scene The scene the node has been added to
		 */
		virtual void begin(const Scene& scene) override;

		/**
		 * Called prior to update, perfect place to do things such as updating the bounding box, etc
		 * @param threadNum the index of the thread this update is called on
		 * @param scene The scene being updated to
		 * @param deltaTime The duration of the last frame in microseconds
		 */
		virtual void preUpdate(size_t threadNum, const Scene& scene, float deltaTime) override;

		/**
		 * Renders the node
		 * @param camera The camera to render with
		 */
		virtual void render(const Camera& camera, float deltaTime) const override;

		/**
		 * Called when the node is removed from the scene, guaranteed to be called from the main thread
		 */
		virtual void end(const Scene& scene) override;

	public:

		/**
		 * The FSM used for path states. Path nodes support FSM based animations, to use the FSM simply set the value of this optional and fill
		 * the map. Access to the FSM must be externally synchronized if done from multiple threads.
		 */
		std::optional<StateAnimatable<Path>> pathFSM;

		/**
		 * A skeletal animatable to use for animating this path node. Both pathFSM and skeletalAnimatable cannot be set at the same time.
		 */
		std::shared_ptr<SkeletalAnimatable> skeletalAnimatable;

		/**
		 * A vector of the skeletal weights for skeletal animations. This must be the same length as the number of beziers in the path, and it must be
		 * set if skeletalAnimatable isn't nullptr.
		 */
		std::optional<std::vector<BezierWeights>> skeletalWeights;

		/**
		 * An optional path to be used for skeletal rigging. If a skeletal rig is provided, then this base path must be the path where
		 * all vertices are aligned with the skeleton's base form
		 */
		std::optional<Path> basePath;

		/**
		 * The z position of this node, setting this allows for rendering in front of or behind other nodes.
		 */
		float zPosition;

		/**
		 * Whether or not to fill the path when rendering
		 * @note This must be false if the path is open. Filling an open path results in undefined behavior
		 */
		bool fill = true;

		/**
		 * Whether or not to stroke the path when rendering
		 */
		StrokeStyle stroke = StrokeStyle::Neither;

		/**
		 * The radius of the stroke if stroke is true
		 */
		float strokeRadius = 20.0f;

		/**
		 * The color of this path node
		 */
		Color color = Color(0x333333);

		/**
		 * The color to stroke this path node with
		 */
		Color strokeColor = Color(0x555555);

		/**
		 * Creates a blank pathnode with nothing to render
		 */
		PathNode();

		/**
		 * Creates a path node based on the json saved values
		 * @param json The json saved values
		 */
		PathNode(const JSON& json);

		/**
		 * Creates a path node given the path to use
		 * @param path The path to use
		 * @param fill Whether or not to fill the node
		 * @param stroke Whether or not to stroke the node
		 */
		PathNode(const Path& path, bool fill = true, StrokeStyle stroke = StrokeStyle::Neither);

	};
}

#endif
