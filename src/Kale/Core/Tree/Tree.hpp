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

#include <list>
#include <iterator>

namespace Kale {

	/**
	 * Represents a tree based data structure
	 * @tparam T The structure to be holding in each tree leaf node
	 */
	template <typename T> class Tree {
	public:

		/**
		 * Represents a single child in the tree
		 */
		struct Child {
		private:

			/**
			 * A pointer to the next node at the same vertical level, or nullptr if this is the last
			 */
			Child* next = nullptr;

			/**
			 * A pointer to the previous node at the same vertical level, or nullptr if this is the first
			 */
			Child* previous = nullptr;

			/**
			 * A pointer to the parent of this node, a vertical level below this node.
			 */
			Child* parent = nullptr;

			/**
			 * The tree this child belongs to
			 */
			Tree& tree;

			/**
			 * A list of the children of this branch of the tree, a vertical level above this node
			 */
			std::list<Child> children;

			/**
			 * The horizontal index of this node, horizontal indices start at zero and increment until next is nullptr
			 */
			size_t horizontal;

			/**
			 * The vertical index of this node, the root is 0 and it increments as you loop through more children
			 */
			size_t vertical;

			/**
			 * The value held within this tree node
			 */
			T value;

			/**
			 * Sets up the next/previous and updates surrounding tree children
			 */
			void setup() {
				// Create a pointer/iterator to use for misc searching purposes
				Child* it = nullptr;
				
				// Find the next child
				it = parent->next;
				while (it != nullptr) {
					if (!it->children.empty()) {
						next = &it->children.front();
						break;
					}
					it = it->next;
				}

				// Find the previous child
				if (next != nullptr) previous = next->previous;
				else {
					it = parent;
					while (it != nullptr) {
						if (!it->children.empty()) {
							previous = &it->children.back();
							break;
						}
						it = it->previous;
					}
				}

				// Update the next/previous of surrounding children
				if (next != nullptr) next->previous = this;
				if (previous != nullptr) previous->next = this;

				// Set the horizontal & vertical indices based on the next/previous/parent
				horizontal = previous != nullptr ? previous->getHorizontalIndex() + 1 : 0;
				vertical = parent->vertical + 1;

				// Update the back if applicable
				if (vertical > tree.backPtr->vertical || (vertical == tree.backPtr->vertical && horizontal > tree.backPtr->horizontal))
					tree.backPtr = this;

				// All children on the same vertical level with greater horizontal indices need to be updated
				it = next;
				while (it != nullptr) {
					it->horizontal++;
					it = it->next;
				}
			}

			friend class Tree<T>;

		public:

			/**
			 * Constructs a new child at the tree root
			 * @note Do not call the constructor manually, instead use addChild.
			 * @param tree The tree this child belongs to
			 */
			Child(Tree& tree) : tree(tree), horizontal(0), vertical(0) {}

			/**
			 * Constructs a new child at the root of a tree given a value
			 * @note Do not call the constructor manually, instead use addChild.
			 * @param tree The tree this child belongs to
			 * @param value The value to initialize the root to
			 */
			Child(Tree& tree, const T& value) : tree(tree), horizontal(0), vertical(0), value(value) {}

			/**
			 * Constructs a new child given a parent using the value default constructor
			 * @note Do not call the constructor manually, instead use addChild.
			 * @param parent The parent of this node
			 */
			Child(Child& parent) : tree(parent.tree), parent(&parent) {
				setup();
			}

			/**
			 * Constructs a new child given a parent and a given value
			 * @note Do not call the constructor manually, instead use addChild.
			 * @param parent The parent
			 * @param value The value to set the node to
			 */
			Child(Child& parent, const T& value) : tree(parent.tree), parent(&parent), value(value) {
				setup();
			}

			/**
			 * Does cleanup and updates indices
			 */
			~Child() {
				if (previous != nullptr) previous->next = next;
				if (next != nullptr) next->previous = previous;

				Child* it = next;
				while (it != nullptr) {
					it->horizontal--;
					it = it->next;
				}
			}

			/**
			 * Adds a child node under this child node using the value's default constructor
			 */
			Child& addChild() {
				return children.emplace_back(*this);
			}

			/**
			 * Adds a child node under this child node
			 * @param value The value to set the child to
			 */
			Child& addChild(const T& value) {
				return children.emplace_back(*this, value);
			}

			/**
			 * Removes a child node from this branch
			 * @param horizontal The horizontal index of the child
			 */
			void removeChild(size_t horizontal) {
				children.remove_if([&](const Child& child) { return child.getHorizontalIndex() == horizontal; });
			}
			
			/**
			 * Allows for accessing the held value's methods or members
			 * @returns A pointer to the value
			 */
			const T* operator->() const {
				return &value;
			}

			/**
			 * Allows for accessing the held value's methods or members
			 * @returns A pointer to the value
			 */
			T* operator->() {
				return &value;
			}

			/**
			 * Allows for accessing the held value's methods or members
			 * @returns A reference to the value
			 */
			const T& getValue() const {
				return value;
			}

			/**
			 * Allows for accessing the held value's methods or members
			 * @returns A reference to the value
			 */
			T& getValue() {
				return value;
			}
			
			/**
			 * Gets a pointer to the next node at the same vertical level, or nullptr if this is the last
			 * @returns A pointer to the next node at the same vertical level, or nullptr if this is the last
			 */
			const Child* getNext() const {
				return next;
			}

			/**
			 * Gets a pointer to the next node at the same vertical level, or nullptr if this is the last
			 * @returns A pointer to the next node at the same vertical level, or nullptr if this is the last
			 */
			Child* getNext() {
				return next;
			}

			/**
			 * Gets a pointer to the previous node at the same vertical level, or nullptr if this is the first
			 * @returns A pointer to the previous node at the same vertical level, or nullptr if this is the first
			 */
			const Child* getPrevious() const {
				return previous;
			}

			/**
			 * Gets a pointer to the previous node at the same vertical level, or nullptr if this is the first
			 * @returns A pointer to the previous node at the same vertical level, or nullptr if this is the first
			 */
			Child* getPrevious() {
				return previous;
			}

			/**
			 * Gets a pointer to the parent of this node, a vertical level below this node.
			 * @returns A pointer to the parent of this node, a vertical level below this node.
			 */
			const Child* getParent() const {
				return parent;
			}

			/**
			 * Gets a pointer to the parent of this node, a vertical level below this node.
			 * @returns A pointer to the parent of this node, a vertical level below this node.
			 */
			Child* getParent() {
				return parent;
			}

			/**
			 * Gets the horizontal index of this node, horizontal indices start at zero and increment until next is nullptr
			 * @returns The horizontal index of this node, horizontal indices start at zero and increment until next is nullptr
			 */
			size_t getHorizontalIndex() const {
				return horizontal;
			}

			/**
			 * Gets the vertical index of this node, the root is 0 and it increments as you loop through more children
			 * @returns The vertical index of this node, the root is 0 and it increments as you loop through more children
			 */
			size_t getVerticalIndex() const {
				return vertical;
			}

			/**
			 * Gets a list of the children of this branch of the tree, a vertical level above this node
			 * @returns A list of the children of this branch of the tree, a vertical level above this node
			 */
			const std::list<Child>& getChildren() const {
				return children;
			}

			/**
			 * Gets a list of the children of this branch of the tree, a vertical level above this node
			 * @returns A list of the children of this branch of the tree, a vertical level above this node
			 */
			std::list<Child>& getChildren() {
				return children;
			}

		};

		/**
		 * Custom iterator for iterating through trees. Attempts to iterate horizontally then vertically
		 * @tparam S The structure held within the iterator (either T or const T)
		 */
		template <typename S> struct RawIterator {
		private:

			/**
			 * A pointer to the held node
			 */
			S* ptr;

			/**
			 * A reference to the tree
			 */
			const Tree& tree;

		public:

			// Required information for custom iterators
			using iterator_category = std::bidirectional_iterator_tag;
			using value_type = S;
			using pointer = S*;
			using reference = S&;

			/**
			 * Constructs a new raw iterator given a pointer
			 * @param tree The tree this iterator belongs to
			 * @param ptr The pointer
			 */
			RawIterator(const Tree& tree, S* ptr = nullptr) : tree(tree), ptr(ptr) {}

			/**
			 * Assigns the iterator to another pointer
			 * @param ptr The pointer to assign to
			 * @returns A reference of this
			 */
			RawIterator<S>& operator=(S* ptr) { this->ptr = ptr; }

			/**
			 * Returns whether or not the pointer is invalid
			 * @returns whether or not the pointer is invalid
			 */
			operator bool() const { return ptr != nullptr; }

			/**
			 * Checks for equality with another iterator
			 * @param it The other iterator
			 * @returns True when equal, false when not equal
			 */
			bool operator==(const RawIterator<S> it) const { return it.ptr == ptr; }

			/**
			 * Checks for inequality with another iterator
			 * @param it The other iterator
			 * @returns True when unequal, false when equal
			 */
			bool operator!=(const RawIterator<S> it) const { return it.ptr != ptr; }

			/**
			 * Increments the iterator to continue to the next iteration
			 * @returns A reference to this (after increment)
			 */
			RawIterator<S>& operator++() {
				// The next isn't nullptr, we simply move on to the next in the same horizontal index
				if (ptr->getNext() != nullptr) {
					ptr = ptr->getNext();
					return *this;
				}

				// We need to move to the next vertical index, keep searching the previous nodes for children
				// If found, set it so we get the earliest one possible.
				S* it = ptr;
				ptr = nullptr;
				while (it != nullptr) {
					if (!it->getChildren().empty()) ptr = &it->getChildren().front();
					it = it->getPrevious();
				}

				// If ptr is nullptr, it means we've finished looping (this iterator is now the end)
				return *this;
			}

			/**
			 * Decrements the iterator to continue to the previous iteration
			 * @returns A reference to this (after decrement)
			 */
			RawIterator<S>& operator--() {
				if (ptr == nullptr) {
					ptr = tree.backPtr;
					return *this;
				}

				// This is the root, it can't be decremented
				if (ptr->getParent() == nullptr) return *this;

				// The previous isn't nullptr, we simply move on to the previous in the same horizontal index
				if (ptr->getPrevious() != nullptr) {
					ptr = ptr->getPrevious();
					return *this;
				}

				// We need to move to the previous vertical index, go to our parent then go to the last index possible
				ptr = ptr->getParent();
				while (ptr->getNext() != nullptr) ptr = ptr->getNext();

				// If ptr is nullptr, it means we've finished looping (this iterator is now the beginning)
				return *this;
			}

			/**
			 * Increments the iterator to continue to the next iteration
			 * @returns A copy of the iterator prior to the increment
			 */
			RawIterator<S> operator++(int) {
				RawIterator<S> it = *this;
				++(*this);
				return it;
			}

			/**
			 * Decrements the iterator to continue to the previous iteration
			 * @returns A copy of the iterator prior to the decrement
			 */
			RawIterator<S> operator--(int) {
				RawIterator<S> it = *this;
				--(*this);
				return it;
			}

			/**
			 * Dereferences the iterator
			 * @returns A reference to the child
			 */
			S& operator*() {
				return *ptr;
			}

			/**
			 * Dereferences the iterator
			 * @returns A reference to the child
			 */
			const S& operator*() const {
				return *ptr;
			}

			/**
			 * Allows for access to the child's methods and members
			 * @returns A pointer to the child
			 */
			S* operator->() {
				return ptr;
			}

			/**
			 * Allows for access to the child's methods and members
			 * @returns A pointer to the child
			 */
			const S* operator->() const {
				return ptr;
			}

			/**
			 * Gets a pointer to the child
			 * @returns A pointer to the child
			 */
			S* getPtr() const {
				return ptr;
			}

			/**
			 * Gets a constant pointer to the child
			 * @return A constant pointer to the child 
			 */
			const S* getConstPtr() const {
				return ptr;
			}
		};

	private:

		/**
		 * Pointer to the object at the back of the tree (the highest horizontal & vertical indices)
		 */
		Child* backPtr;

		friend struct Child;
		friend struct RawIterator<Child>;
		friend struct RawIterator<const Child>;

	public:

		/**
		 * Custom iterator used for traversing trees with mutable elements
		 */
		using Iterator = RawIterator<Child>;

		/**
		 * Custom iterator used for traversing trees with immutable elements
		 */
		using ConstIterator = RawIterator<const Child>;

		/**
		 * The root child of the tree, all other children stem from this one
		 */
		Child root;

		/**
		 * Creates a new tree with a root using the default constructor of the held object
		 */
		Tree() : root(*this), backPtr(&root) {}

		/**
		 * Creates a new tree with a root with an initialized value
		 * @param value The value to initialize the root with
		 */
		Tree(const T& value) : root(*this, value), backPtr(&root) {}

		/**
		 * Gets the root as an iterator to allow for traversal of the tree
		 * @returns The root as an iterator to allow for traversal of the tree
		 */
		Iterator begin() {
			return Iterator(*this, &root);
		}

		/**
		 * Gets the root as a const iterator to allow for traversal of the tree
		 * @returns The root as a const iterator to allow for traversal of the tree
		 */
		ConstIterator cbegin() const {
			return ConstIterator(*this, &root);
		}

		/**
		 * Gets an invalid iterator after the last child to allow for traversal of the tree
		 * @returns An invalid iterator after the last child to allow for traversal of the tree
		 */
		Iterator end() {
			return Iterator(*this, nullptr);
		}

		/**
		 * Gets an invalid constant iterator after the last child to allow for traversal of the tree
		 * @returns An invalid constant iterator after the last child to allow for traversal of the tree
		 */
		ConstIterator cend() const {
			return ConstIterator(*this, nullptr);
		}

		
		/**
		 * Gets the root of the tree
		 * @returns The root of the tree
		 */
		Child& front() {
			return root;
		}
		
		/**
		 * Gets the root of the tree
		 * @returns The root of the tree
		 */
		const Child& front() const {
			return root;
		}
		
		/**
		 * Gets the bottom right element of the tree (the last item iterated usually)
		 * @returns the bottom right element of the tree (the last item iterated)
		 */
		Child& back() {
			return *backPtr;
		}
		
		/**
		 * Gets the bottom right element of the tree (the last item iterated usually)
		 * @returns the bottom right element of the tree (the last item iterated)
		 */
		const Child& back() const {
			return *backPtr;
		}

		/**
		 * Accesses a child element given the horizontal and vertical indices of the child
		 * @param horizontal The horizontal index of the child
		 * @param vertical The vertical index of the child
		 * @returns The child
		 */
		const Child& operator()(size_t horizontal, size_t vertical) const {
			return *std::find_if(cbegin(), cend(), [&](const Child& child) {
				return child.getHorizontalIndex() == horizontal && child.getVerticalIndex() == vertical;
			});
		}

		/**
		 * Accesses a child element given the horizontal and vertical indices of the child
		 * @param horizontal The horizontal index of the child
		 * @param vertical The vertical index of the child
		 * @returns The child
		 */
		Child& operator()(size_t horizontal, size_t vertical) {
			return *std::find_if(begin(), end(), [&](const Child& child) {
				return child.getHorizontalIndex() == horizontal && child.getVerticalIndex() == vertical;
			});
		}

	};

}
