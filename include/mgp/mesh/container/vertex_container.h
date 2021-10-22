/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_CONTAINER_VERTEX_CONTAINER_H
#define MGP_MESH_CONTAINER_VERTEX_CONTAINER_H

#include "../vertex.h"
#include "container_t.h"
#include "optional/optional_components_vector.h"

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

	unsigned int vertexNumber() const;

	template<typename U = T>
	typename std::enable_if<common::hasOptionalColor<U>::value, void>::type
	enableVertexColor();

protected:
	std::vector<T> vertices;
	OptionalComponentsVector<T> optionalComponentsVector;

	unsigned int addVertex();;
};

} // namespace mesh
} // namespace mgp

#include "vertex_container.cpp"

#endif // MGP_MESH_CONTAINER_VERTEX_CONTAINER_H
