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

namespace Kale::Vulkan {

	/**
	 * Forward declaration of parent class
	 */
	class ParentResource;

	/**
	 * Represents a single child resource, all child resources must inherit from this class
	 */
	class ChildResource {
	protected:

		/**
		 * Creates an uninitialized resource, if this constructor is used then
		 * the setup method MUST be called if the derived object is ever created
		 */
		ChildResource();

		/**
		 * Adds a child resource to the list of child resources to be managed
		 * @param parent The parent resource managing this resource
		 */
		ChildResource(ParentResource& parent);

		/**
		 * Adds a child resource to the list of child resources to be managed
		 * @param parent The parent resource managing this resource
		 */
		virtual void init(ParentResource& parent);

		/**
		 * Removes the child resource from the list of child resources to be managed
		 */
		~ChildResource();

		/**
		 * The pointer to the parent
		 */
		ParentResource* parentPtr = nullptr;

		friend class ParentResource;

	public:

		/**
		 * Frees resources if not already freed
		 * @param remove Whether or not to remove this from the list
		 */
		virtual void freeResources(bool remove = true);
	};
}