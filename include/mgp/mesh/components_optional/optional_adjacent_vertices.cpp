/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "optional_adjacent_vertices.h"

#include <algorithm>

namespace mgp::comp {

template<typename Vertex, int N, typename T>
OptionalAdjacentVertices<Vertex, N, T>::OptionalAdjacentVertices()
{
	if constexpr (N >= 0) {
		// I'll use the array, N is >= 0.
		// There will be a static number of references.
		if (B::contPtr)
			B::contPtr->adjVerts(thisId()) = std::array<Vertex*, N> {nullptr};
	}
	else {
		// I'll use the vector, because N is < 0.
		// There will be a dynamic number of references.
		if (B::contPtr)
			B::contPtr->adjVerts(thisId()) = std::vector<Vertex*>();
	}
}

template<typename Vertex, int N, typename T>
unsigned int OptionalAdjacentVertices<Vertex, N, T>::adjVerticesNumber() const
{
	if constexpr (N >= 0) {
		return N;
	}
	else {
		return B::contPtr->adjVerts(thisId()).size();
	}
}

template<typename Vertex, int N, typename T>
Vertex*& OptionalAdjacentVertices<Vertex, N, T>::adjVert(unsigned int i)
{
	assert(i < adjVerticesNumber());
	return B::contPtr->adjVerts(thisId())[i];
}

template<typename Vertex, int N, typename T>
const Vertex* OptionalAdjacentVertices<Vertex, N, T>::adjVert(unsigned int i) const
{
	assert(i < adjVerticesNumber());
	return B::contPtr->adjVerts(thisId())[i];
}

template<typename Vertex, int N, typename T>
Vertex*& OptionalAdjacentVertices<Vertex, N, T>::adjVertMod(int i)
{
	unsigned int n = adjVerticesNumber();
	return B::contPtr->adjVerts(thisId())[(i % n + n) % n];
}

template<typename Vertex, int N, typename T>
const Vertex* OptionalAdjacentVertices<Vertex, N, T>::adjVertMod(int i) const
{
	unsigned int n = adjVerticesNumber();
	return B::contPtr->adjVerts(thisId())[(i % n + n) % n];
}

template<typename Vertex, int N, typename T>
void OptionalAdjacentVertices<Vertex, N, T>::setAdjVertex(Vertex* f, unsigned int i)
{
	assert(i < adjVerticesNumber());
	B::contPtr->adjVerts(thisId())[i] = f;
}

template<typename Vertex, int N, typename T>
void OptionalAdjacentVertices<Vertex, N, T>::setAdjVertices(const std::vector<Vertex*>& list)
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
		B::contPtr->adjVerts(thisId()) = list;
	}
}

template<typename Vertex, int N, typename T>
bool OptionalAdjacentVertices<Vertex, N, T>::containsAdjVertex(const Vertex* v) const
{
	return std::find(B::contPtr->adjVerts.begin(), B::contPtr->adjVerts.end(), v) !=
		   B::contPtr->adjVerts.end();
}

template<typename Vertex, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void>
OptionalAdjacentVertices<Vertex, N, T>::resizeAdjVertices(unsigned int n)
{
	B::contPtr->adjVerts(thisId()).resize(n);
}

template<typename Vertex, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void> OptionalAdjacentVertices<Vertex, N, T>::pushAdjVertex(Vertex* f)
{
	B::contPtr->adjVerts(thisId()).push_back(f);
}

template<typename Vertex, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void>
OptionalAdjacentVertices<Vertex, N, T>::insertAdjVertex(unsigned int i, Vertex* f)
{
	assert(i < adjVerticesNumber());
	B::contPtr->adjVerts(thisId()).insert(B::contPtr->adjVerts(thisId()).begin() + i, f);
}

template<typename Vertex, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void>
OptionalAdjacentVertices<Vertex, N, T>::eraseAdjVertex(unsigned int i)
{
	assert(i < adjVerticesNumber());
	B::contPtr->adjVerts(thisId()).erase(B::contPtr->adjVerts(thisId()).begin() + i);
}

template<typename Vertex, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void> OptionalAdjacentVertices<Vertex, N, T>::clearAdjVertices()
{
	B::contPtr->adjVerts(thisId()).clear();
}

template<typename Vertex, int N, typename T>
typename OptionalAdjacentVertices<Vertex, N, T>::VertexIterator
OptionalAdjacentVertices<Vertex, N, T>::adjVertexBegin()
{
	return B::contPtr->adjVerts(thisId()).begin();
}

template<typename Vertex, int N, typename T>
typename OptionalAdjacentVertices<Vertex, N, T>::VertexIterator
OptionalAdjacentVertices<Vertex, N, T>::adjVertexEnd()
{
	return B::contPtr->adjVerts(thisId()).end();
}

template<typename Vertex, int N, typename T>
typename OptionalAdjacentVertices<Vertex, N, T>::ConstVertexIterator
OptionalAdjacentVertices<Vertex, N, T>::adjVertexBegin() const
{
	return B::contPtr->adjVerts(thisId()).begin();
}

template<typename Vertex, int N, typename T>
typename OptionalAdjacentVertices<Vertex, N, T>::ConstVertexIterator
OptionalAdjacentVertices<Vertex, N, T>::adjVertexEnd() const
{
	return B::contPtr->adjVerts(thisId()).end();
}

template<typename Vertex, int N, typename T>
typename OptionalAdjacentVertices<Vertex, N, T>::VertexRangeIterator
OptionalAdjacentVertices<Vertex, N, T>::adjVertices()
{
	return VertexRangeIterator(
		B::contPtr->adjVerts(thisId()),
		&OptionalAdjacentVertices::vertexBegin,
		&OptionalAdjacentVertices::vertexEnd);
}

template<typename Vertex, int N, typename T>
typename OptionalAdjacentVertices<Vertex, N, T>::ConstVertexRangeIterator
OptionalAdjacentVertices<Vertex, N, T>::adjVertices() const
{
	return ConstVertexRangeIterator(
		B::contPtr->adjVerts(thisId()),
		&OptionalAdjacentVertices::vertexBegin,
		&OptionalAdjacentVertices::vertexEnd);
}

template<typename Vertex, int N, typename T>
void OptionalAdjacentVertices<Vertex, N, T>::updateVertexReferences(
	const Vertex* oldBase,
	const Vertex* newBase)
{
	for (unsigned int j = 0; j < adjVerticesNumber(); ++j) {
		if (adjVert(j) != nullptr) {
			size_t diff = adjVert(j) - oldBase;
			adjVert(j)  = (Vertex*) newBase + diff;
		}
	}
}

template<typename Vertex, int N, typename T>
void OptionalAdjacentVertices<Vertex, N, T>::updateVertexReferencesAfterCompact(
	const Vertex*           base,
	const std::vector<int>& newIndices)
{
	for (unsigned int j = 0; j < adjVerticesNumber(); ++j) {
		if (adjVert(j) != nullptr) {
			size_t diff = adjVert(j) - base;
			if (newIndices[diff] < 0) { // element has been removed
				adjVert(j) = nullptr;
			}
			else { // the new pointer will be base + newIndices[diff]
				adjVert(j) = (Vertex*) base + newIndices[diff];
			}
		}
	}
}

} // namespace mgp::comp
