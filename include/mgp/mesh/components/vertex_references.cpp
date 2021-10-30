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
		refs = std::array<Vertex*, N>{nullptr};
	}
	else {
		refs = std::vector<Vertex*>();
	}
}

template<class Vertex, int N>
Vertex*& VertexReferences<Vertex, N>::v(unsigned int i)
{
	return std::get<VARIANT_ID>(refs)[i];
}

template<class Vertex, int N>
const Vertex* VertexReferences<Vertex, N>::v(unsigned int i) const
{
	assert(i < N);
	return std::get<VARIANT_ID>(refs)[i];
}

template<class Vertex, int N>
void VertexReferences<Vertex, N>::setVertex(Vertex* v, unsigned int i)
{
	assert(i < N);
	std::get<VARIANT_ID>(refs)[i] = v;
}

template<class Vertex, int N>
void VertexReferences<Vertex, N>::setVertices(const std::vector<Vertex*>& list)
{
	unsigned int i = 0;
	for (const auto& v : list) {
		setVertex(v, i);
		++i;
	}
}

template<class Vertex, int N>
void VertexReferences<Vertex, N>::updateVertexReferences(
	const Vertex* oldBase,
	const Vertex* newBase)
{
	for (unsigned int j = 0; j < N; ++j) {
		if (v(j) != nullptr) {
			size_t diff = v(j) - oldBase;
			v(j)        = (Vertex*) newBase + diff;
		}
	}
}

} // namespace mgp::face
