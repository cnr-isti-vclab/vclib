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

template<typename Elem, int N>
void ElementReferences<Elem, N>::updateElementReferences(const Elem* oldBase, const Elem* newBase)
{
	for (uint j = 0; j < Base::size(); ++j) {
		if (Base::at(j) != nullptr) {
			size_t diff = Base::at(j) - oldBase;
			Base::at(j)  = (Elem*) newBase + diff;
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
