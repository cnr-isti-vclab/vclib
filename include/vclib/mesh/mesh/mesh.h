/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
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

#ifndef VCL_MESH_MESH_MESH_H
#define VCL_MESH_MESH_MESH_H

#include <vclib/mesh/containers/containers.h>
#include <vclib/mesh/elements/element_concept.h>

#include "mesh_components.h"
#include "../requirements/mesh_concepts.h"

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
	template<typename El, bool b>
	friend struct comp::internal::CustomComponentsData;

	template<typename El, bool b>
	friend struct comp::internal::ComponentData;

public:
	Mesh();
	Mesh(const Mesh& oth);
	Mesh(Mesh&& oth);

	void clear();

	void compact();

	template<typename OtherMeshType>
	void enableSameOptionalComponentsOf(const OtherMeshType& m);

	template<typename OtherMeshType>
	void importFrom(const OtherMeshType& m);

	/// @private
	template<typename... A> requires HasVertices<A...>
	friend void swap(Mesh<A...>& m1, Mesh<A...>& m2);

	void swap(Mesh& m2);

	Mesh& operator=(Mesh oth);

	/*** Vertices ***/

	uint index(const typename Mesh::VertexType& v) const;
	uint index(const typename Mesh::VertexType* v) const;
	uint addVertex();
	uint addVertex(const typename Mesh::VertexType::CoordType& p);
	uint addVertices(uint n);

	template<typename... VC>
	uint addVertices(const typename Mesh::VertexType::CoordType& p, const VC&... v);

	void reserveVertices(uint n);
	void compactVertices();

	/*** Faces ***/

	template<HasFaces M = Mesh>
	uint index(const typename M::FaceType& f) const;

	template<HasFaces M = Mesh>
	uint index(const typename M::FaceType* f) const;

	template<HasFaces M = Mesh>
	uint addFace();

	template<HasFaces M = Mesh, typename... V>
	uint addFace(V... args);

	template<HasFaces M = Mesh, typename Iterator>
	uint addFace(Iterator begin, Iterator end);

	template<HasFaces M = Mesh>
	uint addFaces(uint n);

	template<HasFaces M = Mesh>
	void reserveFaces(uint n);

	template<HasFaces M = Mesh>
	void compactFaces();

	// functions that could involve other components
	// need to be here and not in the FaceContainer due to the possibility to be in a Dcel

	// WedgeColors
	template<HasFaces M = Mesh>
	bool isPerFaceWedgeColorsEnabled() const requires internal::OptionalWedgeColorsConcept<M>;

	template<HasFaces M = Mesh>
	void enablePerFaceWedgeColors() requires internal::OptionalWedgeColorsConcept<M>;

	template<HasFaces M = Mesh>
	void disablePerFaceWedgeColors() requires internal::OptionalWedgeColorsConcept<M>;

	// WedgeTexCoords
	template<HasFaces M = Mesh>
	bool isPerFaceWedgeTexCoordsEnabled() const requires internal::OptionalWedgeTexCoordsConcept<M>;

	template<HasFaces M = Mesh>
	void enablePerFaceWedgeTexCoords() requires internal::OptionalWedgeTexCoordsConcept<M>;

	template<HasFaces M = Mesh>
	void disablePerFaceWedgeTexCoords() requires internal::OptionalWedgeTexCoordsConcept<M>;

	/*** Edges ***/

	template<HasEdges M = Mesh>
	uint index(const typename M::EdgeType& e) const;

	template<HasEdges M = Mesh>
	uint index(const typename M::EdgeType* v) const;

	template<HasEdges M = Mesh>
	uint addEdge();

	template<HasEdges M = Mesh>
	uint addEdges(uint n);

	template<HasEdges M = Mesh>
	void reserveEdges(uint n);

	template<HasEdges M = Mesh>
	void compactEdges();

	/*** HalfEdges ***/

	template<HasHalfEdges M = Mesh>
	uint index(const typename M::HalfEdgeType& e) const;

	template<HasHalfEdges M = Mesh>
	uint index(const typename M::HalfEdgeType* v) const;

	template<HasHalfEdges M = Mesh>
	uint addHalfEdge();

	template<HasHalfEdges M = Mesh>
	uint addHalfEdges(uint n);

	template<typename M = Mesh> requires HasHalfEdges<M> && HasFaces<M>
	uint addHalfEdgesToFace(uint n, typename M::FaceType& f);

	template<HasHalfEdges M = Mesh>
	void reserveHalfEdges(uint n);

	template<HasHalfEdges M = Mesh>
	void compactHalfEdges();

