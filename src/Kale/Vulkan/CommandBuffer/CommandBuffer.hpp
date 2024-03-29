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

#ifdef KALE_VULKAN

#include <Kale/Vulkan/FrameBuffer/FrameBuffer.hpp>
#include <Kale/Vulkan/ChildResource/ChildResource.hpp>

#include <vulkan/vulkan.hpp>

namespace Kale::Vulkan {

	/**
	 * The command buffer class used for draw commands
	 */
	class CommandBuffer : public ChildResource<FrameBuffer> {
	private:

	public:

		/**
		 * The command buffer object
		 */
		vk::UniqueCommandBuffer commandBuffer;

	};
}

#endif
