/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "vertex_container.h"

namespace mgp {
namespace mesh {

template<class T>
Container<T, mgp::ifIsBaseOf<VertexTriggerer, T> >::Container()
{
	verticesEnabled = true;
}

template<class T>
const typename Container<T, mgp::ifIsBaseOf<VertexTriggerer, T> >::VertexType*
mgp::mesh::Container<T, mgp::ifIsBaseOf<VertexTriggerer, T> >::vertex(unsigned int i) const
{
	return &vertices[i];
}

template<class T>
typename Container<T, mgp::ifIsBaseOf<VertexTriggerer, T> >::VertexType*
mgp::mesh::Container<T, mgp::ifIsBaseOf<VertexTriggerer, T> >::vertex(unsigned int i)
{
	return &vertices[i];
}

template<class T>
unsigned int Container<T, mgp::ifIsBaseOf<VertexTriggerer, T> >::vertexNumber() const
{
	return vertices.size();
}

template<class T>
unsigned int Container<T, mgp::ifIsBaseOf<VertexTriggerer, T> >::addVertex()
{
	vertices.push_back(VertexType());
	vertices[vertices.size() - 1]._id = vertices.size() - 1;
	return vertices[vertices.size() - 1]._id;
}



} // namespace mesh
} // namespace mgp
