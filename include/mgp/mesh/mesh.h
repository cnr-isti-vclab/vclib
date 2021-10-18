/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_MESH_H
#define MGP_MESH_MESH_H

#include "mesh/containers.h"

namespace mgp {

template<class... Args>
class Mesh : public mesh::Container<Args>...
{
protected:
	typedef typename Mesh::VertexContainer VertexContainer;
public:
	typedef typename Mesh::VertexType VertexType;
	using VertexContainer::addVertex;

protected:
	std::vector<VertexType>& verts = VertexContainer::vertices;
};

}

#endif // MGP_MESH_FACE_H
