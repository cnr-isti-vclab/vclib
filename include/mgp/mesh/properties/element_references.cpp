/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "element_references.h"

#include <algorithm>

namespace mgp::prop {

template<class Elem, int N>
ElementReferences<Elem, N>::ElementReferences()
{
	if constexpr (N >= 0) {
		// I'll use the array, N is >= 0.
		// There will be a static number of references.
		elemRefs = std::array<Elem*, N> {nullptr};
	}
	else {
		// I'll use the vector, because N is < 0.
		// There will be a dynamic number of references.
		elemRefs = std::vector<Elem*>();
	}
}

template<class Elem, int N>
unsigned int ElementReferences<Elem, N>::elementNumber() const
{
	if constexpr (N >= 0) {
		return N;
	}
	else {
		return elemRefs.size();
	}
}

template<class Elem, int N>
Elem*& ElementReferences<Elem, N>::element(unsigned int i)
{
	assert(i < elementNumber());
	return elemRefs[i];
}

template<class Elem, int N>
const Elem* ElementReferences<Elem, N>::element(unsigned int i) const
{
	assert(i < elementNumber());
	return elemRefs[i];
}

template<class Elem, int N>
Elem*& ElementReferences<Elem, N>::elementMod(int i)
{
	unsigned int n = elementNumber();
	return elemRefs[(i % n + n) % n];
}

template<class Elem, int N>
const Elem* ElementReferences<Elem, N>::elementMod(int i) const
{
	unsigned int n = elementNumber();
	return elemRefs[(i % n + n) % n];
}

template<class Elem, int N>
void ElementReferences<Elem, N>::setElement(Elem* e, unsigned int i)
{
	assert(i < elementNumber());
	elemRefs[i] = e;
}

template<class Elem, int N>
void ElementReferences<Elem, N>::setElements(const std::vector<Elem*>& list)
{
	if constexpr (N >= 0) {
		assert(list.size() == N);
		unsigned int i = 0;
		for (const auto& v : list) {
			setElement(v, i);
			++i;
		}
	}
	else {
		elemRefs = list;
	}
}

template<class Elem, int N>
bool ElementReferences<Elem, N>::contains(const Elem* e) const
{
	return std::find(elemRefs.begin(), elemRefs.end(), e) != elemRefs.end();
}

template<class Elem, int N>
template<int U>
internal::ReturnIfIsVector<U, void> ElementReferences<Elem, N>::pushElement(Elem* v)
{
	elemRefs.push_back(v);
}

template<class Elem, int N>
template<int U>
internal::ReturnIfIsVector<U, void>
ElementReferences<Elem, N>::insertElement(unsigned int i, Elem* v)
{
	assert(i < elementNumber());
	elemRefs.insert(elemRefs.begin() + i, v);
}

template<class Elem, int N>
template<int U>
internal::ReturnIfIsVector<U, void> ElementReferences<Elem, N>::eraseElement(unsigned int i)
{
	assert(i < elementNumber());
	elemRefs.erase(elemRefs.begin() + i);
}

template<class Elem, int N>
template<int U>
internal::ReturnIfIsVector<U, void> ElementReferences<Elem, N>::clearElements()
{
	elemRefs.clear();
}

template<class Elem, int N>
typename ElementReferences<Elem, N>::ElementIterator ElementReferences<Elem, N>::elementBegin()
{
	return elemRefs.begin();
}

template<class Elem, int N>
typename ElementReferences<Elem, N>::ElementIterator ElementReferences<Elem, N>::elementEnd()
{
	return elemRefs.end();
}

template<class Elem, int N>
typename ElementReferences<Elem, N>::ConstElementIterator
ElementReferences<Elem, N>::elementBegin() const
{
	return elemRefs.begin();
}

template<class Elem, int N>
typename ElementReferences<Elem, N>::ConstElementIterator
ElementReferences<Elem, N>::elementEnd() const
{
	return elemRefs.end();
}

template<class Elem, int N>
typename ElementReferences<Elem, N>::ElementRangeIterator
ElementReferences<Elem, N>::elementIterator()
{
	return ElementRangeIterator(
		*this, &ElementReferences::elementBegin, &ElementReferences::elementEnd);
}

template<class Elem, int N>
typename ElementReferences<Elem, N>::ConstElementRangeIterator
ElementReferences<Elem, N>::elementIterator() const
{
	return ConstElementRangeIterator(
		*this, &ElementReferences::elementBegin, &ElementReferences::elementEnd);
}

template<class Elem, int N>
void ElementReferences<Elem, N>::updateElementReferences(const Elem* oldBase, const Elem* newBase)
{
	for (unsigned int j = 0; j < elementNumber(); ++j) {
		if (element(j) != nullptr) {
			size_t diff = element(j) - oldBase;
			element(j)  = (Elem*) newBase + diff;
		}
	}
}

template<class Elem, int N>
void ElementReferences<Elem, N>::updateElementReferencesAfterCompact(
	const Elem*             base,
	const std::vector<int>& newIndices)
{
	for (unsigned int j = 0; j < elementNumber(); ++j) {
		if (element(j) != nullptr) {
			size_t diff = element(j) - base;
			if (newIndices[diff] < 0) { // element has been removed
				element(j) = nullptr;
			}
			else { // the new pointer will be base + newIndices[diff]
				element(j) = (Elem*) base + newIndices[diff];
			}
		}
	}
}

} // namespace mgp::prop
