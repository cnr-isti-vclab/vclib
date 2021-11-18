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
			B::optCont().adjVerts(thisId()) = std::array<Vertex*, N> {nullptr};
	}
	else {
		// I'll use the vector, because N is < 0.
		// There will be a dynamic number of references.
		if (B::contPtr)
			B::optCont().adjVerts(thisId()) = std::vector<Vertex*>();
	}
}

template<typename Vertex, int N, typename T>
unsigned int OptionalAdjacentVertices<Vertex, N, T>::adjVerticesNumber() const
{
	if constexpr (N >= 0) {
		return N;
	}
	else {
		return B::optCont().adjVerts(thisId()).size();
	}
}

template<typename Vertex, int N, typename T>
Vertex*& OptionalAdjacentVertices<Vertex, N, T>::adjVertex(unsigned int i)
{
	assert(i < adjVerticesNumber());
	return B::optCont().adjVerts(thisId())[i];
}

template<typename Vertex, int N, typename T>
const Vertex* OptionalAdjacentVertices<Vertex, N, T>::adjVertex(unsigned int i) const
{
	assert(i < adjVerticesNumber());
	return B::optCont().adjVerts(thisId())[i];
}

template<typename Vertex, int N, typename T>
Vertex*& OptionalAdjacentVertices<Vertex, N, T>::adjVertexMod(int i)
{
	unsigned int n = adjVerticesNumber();
	return B::optCont().adjVerts(thisId())[(i % n + n) % n];
}

template<typename Vertex, int N, typename T>
const Vertex* OptionalAdjacentVertices<Vertex, N, T>::adjVertexMod(int i) const
{
	unsigned int n = adjVerticesNumber();
	return B::optCont().adjVerts(thisId())[(i % n + n) % n];
}

template<typename Vertex, int N, typename T>
void OptionalAdjacentVertices<Vertex, N, T>::setAdjVertex(Vertex* f, unsigned int i)
{
	assert(i < adjVerticesNumber());
	B::optCont().adjVerts(thisId())[i] = f;
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
		B::optCont().adjVerts(thisId()) = list;
	}
}

template<typename Vertex, int N, typename T>
bool OptionalAdjacentVertices<Vertex, N, T>::containsAdjVertex(const Vertex* v) const
{
	return std::find(B::optCont().adjVerts.begin(), B::optCont().adjVerts.end(), v) !=
		   B::optCont().adjVerts.end();
}

template<typename Vertex, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void>
OptionalAdjacentVertices<Vertex, N, T>::resizeAdjVertices(unsigned int n)
{
	B::optCont().adjVerts(thisId()).resize(n);
}

template<typename Vertex, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void> OptionalAdjacentVertices<Vertex, N, T>::pushAdjVertex(Vertex* f)
{
	B::optCont().adjVerts(thisId()).push_back(f);
}

template<typename Vertex, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void>
OptionalAdjacentVertices<Vertex, N, T>::insertAdjVertex(unsigned int i, Vertex* f)
{
	assert(i < adjVerticesNumber());
	B::optCont().adjVerts(thisId()).insert(B::optCont().adjVerts(thisId()).begin() + i, f);
}

template<typename Vertex, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void>
OptionalAdjacentVertices<Vertex, N, T>::eraseAdjVertex(unsigned int i)
{
	assert(i < adjVerticesNumber());
	B::optCont().adjVerts(thisId()).erase(B::optCont().adjVerts(thisId()).begin() + i);
}

template<typename Vertex, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void> OptionalAdjacentVertices<Vertex, N, T>::clearAdjVertices()
{
	B::optCont().adjVerts(thisId()).clear();
}

template<typename Vertex, int N, typename T>
typename OptionalAdjacentVertices<Vertex, N, T>::VertexIterator
OptionalAdjacentVertices<Vertex, N, T>::adjVertexBegin()
{
	return B::optCont().adjVerts(thisId()).begin();
}

template<typename Vertex, int N, typename T>
typename OptionalAdjacentVertices<Vertex, N, T>::VertexIterator
OptionalAdjacentVertices<Vertex, N, T>::adjVertexEnd()
{
	return B::optCont().adjVerts(thisId()).end();
}

template<typename Vertex, int N, typename T>
typename OptionalAdjacentVertices<Vertex, N, T>::ConstVertexIterator
OptionalAdjacentVertices<Vertex, N, T>::adjVertexBegin() const
{
	return B::optCont().adjVerts(thisId()).begin();
}

template<typename Vertex, int N, typename T>
typename OptionalAdjacentVertices<Vertex, N, T>::ConstVertexIterator
OptionalAdjacentVertices<Vertex, N, T>::adjVertexEnd() const
{
	return B::optCont().adjVerts(thisId()).end();
}

template<typename Vertex, int N, typename T>
typename OptionalAdjacentVertices<Vertex, N, T>::VertexRangeIterator
OptionalAdjacentVertices<Vertex, N, T>::adjVertices()
{
	return VertexRangeIterator(
		B::optCont().adjVerts(thisId()),
		&OptionalAdjacentVertices::vertexBegin,
		&OptionalAdjacentVertices::vertexEnd);
}

template<typename Vertex, int N, typename T>
typename OptionalAdjacentVertices<Vertex, N, T>::ConstVertexRangeIterator
OptionalAdjacentVertices<Vertex, N, T>::adjVertices() const
{
	return ConstVertexRangeIterator(
		B::optCont().adjVerts(thisId()),
		&OptionalAdjacentVertices::vertexBegin,
		&OptionalAdjacentVertices::vertexEnd);
}

template<typename Vertex, int N, typename T>
void OptionalAdjacentVertices<Vertex, N, T>::updateVertexReferences(
	const Vertex* oldBase,
	const Vertex* newBase)
{
	for (unsigned int j = 0; j < adjVerticesNumber(); ++j) {
		if (adjVertex(j) != nullptr) {
			size_t diff = adjVertex(j) - oldBase;
			adjVertex(j)  = (Vertex*) newBase + diff;
		}
	}
}

template<typename Vertex, int N, typename T>
void OptionalAdjacentVertices<Vertex, N, T>::updateVertexReferencesAfterCompact(
	const Vertex*           base,
	const std::vector<int>& newIndices)
{
	for (unsigned int j = 0; j < adjVerticesNumber(); ++j) {
		if (adjVertex(j) != nullptr) {
			size_t diff = adjVertex(j) - base;
			if (newIndices[diff] < 0) { // element has been removed
				adjVertex(j) = nullptr;
			}
			else { // the new pointer will be base + newIndices[diff]
				adjVertex(j) = (Vertex*) base + newIndices[diff];
			}
		}
	}
}

} // namespace mgp::comp
