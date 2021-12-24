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

#include <Kale/Core/Logger/Logger.hpp>

#include <list>

namespace Kale::Vulkan {

	/**
	 * Forward declaration of child resource
	 */
	template <typename Parent> class ChildResource;

	/**
	 * Represents a single parent resource which manages several child resources
	 */
	template <typename Parent> class ParentResource {
	protected:

		/**
		 * All child resources being managed by this parent resource
		 */
		std::list<ChildResource<Parent>*> resources;

		friend class ChildResource<Parent>;
	public:

		/**
		 * Frees all children resources
		 */
		virtual ~ParentResource() {
			freeChildren();
		}

		/**
		 * Frees all children resources
		 */
		void freeChildren() {
			for (ChildResource<Parent>* resource : resources) {
				try {
					resource->freeResources(false);
				}
				catch (const std::exception& e) {
					console.error(e.what());
				}
			}
			resources.clear();
		}
	};
}

#endif
