/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_CONTAINER_VERTEX_CONTAINER_H
#define MGP_MESH_CONTAINER_VERTEX_CONTAINER_H

#include "../vertex.h"
#include "container_t.h"

namespace mgp {
namespace mesh {

template<class T>
class Container<T, mgp::ifIsBaseOf<VertexTriggerer, T>> :
		public virtual EnabledContainers
{
protected:
	// types:
	typedef T                                          VertexType;
	typedef Container<T, mgp::ifIsBaseOf<VertexTriggerer, T>> VertexContainer;

public:
	Container();

	const VertexType* vertex(unsigned int i) const;
	VertexType*       vertex(unsigned int i);

	unsigned int vertexNumber() const;;

protected:
	std::vector<T> vertices;

	unsigned int addVertex();;
};

} // namespace mesh
} // namespace mgp

#include "vertex_container.cpp"

#endif // MGP_MESH_CONTAINER_VERTEX_CONTAINER_H
