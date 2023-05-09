/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#include "element_pointers_container.h"

#include <algorithm>

namespace vcl::comp {

/*
 * Create a container of Element pointers (pointers to Elements stored in some other container).
 * If this Container is a static array, all its element will be initialized to nullptr.
 * If this Container is a dynamic vector, it will be an empty container.
 */
template<typename Elem, int N, typename El, bool o>
ElementPointersContainer<Elem, N, El, o>::ElementPointersContainer()
{
	if constexpr (!Base::IS_VERTICAL) {
		if constexpr (N >= 0) {
			Base::data((void*)nullptr).fill(nullptr);
		}
	}
}

/*
 * Create a container of Element pointers (pointers to Elements stored in some other container).
 * If this Container is a static array, all its element will be initialized to nullptr.
 * If this Container is a dynamic vector, it will be an empty container.
 */
template<typename Elem, int N, typename El, bool o>
template<typename Comp>
void ElementPointersContainer<Elem, N, El, o>::init(Comp* comp)
{
	if constexpr (N >= 0) {
		// I'll use the array, N is >= 0.
		// There will be a static number of pointers.
		container(comp).fill(nullptr);
	}
	else {
		// I'll use the vector, because N is < 0.
		// There will be a dynamic number of pointers.
		container(comp).clear();
	}
}

/*
 * This member function is called when we need to update the pointers in this container.
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
template<typename Elem, int N, typename El, bool o>
template<typename Comp>
void ElementPointersContainer<Elem, N, El, o>::updateElementPointers(
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

template<typename Elem, int N, typename El, bool o>
template<typename Comp>
void ElementPointersContainer<Elem, N, El, o>::updateElementPointersAfterCompact(
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

template<typename Elem, int N, typename El, bool o>
template<typename Comp>
Vector<Elem*, N>& ElementPointersContainer<Elem, N, El, o>::container(Comp* comp)
{
	return Base::data(comp);
}

template<typename Elem, int N, typename El, bool o>
template<typename Comp>
const Vector<Elem*, N>& ElementPointersContainer<Elem, N, El, o>::container(const Comp* comp) const
{
	return Base::data(comp);
}

} // namespace vcl::comp
