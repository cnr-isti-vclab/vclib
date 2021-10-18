/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_MESH_VERTEX_CONTAINER_H
#define MGP_MESH_MESH_VERTEX_CONTAINER_H

#include "../vertex.h"
#include "container_t.h"

namespace mgp {

namespace mesh {

template<class T>
class Container<T, std::enable_if_t<std::is_base_of_v<VertexID, T>>> : public virtual EnabledContainers
{
protected:
	// types:
	typedef T VertexType;
	typedef Container<T, std::enable_if_t<std::is_base_of_v<VertexID, T>>> VertexContainer;

public:
	Container() {verticesEnabled = true;}

	const VertexType* vertex(unsigned int i) const {return &vertices[i];}
	VertexType* vertex(unsigned int i) {return &vertices[i];}

	unsigned int vertexNumber() const {return  vertices.size();};

protected:
	std::vector<T> vertices;

	VertexType* addVertex()
	{
		vertices.push_back(VertexType());
		vertices[vertices.size()-1].id = vertices.size()-1;
		return &vertices[vertices.size()-1];
	};
};

}

}

#endif // MGP_MESH_MESH_COMPONENT_H
