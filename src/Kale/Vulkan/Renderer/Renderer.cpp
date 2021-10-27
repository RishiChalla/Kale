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

#include "Renderer.hpp"

#include <Kale/Vulkan/QueueFamilyIndices/QueueFamilyIndices.hpp>
#include <Kale/Vulkan/Extensions/Extensions.hpp>
#include <Kale/Vulkan/SwapChainSupportDetails/SwapChainSupportDetails.hpp>
#include <stdexcept>
#include <algorithm>

using namespace Kale;
using namespace Kale::Vulkan;

Renderer Kale::Vulkan::renderer;

/**
 * Gets the GPU information of the physical device currently being used for rendering
 * @returns The GPU being used for rendering
 */
const Device& Renderer::getGPU() const {
	return device;
}

/**
 * Uses a specific GPU given the ID
 * @param gpuID The id of the GPU to use
 * @throws If the GPU wasn't found
 */
void Renderer::useGPU(uint32_t gpuID) {
	device = Device(gpuID);
}

/**
 * Uses a specific GPU given the physical device
 * @param pdevice The physical device
 */
void Renderer::useGPU(const vk::PhysicalDevice& pdevice) {
	device = Device(pdevice);
}