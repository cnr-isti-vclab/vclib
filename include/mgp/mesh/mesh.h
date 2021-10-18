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

public:
	typedef typename Mesh::VertexType Vertex;
	typedef typename Mesh::FaceType Face;

	Vertex* addVertex();

	Face* addFace();

protected:
	void updateVertexReferences(const Vertex* oldBase, const Vertex* newBase);

	typedef typename Mesh::VertexContainer VertexContainer;
	typedef typename Mesh::FaceContainer FaceContainer;
	std::vector<Vertex>& vertices = VertexContainer::vertices;
	std::vector<Face>& faces = FaceContainer::faces;
};

}

#include "mesh.cpp"

#endif // MGP_MESH_MESH_H
