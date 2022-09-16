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

#include <Kale/Engine/Node/Node.hpp>
#include <Kale/Engine/Collidable/Collidable.hpp>
#include <Kale/Engine/Transformable/Transformable.hpp>
#include <Kale/Math/Path/Path.hpp>
#include <Kale/OpenGL/VertexArray/VertexArray.hpp>
#include <Kale/OpenGL/Shader/Shader.hpp>

#include <memory>

namespace Kale {

	/**
	 * Used for rendering filled bezier paths
	 * @tparam An enum class containing all the animation states
	 */
	class PathNode : public Node, public Collidable, public Transformable {
	private:

		struct Vertex {
			Vector2f pos;
			float bezier;
			float skeletonTransformWeight;
			float skeletonTransform1;
			float skeletonTransform2;
		};

		std::unique_ptr<OpenGL::VertexArray<Vertex, 2, 1>> vertexArray;
		Path path;
		static inline std::unique_ptr<const OpenGL::Shader> shader = nullptr;

		/**
		 * The location of the uniform within the shader for rendering this node
		 */
		inline static unsigned int cameraUniform;
		
		/**
		 * The location of the uniform within the shader for rendering this node
		 */
		inline static unsigned int localUniform;
		
		/**
		 * The location of the uniform within the shader for rendering this node
		 */
		inline static unsigned int vertexColorUniform;
		
		/**
		 * The location of the uniform within the shader for rendering this node
		 */
		inline static unsigned int zPositionUniform;

		/**
		 * The location of the uniform within the shader for rendering this node
		 */
		inline static unsigned int beziersUniform;
		
		/**
		 * The location of the attribute within the shader for rendering this node
		 */
		inline static unsigned int posAttribute;
		
		/**
		 * The location of the attribute within the shader for rendering this node
		 */
		inline static unsigned int bezierAttribute;
		
		/**
		 * The location of the attribute within the shader for rendering this node
		 */
		inline static unsigned int skeletonTransformWeightAttribute;
		
		/**
		 * The location of the attribute within the shader for rendering this node
		 */
		inline static unsigned int skeletonTransform1Attribute;
		
		/**
		 * The location of the attribute within the shader for rendering this node
		 */
		inline static unsigned int skeletonTransform2Attribute;
		
		/**
		 * Creates and compiles shaders
		 */
		static void setup();

		/**
		 * Deletes shaders/cleans up
		 */
		static void cleanup();

		friend class Application;

	protected:

		/**
		 * Called when the node is added to the scene, guaranteed to be called before any updates & renders
		 * and from the main thread.
		 * @param scene The scene the node has been added to
		 */
		virtual void begin(const Scene& scene) override;

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
		 * The z position of this node, setting this allows for rendering in front of or behind other nodes.
		 */
		float zPosition;

		/**
		 * The color of this path node
		 */
		Color color = Color(0x333333);

		/**
		 * Creates a blank pathnode with nothing to render
		 */
		PathNode();

		/**
		 * Creates a path node given the path to use
		 * @param path The path to use
		 */
		PathNode(const Path& path);

	};
}

#endif