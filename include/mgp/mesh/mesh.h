/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_MESH_H
#define MGP_MESH_MESH_H

#include "container/containers.h"

namespace mgp {

/**
 * @brief The Mesh class represents a generic 3D mesh. A mesh is composed of a generic number of
 * containers of Elements (which can be vertices, faces, edges...
 *
 * The Mesh class will expose all the public members of its containers, and its role is to implement
 * all the functionalities that allow these containers to comunicate (e.g. an operation on the
 * vertex container that requires to update also some face information).
 */
template<class... Args>
class Mesh : public mesh::Container<Args>...
{
protected:
public:
	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, unsigned int> addVertex();
	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, unsigned int> addFace();

protected:
	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, void>
	updateVertexReferences(const typename U::VertexType* oldBase, const typename U::VertexType* newBase);
};

} // namespace mgp

#include "mesh.cpp"

#endif // MGP_MESH_MESH_H
