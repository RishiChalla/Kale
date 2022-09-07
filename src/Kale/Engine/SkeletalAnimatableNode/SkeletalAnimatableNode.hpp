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

#include <Kale/Engine/Node/Node.hpp>
#include <Kale/Engine/StateAnimatableNode/StateAnimatableNode.hpp>
#include <Kale/Math/Transform/Transform.hpp>
#include <Kale/Math/Line/Line.hpp>
#include <Kale/Math/Utils/Utils.hpp>

#include <memory>
#include <vector>
#include <utility>

namespace Kale {

	/**
	 * Represents a single bone in a skeleton for skeletal animation
	 */
	struct Bone {
	private:

		/**
		 * Transformation matrix for converting directly from the base to this bone
		 */
		Transform baseToBone;

		/**
		 * Transformation matrix for converting from the previous bone to this one
		 */
		Transform previousToBone;

		/**
		 * The points of this bone, point1 is always the joint with the previous bone. point2 is the joint
		 * with all connectedBones.
		 */
		Line bone;

		/**
		 * A vector of connected bones, all connected bones extend from this one's transform and use point2 as a joint.
		 */
		std::vector<std::shared_ptr<Bone>> connectedBones;
	
	public:

		Transform getBaseToBone() const;
		Transform getPreviousToBone() const;
		Line getBone() const;
		const std::vector<std::shared_ptr<Bone>>& getConnectedBones() const;

		/**
		 * Adds a new bone attached to this one using point2 as a joint
		 * *NOTE THREAD SAFE* - Usage to this must be externally synchronized.
		 * @param angle The angle of rotation for the new bone from this one
		 * @param unit The unit of the angle
		 * @param length The length of the new bone
		 */
		void addBone(float angle, AngleUnit unit, float length);

		/**
		 * Rotates this bone and all bones connected to this one
		 * *NOTE THREAD SAFE* - Usage to this must be externally synchronized.
		 * @param angle The angle of rotation
		 * @param unit The unit of the angle
		 */
		void rotate(float angle, AngleUnit unit);

		/**
		 * Sets the rotation of this bone, and updates the positions of all connected bones.
		 * *NOTE THREAD SAFE* - Usage to this must be externally synchronized.
		 * @param angle The angle of rotation
		 * @param unit The unit of the angle
		 */
		void setRotation(float angle, AngleUnit unit);
	};

	/**
	 * Represents a skeleton for skeletal animation
	 */
	struct Skeleton {
	private:

		/**
		 * The base bone of the skeleton, all other bones are connected to this one
		 */
		Line baseBone;

		/**
		 * All bones with a joint on point1
		 */
		std::vector<std::shared_ptr<Bone>> leftJointBones;

		/**
		 * All bones with a joint on point2
		 */
		std::vector<std::shared_ptr<Bone>> rightJointBones;
	
	public:

		/**
		 * The base transform of the skeleton, changing this effects all bones
		 */
		Transform skeletonTransform;

		/**
		 * Gets the base bone of the skeleton
		 */
		Line getBaseBone() const;

		/**
		 * Gets the bones connected to the right joint (point1)
		 * @returns all bones connected to the right joint (point1)
		 */
		const std::vector<std::shared_ptr<Bone>>& getLeftJointBones() const;

		/**
		 * Gets the bones connected to the right joint (point2)
		 * @returns all bones connected to the right joint (point2)
		 */
		const std::vector<std::shared_ptr<Bone>>& getRightJointBones() const;

		/**
		 * Calculates the bone weights for a given vertex. These weights are required to allow the vertex to be
		 * "assigned" to bones when rigging.
		 * This method is rather heavy to execute on the CPU for a large amount of vertices and should not be called during an actual
		 * game's loading time. Instead use this method to precalculate weights then save them to an asset file.
		 * @param vertex The vertex to create weights for
		 * @param threshold The ratio at which to cutoff a second bone's influence. The ratio is the closest/n, anything smaller
		 * will be cut off. For example 0.5 means to disclude bones twice as far from the vertex than the first.
		 * @returns A vector of each assigned bone and its weight. The sum of weights is 1, the maimum number of bones are 2.
		 */
		std::vector<std::pair<std::weak_ptr<Bone>, float>> calculateWeights(Vector2f vertex, float threshold) const;

		/**
		 * Transforms a vertex given its weights. This method runs on the CPU and is rather heavy when done on many vertices.
		 * If possible, instead use the skeletal animatable shaders for calculating this on the GPU.
		 * @param vertex The vertex to transform
		 * @param weights The weights for the vertex
		 * @returns The transformed vertex
		 */
		Vector2f transformVertex(Vector2f vertex, const std::vector<std::pair<std::weak_ptr<Bone>, float>>& weights) const;

		/**
		 * Creates a new skeleton lerped between this one and another.
		 * @param other The other skeleton to lerp between
		 * @param significance A number between 0 and 1 determining the significance of this skeleton. The other skeleton will have
		 * significance (1 - significance).
		 * @returns A new skeleton constructed from the linear interpolation.
		 */
		Skeleton lerp(const Skeleton& other, float significance) const;

		/**
		 * Adds a new bone attached to this one using point1 as a joint
		 * *NOTE THREAD SAFE* - Usage to this must be externally synchronized.
		 * @param angle The angle of rotation for the new bone from this one
		 * @param unit The unit of the angle
		 * @param length The length of the new bone
		 */
		void addLeftJointBone(float angle, AngleUnit unit, float length);

		/**
		 * Adds a new bone attached to this one using point2 as a joint
		 * *NOTE THREAD SAFE* - Usage to this must be externally synchronized.
		 * @param angle The angle of rotation for the new bone from this one
		 * @param unit The unit of the angle
		 * @param length The length of the new bone
		 */
		void addRightJointBone(float angle, AngleUnit unit, float length);

	};

	/**
	 * A node capable of skeletal based animations. A single skeletal animatable node can only be used by a single node.
	 * This can be used to make unique "warp" effects which are otherwise impossible.
	 * 
	 * Please see the SkeletonNode if you are looking for a skeletal system capable of connecting several node's transforms.
	 */
	class SkeletalAnimatableNode : public Node, public FiniteStateAnimatable<Skeleton> {
	private:

		/**
		 * The skeleton
		 */
		Skeleton currentSkeleton;
	
	protected:

		/**
		 * Called prior to update, perfect place to do things such as updating the bounding box, etc
		 * @param threadNum the index of the thread this update is called on
		 * @param scene The scene being updated to
		 * @param deltaTime The duration of the last frame in microseconds
		 */
		virtual void preUpdate(size_t threadNum, const Scene& scene, float deltaTime);

	public:
		
		/**
		 * Gets the skeleton used for this frame
		 * @returns The skeleton
		 */
		const Skeleton& getSkeleton() const;

		/**
		 * Gets a modifyable version of the skeleton.
		 * *NOT THREAD SAFE* - It is the responsibility of the caller to synchronize this
		 * @throws If called when stateAnimationNode is not nullptr on debug
		 * @returns The skeleton
		 */
		Skeleton& skeleton();

	};
	
}
