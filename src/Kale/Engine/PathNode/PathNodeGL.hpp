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

#include <Kale/Engine/AnimatableNode/AnimatableNode.hpp>
#include <Kale/Engine/Collidable/Collidable.hpp>
#include <Kale/Math/Transform/Transform.hpp>
#include <Kale/Math/Path/Path.hpp>
#include <Kale/OpenGL/VertexArray/VertexArray.hpp>
#include <Kale/OpenGL/Shader/Shader.hpp>

#include <memory>

namespace Kale {

	struct PathNodeState {

	};

	/**
	 * Used for rendering filled bezier paths
	 * @tparam An enum class containing all the animation states
	 */
	template <typename T> class PathNode : public AnimatableNode<T, Path>, public Collidable {
	private:

		struct Vertex {
			Vector2f pos;
			CubicBezier bezier;
		};

		std::unique_ptr<OpenGL::VertexArray<Vector2f, 2>> innerTriangles;
		std::unique_ptr<OpenGL::VertexArray<Vertex, 2, 2, 2, 2, 2>> outerTriangles;

		Path currentPath;
		size_t pathSize;

	protected:

		/**
		 * Called when the node is added to the scene, guaranteed to be called before any updates & renders
		 * and from the main thread.
		 * @param scene The scene the node has been added to
		 */
		virtual void begin(const Scene& scene) override {
			innerTriangles = std::make_unique<OpenGL::VertexArray<Vector2f, 2>>();
			outerTriangles = std::make_unique<OpenGL::VertexArray<Vertex, 2, 2, 2, 2, 2>>();
		}

		/**
		 * Called prior to update, perfect place to do things such as updating the bounding box, etc
		 * @param threadNum the index of the thread this update is called on
		 * @param scene The scene being updated to
		 * @param deltaTime The duration of the last frame in microseconds
		 */
		virtual void preUpdate(size_t threadNum, const Scene& scene, float deltaTime) override {

			if (!AnimatableNode<T, Path>::transitioning && !currentPath.beziers.empty()) return;

			// Update Animatable Node
			AnimatableNode<T, Path>::preUpdate(deltaTime, scene);

			// Update Current Path
			std::vector<std::pair<T, float>> composition = AnimatableNode<T, Path>::getStateComposition();
			currentPath = Path(AnimatableNode<T, Path>::structures.at(composition[0].first).beziers.size());
			for (const std::pair<T, float>& comp : composition)
				currentPath += AnimatableNode<T, Path>::structures.at(comp.first) * comp.second;

			// Update bounding box
			Node::boundingBox = currentPath.getBoundingBox();
		}

		/**
		 * Renders the node
		 * @param camera The camera to render with
		 */
		virtual void render(const Camera& camera, float deltaTime) const override {
			
		}

		/**
		 * Called when the node is removed from the scene, guaranteed to be called from the main thread
		 */
		virtual void end(const Scene& scene) override {
			innerTriangles.reset();
			outerTriangles.reset();
		}

	public:

		/**
		 * The transform of this node
		 */
		Transform transform;

		PathNode(size_t pathSize) 

	};
}

#endif
