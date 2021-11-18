/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_MESH_H
#define MGP_MESH_MESH_H

#include "mesh_argument.h"
#include "mesh_components.h"

namespace mgp {

/**
 * @brief The Mesh class represents a generic 3D mesh. A mesh is composed of a generic number of
 * containers of Elements (which can be vertices, faces, edges...), plus some other components.
 *
 * The Mesh class will expose all the public members of its containers, and its role is to implement
 * all the functionalities that allow these containers to comunicate (e.g. an operation on the
 * vertex container that requires to update also some face information).
 *
 * Therefore, in this class there is only the implementation of all the functions that in some way
 * need to modify the status of more than one Container of the Mesh. Other functions are inherited
 * by the Container classes of the Elements of the Mesh, or from its Components, and depend on all
 * the templates that compose a specific Mesh definition.
 */
template<typename... Args>
class Mesh : public mesh::Container<Args>...
{
public:
	Mesh();
	Mesh(const Mesh& oth);
	Mesh(Mesh&& oth);

	void clear();

	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, unsigned int>
	index(const typename Mesh::VertexType& v) const;

	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, unsigned int>
	index(const typename Mesh::VertexType* v) const;

	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, unsigned int> addVertex();

	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, unsigned int>
	addVertex(const typename Mesh::VertexType::CoordType& p);

	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, unsigned int> addVertices(unsigned int n);

	template<typename... VC, typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, unsigned int> addVertices(const VC&... v);

	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, void> reserveVertices(unsigned int n);

	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, void> compactVertices();

	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, unsigned int>
	index(const typename Mesh::FaceType& f) const;

	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, unsigned int>
	index(const typename Mesh::FaceType* f) const;

	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, unsigned int> addFace();

	template<typename U = Mesh, typename... V>
	mesh::ReturnIfHasFaceContainer<U, unsigned int> addFace(V... args);

	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, unsigned int>
	addFace(const std::vector<typename Mesh::VertexType*>& v);

	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, unsigned int> addFaces(unsigned int n);

	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, void> reserveFaces(unsigned int n);

	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, void> compactFaces();

	/// @private
	template<typename... A>
	friend void swap(Mesh<A...>& m1, Mesh<A...>& m2);

	void swap(Mesh& m2);

	Mesh& operator=(Mesh oth);

protected:
	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, void> updateVertexReferences(
		const typename U::VertexType* oldBase,
		const typename U::VertexType* newBase);

	template<typename U = Mesh>
	mesh::ReturnIfHasVertexContainer<U, void> updateVertexReferencesAfterCompact(
		const typename U::VertexType* base,
		const std::vector<int>&       newIndices);

	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, void>
	updateFaceReferences(const typename U::FaceType* oldBase, const typename U::FaceType* newBase);

	template<typename U = Mesh>
	mesh::ReturnIfHasFaceContainer<U, void> updateFaceReferencesAfterCompact(
		const typename U::FaceType* base,
		const std::vector<int>&     newIndices);

	void updateAllOptionalContainerReferences();
};

template<typename... A>
inline void swap(Mesh<A...>& m1, Mesh<A...>& m2);

} // namespace mgp

#include "mesh.cpp"

#endif // MGP_MESH_MESH_H
