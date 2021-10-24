/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "mesh.h"

namespace mgp {

template<class... Args>
template<typename U>
mesh::ReturnIfHasVertexContainer<U, unsigned int> mgp::Mesh<Args...>::addVertex()
{
	Vertex* oldBase = VertexContainer::vertices.data();
	unsigned int vid = VertexContainer::addVertex();
	Vertex* newBase = VertexContainer::vertices.data();
	if (oldBase != nullptr && oldBase != newBase)
		updateVertexReferences(oldBase, newBase);
	return vid;
}

template<class... Args>
template<typename U>
mesh::ReturnIfHasFaceContainer<U, unsigned int> Mesh<Args...>::addFace()
{
	unsigned int fid = FaceContainer::addFace();
	return fid;
}

template<class...Args>
template<typename U>
ReturnIfHasBox<U, void> Mesh<Args...>::updateBoundingBox()
{
	Mesh::Box::setNull();
	for (const Vertex& v : VertexContainer::vertexIterator()) {
		Mesh::Box::add(v.coordinate());
	}
}

template<class... Args>
void Mesh<Args...>::updateVertexReferences(const Vertex* oldBase, const Vertex* newBase)
{
	FaceContainer::updateVertexReferences(oldBase, newBase);
}

}
