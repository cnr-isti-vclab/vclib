/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_MESH_MESH_H
#define VCL_MESH_MESH_H

#include "containers/containers.h"
#include "mesh_components.h"
#include "requirements/mesh_concepts.h"

namespace vcl {

/**
 * @brief The Mesh class represents a generic 3D mesh. A mesh is composed of a generic number of
 * containers of Elements (which can be vertices, faces, edges...), plus some other components.
 *
 * The Mesh class will expose all the public members of its containers and is components, and its
 * role is to implement all the functionalities that allow these containers to comunicate
 * (e.g. an operation on the vertex container that requires to update also some face information).
 *
 * Therefore, in this page are documented only the functions that in some way need to modify the
 * status of more than one Container of the Mesh. Other functions are inherited by the Container
 * classes of the Elements of the Mesh, or from its Components, and depend on all the templates
 * that compose a specific Mesh definition.
 */
template<typename... Args> requires HasVertices<Args...>
class Mesh : public Args...
{
public:
	Mesh();
	Mesh(const Mesh& oth);
	Mesh(Mesh&& oth);

	void clear();

	// Vertices
	uint index(const typename Mesh::VertexType& v) const;
	uint index(const typename Mesh::VertexType* v) const;
	uint addVertex();
	uint addVertex(const typename Mesh::VertexType::CoordType& p);
	uint addVertices(uint n);

	template<typename... VC>
	uint addVertices(const typename Mesh::VertexType::CoordType& p, const VC&... v);

	void reserveVertices(uint n);
	void compactVertices();

	// Faces

	template<HasFaces M = Mesh>
	uint index(const typename M::FaceType& f) const;

	template<HasFaces M = Mesh>
	uint index(const typename M::FaceType* f) const;

	uint addFace() requires HasFaces<Mesh>;

	template<typename... V>
	uint addFace(V... args) requires HasFaces<Mesh>;

	template<typename Iterator>
	uint addFace(Iterator begin, Iterator end) requires HasFaces<Mesh>;

	uint addFaces(uint n) requires HasFaces<Mesh>;
	void reserveFaces(uint n) requires HasFaces<Mesh>;
	void compactFaces() requires HasFaces<Mesh>;

	// Edges

	template<HasEdges M = Mesh>
	uint index(const typename M::EdgeType& e) const;

	template<HasEdges M = Mesh>
	uint index(const typename M::EdgeType* v) const;

	uint addEdge() requires HasEdges<Mesh>;
	uint addEdges(uint n) requires HasEdges<Mesh>;
	void reserveEdges(uint n) requires HasEdges<Mesh>;
	void compactEdges() requires HasEdges<Mesh>;

	// HalfEdges

	template<HasHalfEdges M = Mesh>
	uint index(const typename M::HalfEdgeType& e) const;

	template<HasHalfEdges M = Mesh>
	uint index(const typename M::HalfEdgeType* v) const;

	uint addHalfEdge() requires HasHalfEdges<Mesh>;
	uint addHalfEdges(uint n) requires HasHalfEdges<Mesh>;

	template<typename M = Mesh> requires HasFaces<M>
	uint addHalfEdgesToFace(uint n, typename M::FaceType& f) requires HasHalfEdges<Mesh>;

	void reserveHalfEdges(uint n) requires HasHalfEdges<Mesh>;
	void compactHalfEdges() requires HasHalfEdges<Mesh>;

	template<typename OtherMeshType>
	void enableSameOptionalComponentsOf(const OtherMeshType& m);

	template<typename OtherMeshType>
	void importFrom(const OtherMeshType& m);

	/// @private
	template<typename... A> requires HasVertices<A...>
	friend void swap(Mesh<A...>& m1, Mesh<A...>& m2);

	void swap(Mesh& m2);

	Mesh& operator=(Mesh oth);

protected:
	// Vertices
	void updateVertexReferences(
		const typename Mesh::VertexType* oldBase,
		const typename Mesh::VertexType* newBase);

	void updateVertexReferencesAfterCompact(
		const typename Mesh::VertexType* base,
		const std::vector<int>&       newIndices);

	// Faces
	template<HasFaces M = Mesh>
	void updateFaceReferences(
		const typename M::FaceType* oldBase,
		const typename M::FaceType* newBase);

	template<HasFaces M = Mesh>
	void updateFaceReferencesAfterCompact(
		const typename M::FaceType* base,
		const std::vector<int>&     newIndices);

	// Edges
	template<HasEdges M = Mesh>
	void updateEdgeReferences(
		const typename M::EdgeType* oldBase,
		const typename M::EdgeType* newBase);

	template<HasEdges M = Mesh>
	void updateEdgeReferencesAfterCompact(
		const typename M::EdgeType* base,
		const std::vector<int>&     newIndices);

	// HalfEdges
	template<HasHalfEdges M = Mesh>
	void updateHalfEdgeReferences(
		const typename M::HalfEdgeType* oldBase,
		const typename M::HalfEdgeType* newBase);

	template<HasHalfEdges M = Mesh>
	void updateHalfEdgeReferencesAfterCompact(
		const typename M::HalfEdgeType* base,
		const std::vector<int>&         newIndices);

	void updateAllOptionalContainerReferences();

private:
	void addFaceHelper(typename Mesh<Args...>::FaceType& f);

	template<typename... V>
	void addFaceHelper(
		typename Mesh<Args...>::FaceType&   f,
		typename Mesh<Args...>::VertexType* v,
		V... args);

	template<typename... V>
	void addFaceHelper(typename Mesh<Args...>::FaceType& f, uint vid, V... args);

	template<typename Cont, typename OthMesh>
	void importReferences(const OthMesh& m);

	template<typename OthMesh>
	void manageImportTriFromPoly(const OthMesh& m);

	template<typename FaceType, typename MFaceType, typename VertexType, typename MVertexType>
	static void importTriReferencesHelper(
		FaceType&                f,
		const MFaceType&         mf,
		VertexType*              base,
		const MVertexType*       mvbase,
		const std::vector<uint>& tris,
		uint                     basetri);
};

template<typename... A> requires HasVertices<A...>
inline void swap(Mesh<A...>& m1, Mesh<A...>& m2);

} // namespace vcl

#include "mesh.cpp"

#endif // VCL_MESH_MESH_H
