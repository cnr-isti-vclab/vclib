/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
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
template<typename Elem, int N, typename El>
template<typename Comp>
void ElementReferences<Elem, N, El>::init(Comp* comp)
{
	if constexpr (N >= 0) {
		// I'll use the array, N is >= 0.
		// There will be a static number of references.
		container(comp).fill(nullptr);
	}
	else {
		// I'll use the vector, because N is < 0.
		// There will be a dynamic number of references.
		container(comp).clear();
	}
}

template<typename Elem, int N, typename El>
template<typename Comp>
bool ElementReferences<Elem, N, El>::isEnabled(Comp* comp)
{
	return data.template isComponentEnabled<El>(comp);
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
template<typename Elem, int N, typename El>
template<typename Comp>
void ElementReferences<Elem, N, El>::updateElementReferences(
	const Elem* oldBase,
	const Elem* newBase,
	Comp* comp)
{
	for (uint j = 0; j < container(comp).size(); ++j) { // for each pointer in this container
		if (container(comp).at(j) != nullptr) {
			size_t diff = container(comp).at(j) - oldBase; // offset w.r.t. the old base
			container(comp).at(j)  = (Elem*) newBase + diff; // update the pointer using newBase
		}
	}
}

template<typename Elem, int N, typename El>
template<typename Comp>
void ElementReferences<Elem, N, El>::updateElementReferencesAfterCompact(
	const Elem*             base,
	const std::vector<int>& newIndices,
	Comp* comp)
{
	for (uint j = 0; j < container(comp).size(); ++j) {
		if (container(comp).at(j) != nullptr) {
			size_t diff = container(comp).at(j) - base;
			if (newIndices[diff] < 0) { // element has been removed
				container(comp).at(j) = nullptr;
			}
			else { // the new pointer will be base + newIndices[diff]
				container(comp).at(j) = (Elem*) base + newIndices[diff];
			}
		}
	}
}

template<typename Elem, int N, typename El>
template<typename Comp>
RandomAccessContainer<Elem*, N>& ElementReferences<Elem, N, El>::container(Comp* comp)
{
	return data.template get<El>(comp);
}

template<typename Elem, int N, typename El>
template<typename Comp>
const RandomAccessContainer<Elem*, N>& ElementReferences<Elem, N, El>::container(const Comp* comp) const
{
	return data.template get<El>(comp);
}

} // namespace vcl::comp::internal
