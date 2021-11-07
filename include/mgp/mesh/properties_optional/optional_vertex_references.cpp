/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "optional_vertex_references.h"

namespace mgp::prop {

template<class Vertex, int N, typename T>
OptionalVertexReferences<Vertex, N, T>::OptionalVertexReferences()
{
	if constexpr (N >= 0) {
		// I'll use the array, N is >= 0.
		// There will be a static number of references.
		if (B::contPtr)
			B::contPtr->vertRefs(thisId()) = std::array<Vertex*, N> {nullptr};
	}
	else {
		// I'll use the vector, because N is < 0.
		// There will be a dynamic number of references.
		if (B::contPtr)
			B::contPtr->vertRefs(thisId()) = std::vector<Vertex*>();
	}
}

template<class Vertex, int N, typename T>
unsigned int OptionalVertexReferences<Vertex, N, T>::vertexNumber() const
{
	if constexpr (N >= 0) {
		return N;
	}
	else {
		return B::contPtr->vertRefs(thisId()).size();
	}
}

template<class Vertex, int N, typename T>
Vertex*& OptionalVertexReferences<Vertex, N, T>::v(unsigned int i)
{
	assert(i < vertexNumber());
	return B::contPtr->vertRefs(thisId())[i];
}

template<class Vertex, int N, typename T>
const Vertex* OptionalVertexReferences<Vertex, N, T>::v(unsigned int i) const
{
	assert(i < vertexNumber());
	return B::contPtr->vertRefs(thisId())[i];
}

template<class Vertex, int N, typename T>
Vertex*& OptionalVertexReferences<Vertex, N, T>::vMod(int i)
{
	unsigned int n = vertexNumber();
	return B::contPtr->vertRefs(thisId())[(i % n + n) % n];
}

template<class Vertex, int N, typename T>
const Vertex* OptionalVertexReferences<Vertex, N, T>::vMod(int i) const
{
	unsigned int n = vertexNumber();
	return B::contPtr->vertRefs(thisId())[(i % n + n) % n];
}

template<class Vertex, int N, typename T>
void OptionalVertexReferences<Vertex, N, T>::setVertex(Vertex* f, unsigned int i)
{
	assert(i < vertexNumber());
	B::contPtr->vertRefs(thisId())[i] = f;
}

template<class Vertex, int N, typename T>
void OptionalVertexReferences<Vertex, N, T>::setVertexs(const std::vector<Vertex*>& list)
{
	if constexpr (N >= 0) {
		assert(list.size() == N);
		unsigned int i = 0;
		for (const auto& f : list) {
			setVertex(f, i);
			++i;
		}
	}
	else {
		B::contPtr->vertRefs(thisId()) = list;
	}
}

template<class Vertex, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void> OptionalVertexReferences<Vertex, N, T>::pushVertex(Vertex* f)
{
	B::contPtr->vertRefs(thisId()).push_back(f);
}

template<class Vertex, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void>
OptionalVertexReferences<Vertex, N, T>::insertVertex(unsigned int i, Vertex* f)
{
	assert(i < vertexNumber());
	B::contPtr->vertRefs(thisId()).insert(B::contPtr->vertRefs(thisId()).begin() + i, f);
}

template<class Vertex, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void> OptionalVertexReferences<Vertex, N, T>::eraseVertex(unsigned int i)
{
	assert(i < vertexNumber());
	B::contPtr->vertRefs(thisId()).erase(B::contPtr->vertRefs(thisId()).begin() + i);
}

template<class Vertex, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void> OptionalVertexReferences<Vertex, N, T>::clearVertexs()
{
	B::contPtr->vertRefs(thisId()).clear();
}

template<class Vertex, int N, typename T>
typename OptionalVertexReferences<Vertex, N, T>::VertexIterator
OptionalVertexReferences<Vertex, N, T>::vertexBegin()
{
	return B::contPtr->vertRefs(thisId()).begin();
}

template<class Vertex, int N, typename T>
typename OptionalVertexReferences<Vertex, N, T>::VertexIterator
OptionalVertexReferences<Vertex, N, T>::vertexEnd()
{
	return B::contPtr->vertRefs(thisId()).end();
}

template<class Vertex, int N, typename T>
typename OptionalVertexReferences<Vertex, N, T>::ConstVertexIterator
OptionalVertexReferences<Vertex, N, T>::vertexBegin() const
{
	return B::contPtr->vertRefs(thisId()).begin();
}

template<class Vertex, int N, typename T>
typename OptionalVertexReferences<Vertex, N, T>::ConstVertexIterator
OptionalVertexReferences<Vertex, N, T>::vertexEnd() const
{
	return B::contPtr->vertRefs(thisId()).end();
}

template<class Vertex, int N, typename T>
typename OptionalVertexReferences<Vertex, N, T>::VertexRangeIterator
OptionalVertexReferences<Vertex, N, T>::vertexIterator()
{
	return VertexRangeIterator(
		B::contPtr->vertRefs(thisId()),
		&OptionalVertexReferences::vertexBegin,
		&OptionalVertexReferences::vertexEnd);
}

template<class Vertex, int N, typename T>
typename OptionalVertexReferences<Vertex, N, T>::ConstVertexRangeIterator
OptionalVertexReferences<Vertex, N, T>::vertexIterator() const
{
	return ConstVertexRangeIterator(
		B::contPtr->vertRefs(thisId()),
		&OptionalVertexReferences::vertexBegin,
		&OptionalVertexReferences::vertexEnd);
}

template<class Vertex, int N, typename T>
void OptionalVertexReferences<Vertex, N, T>::updateVertexReferences(
	const Vertex* oldBase,
	const Vertex* newBase)
{
	for (unsigned int j = 0; j < vertexNumber(); ++j) {
		if (v(j) != nullptr) {
			size_t diff = v(j) - oldBase;
			v(j)        = (Vertex*) newBase + diff;
		}
	}
}

template<class Vertex, int N, typename T>
void OptionalVertexReferences<Vertex, N, T>::updateVertexReferencesAfterCompact(
	const Vertex* base,
	const std::vector<int>& newIndices)
{
	for (unsigned int j = 0; j < vertexNumber(); ++j) {
		if (v(j) != nullptr) {
			size_t diff = v(j) - base;
			if (newIndices[diff] < 0){ // element has been removed
				v(j) = nullptr;
			}
			else { // the new pointer will be base + newIndices[diff]
				v(j) = (Vertex*) base + newIndices[diff];
			}
		}
	}
}

} // namespace mgp::prop
