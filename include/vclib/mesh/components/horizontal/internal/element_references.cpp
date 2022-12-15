/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include "element_references.h"

#include <algorithm>

namespace vcl::comp::internal {

/*
 * Create a container of Element references (pointers to Elements stored in some other container).
 * If this Container is a static array, all its element will be initialized to nullptr.
 * If this Container is a dynamic vector, it will be an empty container.
 */
template<typename Elem, int N>
ElementReferences<Elem, N>::ElementReferences() : Base()
{
	if constexpr (N >= 0) {
		// I'll use the array, N is >= 0.
		// There will be a static number of references.
		Base::container = std::array<Elem*, N> {nullptr};
	}
	else {
		// I'll use the vector, because N is < 0.
		// There will be a dynamic number of references.
		Base::container = std::vector<Elem*>();
	}
}

/*
 * This member function is called when we need to update the references in this container.
 *
 * This is necessary when, for example, the original container of Elements has been reallocated.
 * When this happens, the all the Elements have been moved in another portion of memory, and
 * all the pointers to that Elements must be updated. Since in this container are stored pointers
 * to Elements, we need to update them.
 *
 * To update them, we need to know the oldBase (the pointer to the first Element of the reallocated
 * Container before the reallocation) and the newBase (the pointer to the first Element of the
 * reallocated Container after the reallocation. We can then compute, for each pointer, the offset
 * w.r.t. the first element of the Container, and update the the pointer accordingly using the
 * newBase.
 */
template<typename Elem, int N>
void ElementReferences<Elem, N>::updateElementReferences(const Elem* oldBase, const Elem* newBase)
{
	for (uint j = 0; j < Base::size(); ++j) { // for each pointer in this container
		if (Base::at(j) != nullptr) {
			size_t diff = Base::at(j) - oldBase; // offset w.r.t. the old base
			Base::at(j)  = (Elem*) newBase + diff; // update the pointer using newBase
		}
	}
}

template<typename Elem, int N>
void ElementReferences<Elem, N>::updateElementReferencesAfterCompact(
	const Elem*             base,
	const std::vector<int>& newIndices)
{
	for (uint j = 0; j < Base::size(); ++j) {
		if (Base::at(j) != nullptr) {
			size_t diff = Base::at(j) - base;
			if (newIndices[diff] < 0) { // element has been removed
				Base::at(j) = nullptr;
			}
			else { // the new pointer will be base + newIndices[diff]
				Base::at(j) = (Elem*) base + newIndices[diff];
			}
		}
	}
}

} // namespace vcl::comp::internal
