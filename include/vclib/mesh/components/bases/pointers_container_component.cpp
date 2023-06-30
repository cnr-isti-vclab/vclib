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

#include "pointers_container_component.h"

#include <algorithm>

namespace vcl::comp {

/*
 * This member function is called when we need to update the pointers in this
 * container after a reallocation (the pointer of the first element of the
 * container is changed from oldBase to newBase).
 *
 * This is necessary when, for example, the original container of Elements has
 * been reallocated. When this happens, the all the Elements have been moved in
 * another portion of memory, and all the pointers to that Elements must be
 * updated. Since in this container are stored pointers to Elements, we need to
 * update them.
 *
 * To update them, we need to know the oldBase (the pointer to the first Element
 * of the reallocated Container before the reallocation) and the newBase (the
 * pointer to the first Element of the reallocated Container after the
 * reallocation. We can then compute, for each pointer, the offset w.r.t. the
 * first element of the Container, and update the the pointer accordingly using
 * the newBase.
 */
template<uint CT, typename Elem, int N, typename El, bool o, bool TT>
template<typename Comp>
void PointersContainerComponent<CT, Elem, N, El, o, TT>::updateElementPointers(
	const Elem* oldBase,
	const Elem* newBase,
	Comp*       comp)
{
	for (uint j = 0; j < container(comp).size(); ++j)
	{ // for each pointer in this container
		if (container(comp).at(j) != nullptr) {
			size_t diff =
				container(comp).at(j) - oldBase; // offset w.r.t. the old base
			container(comp).at(j) =
				(Elem*) newBase + diff; // update the pointer using newBase
		}
	}
}

/*
 * This member function is called when we need to update the pointers in this
 * containers, usually after a compaction of the container (but not always).
 *
 * In this case, the address of the first element in the container is not
 * changed, but may change the position of each element inside the container.
 * The function takes the base pointer of the first element of the container,
 * and a vector that stores, for each old element position, the new position
 * in the container (UINT_NULL if the element has been removed and must be
 * left unreferenced).
 */
template<uint CT, typename Elem, int N, typename El, bool o, bool TT>
template<typename Comp>
void PointersContainerComponent<CT, Elem, N, El, o, TT>::updateElementPointers(
	const Elem*              base,
	const std::vector<uint>& newIndices,
	Comp*                    comp)
{
	for (uint j = 0; j < container(comp).size(); ++j) {
		if (container(comp).at(j) != nullptr) {
			size_t diff = container(comp).at(j) - base;
			if (newIndices[diff] == UINT_NULL) { // element has been removed
				container(comp).at(j) = nullptr;
			}
			else { // the new pointer will be base + newIndices[diff]
				container(comp).at(j) = (Elem*) base + newIndices[diff];
			}
		}
	}
}

} // namespace vcl::comp