protected:
	// Vertices
	template<typename Cont>
	void updateVertexReferences(
		const typename Mesh::VertexType* oldBase,
		const typename Mesh::VertexType* newBase);

	template<typename Cont>
	void updateVertexReferencesAfterCompact(
		const typename Mesh::VertexType* base,
		const std::vector<int>&       newIndices);

	// Faces
	template<typename Cont, HasFaces M = Mesh>
	void updateFaceReferences(
		const typename M::FaceType* oldBase,
		const typename M::FaceType* newBase);

	template<typename Cont, HasFaces M = Mesh>
	void updateFaceReferencesAfterCompact(
		const typename M::FaceType* base,
		const std::vector<int>&     newIndices);

	// Edges
	template<typename Cont, HasEdges M = Mesh>
	void updateEdgeReferences(
		const typename M::EdgeType* oldBase,
		const typename M::EdgeType* newBase);

	template<typename Cont, HasEdges M = Mesh>
	void updateEdgeReferencesAfterCompact(
		const typename M::EdgeType* base,
		const std::vector<int>&     newIndices);

	// HalfEdges
	template<typename Cont, HasHalfEdges M = Mesh>
	void updateHalfEdgeReferences(
		const typename M::HalfEdgeType* oldBase,
		const typename M::HalfEdgeType* newBase);

	template<typename Cont, HasHalfEdges M = Mesh>
	void updateHalfEdgeReferencesAfterCompact(
		const typename M::HalfEdgeType* base,
		const std::vector<int>&         newIndices);

	void updateAllParentMeshPointers();

private:
	// hide init and isEnabled members
	void init() {};
	bool isEnabled() { return true; }

	template<typename Cont>
	void clearContainer();

	template<typename Cont>
	void compactContainer();

	template<HasFaces M = Mesh>
	void addFaceHelper(typename M::FaceType& f);

	template<HasFaces M = Mesh, typename... V>
	void addFaceHelper(
		typename M::FaceType&   f,
		typename Mesh<Args...>::VertexType* v,
		V... args);

	template<HasFaces M = Mesh, typename... V>
	void addFaceHelper(typename M::FaceType& f, uint vid, V... args);

	template<typename Cont>
	void setParentMeshPointers();
	
	template<typename Cont, typename OthMesh>
	void importContainersAndComponents(const OthMesh& m);

	template<typename Cont, typename OthMesh>
	void importReferences(const OthMesh& m);

	template<typename OthMesh>
	void manageImportTriFromPoly(const OthMesh& m);

	template<typename OthMesh>
	void manageImportDcelFromMesh(const OthMesh& m);

	template<typename FaceType, typename MFaceType, typename VertexType, typename MVertexType>
	static void importTriReferencesHelper(
		FaceType&                f,
		const MFaceType&         mf,
		VertexType*              base,
		const MVertexType*       mvbase,
		const std::vector<uint>& tris,
		uint                     basetri);

	template<typename El>
	auto& customComponents() requires ElementConcept<El>;

	template<typename El>
	const auto& customComponents() const requires ElementConcept<El>;
	
	template<typename El>
	auto& verticalComponents() requires ElementConcept<El>;
	
	template<typename El>
	const auto& verticalComponents() const requires ElementConcept<El>;
};

template<typename... A> requires HasVertices<A...>
inline void swap(Mesh<A...>& m1, Mesh<A...>& m2);

} // namespace vcl

#include "mesh.cpp"

#endif // VCL_MESH_MESH_MESH_H
