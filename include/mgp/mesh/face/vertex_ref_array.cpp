/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "vertex_ref_array.h"

namespace mgp {
namespace face {

template<class Vertex, int N>
VertexRefsArray<Vertex, N>::VertexRefsArray() : refs({nullptr})
{
}

template<class Vertex, int N>
Vertex*& VertexRefsArray<Vertex, N>::v(unsigned int i)
{
	assert(i < N);
	return refs[i];
}

template<class Vertex, int N>
const Vertex* VertexRefsArray<Vertex, N>::v(unsigned int i) const
{
	assert(i < N);
	return refs[i];
}

template<class Vertex, int N>
void VertexRefsArray<Vertex, N>::setVertex(Vertex* v, unsigned int i)
{
	assert(i < N);
	refs[i] = v;
}

template<class Vertex, int N>
void VertexRefsArray<Vertex, N>::setVertices(const std::initializer_list<Vertex*>& list)
{
	// todo: static assert
	unsigned int i = 0;
	for (const auto& v : list) {
		setVertex(v, i);
		++i;
	}
}

template<class Vertex, int N>
void VertexRefsArray<Vertex, N>::updateVertexReferences(
	const Vertex* oldBase,
	const Vertex* newBase)
{
	for (unsigned int j = 0; j < N_VERTICES; ++j) {
		if (v(j) != nullptr) {
			size_t diff = v(j) - oldBase;
			v(j)        = (Vertex*) newBase + diff;
		}
	}
}

} // namespace face
} // namespace mgp
