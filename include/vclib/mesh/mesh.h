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

#include "container/containers.h"
#include "mesh_components.h"

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
template<typename... Args>
class Mesh : public Args...
{
public:
	Mesh();
	Mesh(const Mesh& oth);
	Mesh(Mesh&& oth);

	void clear();

	// Vertices

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasVertices<M>(), uint)
	index(const typename M::VertexType& v) const;

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasVertices<M>(), uint)
	index(const typename M::VertexType* v) const;

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasVertices<M>(), uint)
	addVertex();

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasVertices<M>(), uint)
	addVertex(const typename M::VertexType::CoordType& p);

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasVertices<M>(), uint)
	addVertices(uint n);

	template<typename... VC, typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasVertices<M>(), uint)
	addVertices(const typename M::VertexType::CoordType& p, const VC&... v);

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasVertices<M>(), void)
	reserveVertices(uint n);

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasVertices<M>(), void)
	compactVertices();

	// Faces

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasFaces<M>(), uint)
	index(const typename M::FaceType& f) const;

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasFaces<M>(), uint)
	index(const typename M::FaceType* f) const;

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasFaces<M>(), uint)
	addFace();

	template<typename M = Mesh, typename... V>
	VCL_ENABLE_IF(mesh::hasFaces<M>() && mesh::hasVertices<M>(), uint)
	addFace(V... args);

	template<typename M = Mesh, typename Iterator>
	VCL_ENABLE_IF(mesh::hasFaces<M>() && mesh::hasVertices<M>(), uint)
	addFace(Iterator begin, Iterator end);

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasFaces<M>(), uint)
	addFaces(uint n);

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasFaces<M>(), void)
	reserveFaces(uint n);

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasFaces<M>(), void)
	compactFaces();

	// Edges

	template<typename M = Mesh<Args...>>
	VCL_ENABLE_IF(mesh::hasEdges<M>(), uint)
	index(const typename M::EdgeType& e) const;

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasEdges<M>(), uint)
	index(const typename M::EdgeType* v) const;

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasEdges<M>(), uint)
	addEdge();

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasEdges<M>(), uint)
	addEdges(uint n);

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasEdges<M>(), void)
	reserveEdges(uint n);

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasEdges<M>(), void)
	compactEdges();

	// HalfEdges

	template<typename M = Mesh<Args...>>
	VCL_ENABLE_IF(mesh::hasHalfEdges<M>(), uint)
	index(const typename M::HalfEdgeType& e) const;

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasHalfEdges<M>(), uint)
	index(const typename M::HalfEdgeType* v) const;

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasHalfEdges<M>(), uint)
	addHalfEdge();

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasHalfEdges<M>(), uint)
	addHalfEdges(uint n);

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasHalfEdges<M>() && mesh::hasFaces<M>(), uint)
	addHalfEdgesToFace(uint n, typename M::FaceType& f);

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasHalfEdges<M>(), void)
	reserveHalfEdges(uint n);

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasHalfEdges<M>(), void)
	compactHalfEdges();

	template<typename OtherMeshType>
	void enableSameOptionalComponentsOf(const OtherMeshType& m);

	template<typename OtherMeshType>
	void importFrom(const OtherMeshType& m);

	/// @private
	template<typename... A>
	friend void swap(Mesh<A...>& m1, Mesh<A...>& m2);

	void swap(Mesh& m2);

	Mesh& operator=(Mesh oth);

protected:
	// Vertices

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasVertices<M>(), void)
	updateVertexReferences(
		const typename M::VertexType* oldBase,
		const typename M::VertexType* newBase);

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasVertices<M>(), void)
	updateVertexReferencesAfterCompact(
		const typename M::VertexType* base,
		const std::vector<int>&       newIndices);

	// Faces

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasFaces<M>(), void)
	updateFaceReferences(const typename M::FaceType* oldBase, const typename M::FaceType* newBase);

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasFaces<M>(), void)
	updateFaceReferencesAfterCompact(
		const typename M::FaceType* base,
		const std::vector<int>&     newIndices);

	// Edges

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasEdges<M>(), void)
	updateEdgeReferences(const typename M::EdgeType* oldBase, const typename M::EdgeType* newBase);

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasEdges<M>(), void)
	updateEdgeReferencesAfterCompact(
		const typename M::EdgeType* base,
		const std::vector<int>&     newIndices);

	// HalfEdges

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasHalfEdges<M>(), void)
	updateHalfEdgeReferences(
		const typename M::HalfEdgeType* oldBase,
		const typename M::HalfEdgeType* newBase);

	template<typename M = Mesh>
	VCL_ENABLE_IF(mesh::hasHalfEdges<M>(), void)
	updateHalfEdgeReferencesAfterCompact(
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

template<typename... A>
inline void swap(Mesh<A...>& m1, Mesh<A...>& m2);

} // namespace vcl

#include "mesh.cpp"

#endif // VCL_MESH_MESH_H
