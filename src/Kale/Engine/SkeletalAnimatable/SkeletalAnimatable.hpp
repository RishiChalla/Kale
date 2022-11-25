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

#include <Kale/Math/Transform/Transform.hpp>
#include <Kale/Engine/StateAnimatable/StateAnimatable.hpp>

#include <utility>
#include <vector>
#include <tuple>
#include <array>
#include <mutex>
#include <algorithm>

namespace Kale {

	/**
	 * Represents a skeleton used for skeletal rigging. A collection of connected bones which allow for transforming vertices
	 * in unique ways
	 */
	struct Skeleton {

		/**
		 * A structure containing a single bone in a skeleton
		 */
		struct Bone {
			
			/**
			 * Transforms from the world coordinate space to the bone's coordinate space
			 */
			Transform worldToBone;

			/**
			 * Transforms from the previous bone's coordinate space to this bone's coordinate space
			 */
			Transform previousToBone;

			/**
			 * The inverse matrix of worldToBone
			 */
			Transform inverseWorldToBone;

			/**
			 * The index of the previous bone, -1 if this is the base
			 */
			int previousBone;

			/**
			 * The length of this bone
			 */
			float length;

			/**
			 * The rotation of this bone in radians
			 */
			float rotation;
		};
		
		/**
		 * A vector of the held bones
		 */
		std::vector<Bone> bones;

		/**
		 * Creates an empty skeleton
		 */
		Skeleton();

		/**
		 * Creates a skeleton given a vector of tuples containing the index of the previous bone, the bone length, and the bone rotation
		 * @param bones The vector of bones
		 * @param unit The unit of the rotation given
		 */
		Skeleton(const std::vector<std::tuple<int, float, float>>& bones, AngleUnit unit);
		
		/**
		 * Creates a skeleton from a vector of bones
		 * @param bones The list of bones
		 */
		Skeleton(const std::vector<Bone>& bones);

		/**
		 * Calculates a single bone
		 * @param bones A vector of the existing bones, this must containt he parent of this bone if previousBone is not -1
		 * @param previousBone The previous bone index, -1 if this is the base
		 * @param length The length of the bone
		 * @param rotation The rotation of the bone
		 * @param unit The unit the bone's rotation is given in
		 * @returns The calculated bone
		 */
		static Bone calculateBone(const std::vector<Bone>& bones, int previousBone, float length, float rotation, AngleUnit unit) {
			Bone bone;
			bone.previousBone = previousBone;
			bone.length = length;
			bone.rotation = rotation;
			bone.previousToBone = Transform(length, 0.0f, rotation, 1.0f, 1.0f, unit);
			
			// If the bone is the root
			if (previousBone == -1) bone.worldToBone = bone.previousToBone;
			else bone.worldToBone = bone.previousToBone * bones[previousBone].worldToBone;
			bone.inverseWorldToBone = bone.worldToBone.inverse();
			return bone;
		}

		/**
		 * Transforms a single vertex given its weights
		 * @param offsets The offset matrices of the skeleton to transform with
		 * @param vert The vertex to transform
		 * @param weights The weights of the vertex, 4 pairs of the index of the bone along with the significance. Must add up to 1, set the
		 * index of the bone to -1 if it is unassigned.
		 * @returns The transformed vertex
		 */
		static Vector2f transform(const std::vector<Transform>& offsets, Vector2f vert, const std::array<std::pair<int, float>, 4>& weights) {
			Vector2f transformed = Vector2f::zero();
			for (const std::pair<int, float> weight : weights) {
				if (weight.first == -1) break;
				transformed += offsets[weight.first].transform(vert) * weight.second;
			}
			return transformed;
		}
		
	};

	/**
	 * A system for skeletal animations. Unlike the state animatable system, skeletal animatables are a node and can be directly added to the
	 * scene. Nodes which support skeleton animatables can link to the node via a shared ptr.
	 */
	class SkeletalAnimatable : public Node, public StateAnimatable<Skeleton> {
	private:

		/**
		 * The currently held skeleton offsets
		 */
		std::vector<Transform> skeleton;

		/**
		 * The base state of the skeleton, vertices are rigged to this skeleton
		 */
		Skeleton base;

		/**
		 * Whether or not the skeleton has been recalculated for this frame. This is set to true whenever the skeleton is recalculated
		 * and set to false again every update.
		 */
		bool skeletonRecalculated;

		/**
		 * Mutex used for ensuring safe access to skeleton updates
		 */
		std::mutex mutex;

		/**
		 * Recalculates the skeleton based on the current state composition
		 * @param deltaTime The duration of the last frame in microseconds
		 */
		void recalculateSkeleton(float deltaTime);

	protected:

		/**
		 * Called on update, perfect place to do any physics updating, game logic, etc
		 * @param threadNum the index of the thread this update is called on
		 * @param scene The scene being updated to
		 * @param deltaTime The duration of the last frame in microseconds
		 */
		void update(size_t threadNum, const Scene& scene, float deltaTime) override;

		/**
		 * Called prior to update, perfect place to do things such as updating the bounding box, etc
		 * @param threadNum the index of the thread this update is called on
		 * @param scene The scene being updated to
		 * @param deltaTime The duration of the last frame in microseconds
		 */
		void preUpdate(size_t threadNum, const Scene& scene, float deltaTime) override;

	public:

		/**
		 * Creates an empty skeletal animatable
		 */
		SkeletalAnimatable();

		/**
		 * Creates a skeletal animatable with a base skeleton which all given vertices are weighted by
		 * @param base The base skeleton all transformed vertices are rigged by
		 */
		SkeletalAnimatable(const Skeleton& base);

		/**
		 * Sets the base skeleton transformed vertices are rigged by
		 * @param skeleton The base skeleton transformed vertices are rigged by
		 */
		void setBase(const Skeleton& skeleton);

		/**
		 * Thread-Safe method to get the current skeleton, must be called during a pre-update and cannot be called during the update.
		 * If you need access to the skeleton for transformation/skinning during an update, use getSkeletonNoRecalc().
		 * @param deltaTime The duration of the last frame in microseconds
		 * @returns The skeleton for this frame
		 */
		const std::vector<Transform>& getSkeleton(float deltaTime);

		/**
		 * Thread-Safe method to get the current skeleton, the skeleton will not be recalculated even if it is flagged as out of date.
		 * This method should only be called after pre-updates, otherwise it may not be updated, or worse may update on another thread while
		 * given.
		 * @returns The skeleton as it is
		 */
		const std::vector<Transform>& getSkeletonNoRecalc() const;

		/**
		 * Transforms a single vertex given its weights, cannot be called during the update.
		 * @param vert The vertex to transform
		 * @param weights The weights of the vertex, 4 pairs of the index of the bone along with the significance. Must add up to 1, set the
		 * index of the bone to -1 if it is unassigned.
		 * @param deltaTime The duration of the last frame in microseconds
		 * @returns The transformed vertex
		 */
		Vector2f transform(Vector2f vert, const std::array<std::pair<int, float>, 4>& weights, float deltaTime);

		/**
		 * Transforms a single vertex given its weights. This method should only be called after pre-updates, otherwise it may
		 * not be updated, or worse may update on another thread while given.
		 * @param vert The vertex to transform
		 * @param weights The weights of the vertex, 4 pairs of the index of the bone along with the significance. Must add up to 1, set the
		 * index of the bone to -1 if it is unassigned.
		 * @returns The transformed vertex
		 */
		Vector2f transformNoRecalc(Vector2f vert, const std::array<std::pair<int, float>, 4>& weights) const;

	};

}
