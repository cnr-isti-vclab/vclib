/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "vertex_references.h"

namespace mgp::components {

template<class Vertex, int N>
VertexReferences<Vertex, N>::VertexReferences()
{
	if constexpr (N >= 0) {
		// I'll use the array, N is >= 0.
		// There will be a static number of references.
		vertRefs = std::array<Vertex*, N> {nullptr};
	}
	else {
		// I'll use the vector, because N is < 0.
		// There will be a dynamic number of references.
		vertRefs = std::vector<Vertex*>();
	}
}

template<class Vertex, int N>
unsigned int VertexReferences<Vertex, N>::vertexNumber() const
{
	if constexpr (N >= 0) {
		return N;
	}
	else {
		return vertRefs.size();
	}
}

template<class Vertex, int N>
unsigned int VertexReferences<Vertex, N>::sizeMod(unsigned int i) const
{
	return i % vertexNumber();
}

template<class Vertex, int N>
Vertex*& VertexReferences<Vertex, N>::v(unsigned int i)
{
	assert(i < vertexNumber());
	return vertRefs[i];
}

template<class Vertex, int N>
const Vertex* VertexReferences<Vertex, N>::v(unsigned int i) const
{
	assert(i < vertexNumber());
	return vertRefs[i];
}

template<class Vertex, int N>
void VertexReferences<Vertex, N>::setVertex(Vertex* v, unsigned int i)
{
	assert(i < vertexNumber());
	vertRefs[i] = v;
}

template<class Vertex, int N>
void VertexReferences<Vertex, N>::setVertices(const std::vector<Vertex*>& list)
{
	if constexpr (N >= 0) {
		assert(list.size() == N);
		unsigned int i = 0;
		for (const auto& v : list) {
			setVertex(v, i);
			++i;
		}
	}
	else {
		vertRefs = list;
	}
}

template<class Vertex, int N>
template<int U>
internal::ReturnIfIsVector<U, void> VertexReferences<Vertex, N>::pushVertex(Vertex* v)
{
	vertRefs.push_back(v);
}

template<class Vertex, int N>
template<int U>
internal::ReturnIfIsVector<U, void>
VertexReferences<Vertex, N>::insertVertex(unsigned int i, Vertex* v)
{
	assert(i < vertexNumber());
	vertRefs.insert(vertRefs.begin() + i, v);
}

template<class Vertex, int N>
template<int U>
internal::ReturnIfIsVector<U, void> VertexReferences<Vertex, N>::eraseVertex(unsigned int i)
{
	assert(i < vertexNumber());
	vertRefs.erase(vertRefs.begin() + i);
}

template<class Vertex, int N>
typename VertexReferences<Vertex, N>::VertexIterator VertexReferences<Vertex, N>::vertexBegin()
{
	return vertRefs.begin();
}

template<class Vertex, int N>
typename VertexReferences<Vertex, N>::VertexIterator VertexReferences<Vertex, N>::vertexEnd()
{
	return vertRefs.end();
}

template<class Vertex, int N>
typename VertexReferences<Vertex, N>::ConstVertexIterator VertexReferences<Vertex, N>::vertexBegin() const
{
	return vertRefs.begin();
}

template<class Vertex, int N>
typename VertexReferences<Vertex, N>::ConstVertexIterator VertexReferences<Vertex, N>::vertexEnd() const
{
	return vertRefs.end();
}

template<class Vertex, int N>
typename VertexReferences<Vertex, N>::VertexRangeIterator VertexReferences<Vertex, N>::vertexIterator()
{
	return VertexRangeIterator(
		vertRefs, &vertexBegin, &vertexEnd);
}

template<class Vertex, int N>
typename VertexReferences<Vertex, N>::ConstVertexRangeIterator VertexReferences<Vertex, N>::vertexIterator() const
{
	return ConstVertexRangeIterator(
		vertRefs, &vertexBegin, &vertexEnd);
}

template<class Vertex, int N>
void VertexReferences<Vertex, N>::updateVertexReferences(
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

} // namespace mgp::components
