/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_MESH_H
#define MGP_MESH_MESH_H

#include "container/containers.h"
#include "mesh_properties.h"

namespace mgp {

/**
 * @brief The Mesh class represents a generic 3D mesh. A mesh is composed of a generic number of
 * containers of Elements (which can be vertices, faces, edges...).
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
	Mesh() {};
	Mesh(const Mesh& oth);
	Mesh(Mesh&& oth);

	void clear();

	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, unsigned int> addVertex();

	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, void> reserveVertices(unsigned int i);

	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, void> compactVertices();

	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, unsigned int> addFace();

	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, void> reserveFaces(unsigned int i);

	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, void> compactFaces();

	template<class...A>
	friend void swap(Mesh<A...>& m1, Mesh<A...>& m2);

	void swap(Mesh& m2);

	Mesh& operator=(Mesh oth);

protected:
	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, void>
	updateVertexReferences(const typename U::VertexType* oldBase, const typename U::VertexType* newBase);

	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, void>
	updateVertexReferencesAfterCompact(const typename U::VertexType* base, const std::vector<int>& newIndices);

	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, void>
	updateFaceReferences(const typename U::FaceType* oldBase, const typename U::FaceType* newBase);

	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, void>
	updateFaceReferencesAfterCompact(const typename U::FaceType* base, const std::vector<int>& newIndices);

	void updateAllOptionalContainerReferences();
};

template<class...A>
inline void swap(Mesh<A...>& m1, Mesh<A...>& m2);

} // namespace mgp

#include "mesh.cpp"

#endif // MGP_MESH_MESH_H
