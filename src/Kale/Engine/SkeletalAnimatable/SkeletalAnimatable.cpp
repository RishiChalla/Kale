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

#include "SkeletalAnimatable.hpp"

using namespace Kale;

/**
 * Creates an empty skeleton
 */
Skeleton::Skeleton() {
	// Empty Body
}

/**
 * Creates a skeleton given a vector of tuples containing the index of the previous bone, the bone length, and the bone rotation
 * @param bones The vector of bones
 * @param unit The unit of the rotation given
 */
Skeleton::Skeleton(const std::vector<std::tuple<int, float, float>>& bones, AngleUnit unit) {
	this->bones.reserve(bones.size());
	for (const std::tuple<int, float, float>& bone : bones)
		this->bones.push_back(Skeleton::calculateBone(this->bones, std::get<0>(bone), std::get<1>(bone), std::get<2>(bone), unit));
}

/**
 * Creates a skeleton from a vector of bones
 * @param bones The list of bones
 */
Skeleton::Skeleton(const std::vector<Bone>& bones) : bones(bones) {
	// Empty Body
}

/**
 * Creates an empty skeletal animatable
 */
SkeletalAnimatable::SkeletalAnimatable() {
	// Empty Body
}

/**
 * Creates a skeletal animatable with a base skeleton which all given vertices are weighted by
 * @param base The base skeleton all transformed vertices are rigged by
 */
SkeletalAnimatable::SkeletalAnimatable(const Skeleton& base) : base(base) {
	// Empty Body
}

/**
 * Sets the base skeleton transformed vertices are rigged by
 * @param skeleton The base skeleton transformed vertices are rigged by
 */
void SkeletalAnimatable::setBase(const Skeleton& skeleton) {
	base = skeleton;
}

/**
 * Recalculates the skeleton based on the current state composition
 * @param deltaTime The duration of the last frame in microseconds
 */
void SkeletalAnimatable::recalculateSkeleton(float deltaTime) {
	// Everything below is thread sensitive and modifies data accessed from other threads. We need mutual access to it
	std::lock_guard lock(mutex);

	// If the skeleton has already been recalculated, we don't need to recalculate.
	if (skeletonRecalculated) return;

	// Update the states and retrieve the composition. Also flag the skeleton as recalculated
	updateState(deltaTime);
	skeletonRecalculated = true;
	std::vector<std::pair<int, float>> composition = getStateComposition<int>();

	// Resize the skeleton if it isn't already at the correct size
	if (skeleton.size() != structures[composition[0].first].bones.size())
		skeleton.resize(structures[composition[0].first].bones.size());

	// If the composition size is 1, then the skeleton is simply a single state.
	if (composition.size() == 1) {
		// Simply take the state's worldToBones and multiply it by the inverse of our base
		const std::vector<Skeleton::Bone>& skeletonState = structures[composition[0].first].bones;
		for (size_t i = 0; i < skeleton.size(); i++) skeleton[i] = base.bones[i].inverseWorldToBone * skeletonState[i].worldToBone;
		return;
	}

	// If the composition size is 2, then the skeleton is a lerp between two states
	const std::vector<Skeleton::Bone>& fromSkeletonState = structures[composition[0].first].bones;
	const std::vector<Skeleton::Bone>& toSkeletonState = structures[composition[1].first].bones;
	for (size_t i = 0; i < skeleton.size(); i++) {
		// Lerp the transformation matrices between the two states then multiply it with our inverse to get the offset matrix
		skeleton[i] = fromSkeletonState[i].worldToBone * composition[0].second + toSkeletonState[i].worldToBone * composition[1].second;
		skeleton[i] = base.bones[i].inverseWorldToBone * skeleton[i];
	}
}

/**
 * Called on update, perfect place to do any physics updating, game logic, etc
 * @param threadNum the index of the thread this update is called on
 * @param scene The scene being updated to
 * @param deltaTime The duration of the last frame in microseconds
 */
void SkeletalAnimatable::update(size_t threadNum, const Scene& scene, float deltaTime) {
	skeletonRecalculated = false;
}

/**
 * Called prior to update, perfect place to do things such as updating the bounding box, etc
 * @param threadNum the index of the thread this update is called on
 * @param scene The scene being updated to
 * @param deltaTime The duration of the last frame in microseconds
 */
void SkeletalAnimatable::preUpdate(size_t threadNum, const Scene& scene, float deltaTime) {
	recalculateSkeleton(deltaTime);
}

/**
 * Thread-Safe method to get the current skeleton, must be called during a pre-update and cannot be called during the update.
 * If you need access to the skeleton for transformation/skinning during an update, use getSkeletonNoRecalc().
 * @param deltaTime The duration of the last frame in microseconds
 * @returns The skeleton for this frame
 */
const std::vector<Transform>& SkeletalAnimatable::getSkeleton(float deltaTime) {
	recalculateSkeleton(deltaTime);
	return skeleton;
}

/**
 * Thread-Safe method to get the current skeleton, the skeleton will not be recalculated even if it is flagged as out of date.
 * This method should only be called after pre-updates, otherwise it may not be updated, or worse may update on another thread while
 * given.
 * @returns The skeleton as it is
 */
const std::vector<Transform>& SkeletalAnimatable::getSkeletonNoRecalc() const {
	return skeleton;
}

/**
 * Transforms a single vertex given its weights
 * @param vert The vertex to transform
 * @param weights The weights of the vertex, 4 pairs of the index of the bone along with the significance. Must add up to 1, set the
 * index of the bone to -1 if it is unassigned.
 * @returns The transformed vertex
 */
Vector2f SkeletalAnimatable::transform(Vector2f vert, const std::array<std::pair<int, float>, 4>& weights, float deltaTime) {
	recalculateSkeleton(deltaTime);
	return Skeleton::transform(skeleton, vert, weights);
}

/**
 * Transforms a single vertex given its weights
 * @param vert The vertex to transform
 * @param weights The weights of the vertex, 4 pairs of the index of the bone along with the significance. Must add up to 1, set the
 * index of the bone to -1 if it is unassigned.
 * @returns The transformed vertex
 */
Vector2f SkeletalAnimatable::transformNoRecalc(Vector2f vert, const std::array<std::pair<int, float>, 4>& weights) const {
	return Skeleton::transform(skeleton, vert, weights);
}
