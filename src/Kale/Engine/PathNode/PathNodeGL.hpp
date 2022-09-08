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

		struct InnerVertex {
			Vector2f pos;
			float skeletonTransformWeight;
			float skeletonTransform1;
			float skeletonTransform2;
		};

		struct OuterVertex {
			Vector2f pos;
			CubicBezier bezier;
			float skeletonTransformWeight;
			float skeletonTransform1;
			float skeletonTransform2;
		};

		std::unique_ptr<OpenGL::VertexArray<InnerVertex, 2, 1, 1, 1>> innerTriangles;
		std::unique_ptr<OpenGL::VertexArray<OuterVertex, 2, 2, 2, 2, 2, 1, 1, 1>> outerTriangles;

		Path path;

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
