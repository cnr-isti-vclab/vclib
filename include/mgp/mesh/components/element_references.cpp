/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "element_references.h"

#include <algorithm>

namespace mgp::comp {

template<class Elem, int N>
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

template<class Elem, int N>
void ElementReferences<Elem, N>::updateElementReferences(const Elem* oldBase, const Elem* newBase)
{
	for (unsigned int j = 0; j < Base::size(); ++j) {
		if (Base::at(j) != nullptr) {
			size_t diff = Base::at(j) - oldBase;
			Base::at(j)  = (Elem*) newBase + diff;
		}
	}
}

template<class Elem, int N>
void ElementReferences<Elem, N>::updateElementReferencesAfterCompact(
	const Elem*             base,
	const std::vector<int>& newIndices)
{
	for (unsigned int j = 0; j < Base::size(); ++j) {
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

} // namespace mgp::comp
