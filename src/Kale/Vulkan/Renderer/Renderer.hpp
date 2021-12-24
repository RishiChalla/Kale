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

#ifdef KALE_VULKAN

#include <Kale/Vulkan/ChildResource/ChildResource.hpp>
#include <Kale/Vulkan/ParentResource/ParentResource.hpp>
#include <Kale/Vulkan/Device/Device.hpp>
#include <Kale/Vulkan/SwapChain/SwapChain.hpp>

#include <vulkan/vulkan.hpp>

namespace Kale::Vulkan {

	/**
	 * The main renderer class for the game, each scene uses this internally.
	 */
	class Renderer : public ParentResource<Renderer>, public ChildResource<SwapChain> {
	private:

		/**
		 * Creates the render pass
		 */
		void createRenderPass();

// Used for assertion that frame buffers created with renderers have the appropriate swapchain parent
#ifdef KALE_DEBUG
	protected:

		/**
		 * Gets the swapchain for debug assertions
		 */
		const SwapChain* getSwapChain() const;

		friend class FrameBuffer;

#endif

	public:

		/**
		 * The main render pass for this renderer
		 */
		vk::UniqueRenderPass renderPass;

		/**
		 * Creates an uninstantiated renderer
		 */
		Renderer();

		/**
		 * Initializes the object
		 */
		void init();

		/**
		 * Frees resources if not already freed
		 */
		void freeResources(bool remove = true) override;

	};
}

#endif
