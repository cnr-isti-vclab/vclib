/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "mesh.h"

namespace mgp {

template<class... Args>
typename Mesh<Args...>::Vertex* mgp::Mesh<Args...>::addVertex()
{
	Vertex* oldBase = vertices.data();
	Vertex* v = VertexContainer::addVertex();
	Vertex* newBase = vertices.data();
	if (oldBase != nullptr && oldBase != newBase)
		updateVertexReferences(oldBase, newBase);
	return v;
}

template<class... Args>
typename Mesh<Args...>::Face* Mesh<Args...>::addFace()
{
	Face* f = FaceContainer::addFace();
	return f;
}

template<class... Args>
void Mesh<Args...>::updateVertexReferences(const Vertex* oldBase, const Vertex* newBase)
{
	if constexpr (face::hasVertexRefsArray<Mesh::Face>::value) {
		for (unsigned int i = 0; i < faces.size(); ++i){
			for (unsigned int j = 0; j < faces[i].N_VERTICES; ++j){
				if (faces[i].v(j) != nullptr) {
					size_t diff = faces[i].v(j) - oldBase;
					faces[i].v(j) = (Vertex*)newBase + diff;
				}
			}
		}
	}
}

}
