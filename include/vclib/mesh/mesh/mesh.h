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

namespace internal {

template<uint i, typename Cont, typename Array, typename... A>
void setContainerBase(Mesh<A...>& m, Array& bases);

template<typename... A>
auto getContainerBases(Mesh<A...>& m);

template<typename Cont, typename Array, typename... A>
void updateReferencesOfContainerType(Mesh<A...>& m, Array& bases);

} // namespace vcl::internal

template<typename... A> requires HasVertices<A...>
inline void swap(Mesh<A...>& m1, Mesh<A...>& m2);

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
	// filter Components of the Mesh, taking only the Container
	// Containers is a vcl::TypeWrapper containing all the containers that were in Args
	// Containers are the types that satisfy the concept IsElementContainer
	//
	// Using this type, you can iterate over containers of a mesh and call generic functions on
	// them, regardless the actual type of the container
	using Containers = typename vcl::
		FilterTypesByCondition<mesh::IsElementContainerPred, vcl::TypeWrapper<Args...>>::type;

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
	template<uint i, typename Cont, typename Array, typename... A>
	friend void internal::setContainerBase(Mesh<A...>& m, Array& bases);

	/// @private
	template<typename Cont, typename Array, typename... A>
	friend void internal::updateReferencesOfContainerType(Mesh<A...>& m, Array& bases);

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
	template<typename Cont>
	uint addElement();

	template<typename Cont>
	uint addElements(uint n);

	template<typename Cont>
	void reserveElements(uint n);

	template<typename Cont>
	void compactElements();

	template<typename Cont>
	void clearElements();

	template<typename Cont, typename Element>
	void updateReferences(
		const Element* oldBase,
		const Element* newBase);

	template<typename Cont, typename Element>
	void updateReferencesAfterCompact(
		const Element*          base,
		const std::vector<int>& newIndices);

private:
	// hide init and isEnabled members
	void init() {};
	bool isEnabled() { return true; }

	void updateAllParentMeshPointers();

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

	// member functions used by friends

	template<typename El>
	auto& customComponents();

	template<typename El>
	const auto& customComponents() const;
	
	template<typename El>
	auto& verticalComponents();
	
	template<typename El>
	const auto& verticalComponents() const;
};

} // namespace vcl

#include "mesh.cpp"

#endif // VCL_MESH_MESH_MESH_H
