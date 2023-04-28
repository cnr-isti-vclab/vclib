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

#include "mesh.h"

#include <vclib/views/mesh.h>
#include <vclib/space/polygon.h>

namespace vcl {

/**
 * @brief Empty constructor, constructs an empty mesh.
 */
template<typename... Args> requires HasVertices<Args...>
Mesh<Args...>::Mesh()
{
}

/**
 * @brief Copy constructor of the Mesh. Will create a deep copy of the given input mesh,
 * taking care of copying everithing and then update all the pointers
 *
 * @param oth: the mesh from which constructo this Mesh.
 */
template<typename... Args> requires HasVertices<Args...>
Mesh<Args...>::Mesh(const Mesh<Args...>& oth) :
		Args(oth)... // call auto copy constructors for all the container elements and components
{
	// Set to all elements their parent mesh (this)
	updateAllParentMeshPointers();

	// save base pointer of each container of the other mesh
	constexpr uint N_CONTAINERS = NumberOfTypes<typename Mesh<Args...>::Containers>::value;
	std::array<const void*, N_CONTAINERS> othBases = Mesh<Args...>::getContainerBases(oth);

	// update all the pointers contained on each container
	// use the base pointer of each container of oth to update all the pointers in this mesh
	// each pointer of oth that was copied in this mesh, will be updated computing its offset wrt
	// the base of oth, and then adding that offset to the new base pointer of this mesh
	(Mesh<Args...>::template updatePointersOfContainerType<Args>(*this, othBases), ...);
}

/**
 * @brief Move constructor, moves the given mesh into this one, without any other
 * resource acquisition.
 *
 * @param oth: the mesh that will be moved into this Mesh.
 */
template<typename... Args> requires HasVertices<Args...>
Mesh<Args...>::Mesh(Mesh<Args...>&& oth)
{
	swap(oth); // use copy ans swap idiom: this (empty) mesh is swapped with the input one
}

/**
 * @brief Clears all the Elements contained in the mesh.
 * @todo manage also other components
 */
template<typename... Args> requires HasVertices<Args...>
void Mesh<Args...>::clear()
{
	(clearElements<Args>(), ...);
}

template<typename... Args> requires HasVertices<Args...>
void Mesh<Args...>::compact()
{
	(compactElements<Args>(), ...);
}

/**
 * @brief The HasContainerOf struct sets the bool `value` to true if this Mesh has a container
 * of elements having the same Element ID of the template Element El.
 *
 * This means that this the only value checked is the ELEMENT_TYPE unsigned int exposed by the
 * Element, meaning that it does not check if the Elements of this mesh are exactly the same of
 * El.
 *
 * In other words, it returns true also if we pass an Element of another mesh that is of the
 * same ELEMENT_TYPE (both Vertices, Faces, ecc).
 *
 * Example of usage (Note: EdgeMesh has Vertices, but not Faces):
 *
 * @code{.cpp}
 * static_assert(vcl::EdgeMesh::hasContainerOf<vcl::TriMesh::Vertex>(),
 *					"EdgeMesh does not have Vertices");
 * static_assert(!vcl::EdgeMesh::hasContainerOf<vcl::TriMesh::Face>(),
 *					"EdgeMesh has Faces");
 * @endcode
 *
 * HasContainerOf sets its value to true when El is the TriMesh::Vertex, because EdgeMesh has
 * a Container of Vertices (Vertices of TriMesh and EdgeMesh are defined in different ways, but
 * they have the same ELEMENT_TYPE id).
 * HasContainerOf sets its value to false when El is the TriMesh::Face, because EdgeMesh does
 * not have a Container of Faces.
 */
template<typename... Args> requires HasVertices<Args...>
template<ElementConcept El>
constexpr bool Mesh<Args...>::hasContainerOf()
{
	return HasContainerOfPred<El>::value;
}

/**
 * @brief Enables all the OptionalComponents of this mesh according to the Components available
 * on the OtherMeshType m.
 *
 * This function is useful to call before importing data from another MeshType, to be sure that
 * all the available data contained in the MeshType mesh will be imported.
 *
 * This function:
 * - disables all the optional components that are not available in m
 * - enables all the optional components that are available in m (which can be both optional or not)
 *
 * Example of usage:
 *
 * @code{.cpp}
 * MeshType m1;
 * OtherMeshType m2;
 *
 * // do stuff on m2
 *
 * m1.enableSameOptionalComponentsOf(m2); // m1 enables all the available components of m2
 * m1.importFrom(m2); // m1 will import all the data contained in m2 that can be stored in m1
 * @endcode
 *
 * @param m
 */
template<typename... Args> requires HasVertices<Args...>
template<typename OtherMeshType>
void Mesh<Args...>::enableSameOptionalComponentsOf(const OtherMeshType& m)
{
	// enable all optional components of this Mesh depending on what's available in the
	// OtherMeshType

	(enableSameOptionalComponentsOf<Args>(m), ...);
}

/**
 * @brief Imports all the components that can be imported from another type of mesh.
 *
 * This function can be called from any Mesh type having all the Elements and Components that
 * implement the member function importFrom.
 *
 * Note that this function does not enable optional components that are disabled.
 * If you want to import all the possible data including also disabled components of this mesh, you
 * should call the function m1.enableSameOptionalComponentsOf(m2) before this function.
 *
 * @param[in] m: the mesh from which import all the data.
 */
template<typename... Args> requires HasVertices<Args...>
template<typename OtherMeshType>
void Mesh<Args...>::importFrom(const OtherMeshType& m)
{
	// This function will first:
	// Call, for each Container and Component of the mesh, its importFrom function.
	// In case of containers, it first creates the same number of elements in the container,
	// and then calls the importFrom function for each new element.
	// Pointers are not managed here, since they need additional parameters to be imported
	
	(importContainersAndComponents<Args>(m), ...);

	// Set to all elements their parent mesh (this)
	updateAllParentMeshPointers();

	// after importing ordinary components, we need to convert the pointers between containers.
	// each container can import more than one pointer type, e.g.:
	// - VertexContainer could import vertex pointers (adjacent vertices), face pointers
	//   (adjacent faces), and so on;
	// - FaceContainer will always import vertex pointers, but could also import face pointers
	//   (adjacent faces), edge pointers (adjacent edges)...
	// for each container of this Mesh, we'll call the importPointers passing the container (Args)
	// as template parameter. This parameter will be used to call all the possible import functions
	// available (vertices, faces, edges, half edges)

	(importPointers<Args>(m), ...);

	if constexpr(mesh::HasFaceContainer<Mesh<Args...>> && mesh::HasFaceContainer<OtherMeshType>) {
		// Now I need to manage imports between different types of meshes (same type of meshes are
		// already managed from importFrom and importPointersFrom member functions).
		//
		// Generally speaking, Polygon or Dcel meshes can import from any other type of mesh.
		// We need to take care when this mesh has static vertex pointers number in the face
		// container (VERTEX_NUMBER >= 3).
		//
		// The follwing case don't need to be managed:
		// - import polygon non-dcel mesh from triangle mesh
		//
		// I can manage the following cases:
		// - import triangle mesh from polygon mesh (also dcel): need triangulation
		// - import dcel from non-dcel mesh (need to add half edges into the dcel)
		//
		// I cannot manage the follwing cases:
		// - import static non-triangle mesh from polygon mesh or from a mesh with different
		//   VERTEX_NUMBER

		// if this is not a Dcel Mesh
		if constexpr (!DcelMeshConcept<Mesh<Args...>>) {
			// in case of import from poly (could be also dcel) to triangle mesh, I need to manage
			// triangulation of polygons and create additional triangle faces for each of the
			// imported polygons. This function statically asserts that the import can be done.
			manageImportTriFromPoly(m);
		}
		else { // if this is a dcel mesh
			// manage import from another non-dcel mesh (no need to manage the import from another
			// dcel)
			if constexpr(!DcelMeshConcept<OtherMeshType>) {
				manageImportDcelFromMesh(m);
			}
		}

	}
}

/**
 * @brief Swaps this mesh with the other input Mesh m2.
 * @param m2: the Mesh to swap with this Mesh.
 */
template<typename... Args> requires HasVertices<Args...>
void Mesh<Args...>::swap(Mesh& m2)
{
	Mesh<Args...>& m1 = *this;

	constexpr uint N_CONTAINERS = NumberOfTypes<typename Mesh<Args...>::Containers>::value;
	static_assert(N_CONTAINERS != 0);

	// container bases of each container for m1 and m2
	// we save the bases of the containers before swap
	std::array<const void*, N_CONTAINERS> m1Bases = Mesh<Args...>::getContainerBases(m1);
	std::array<const void*, N_CONTAINERS> m2Bases = Mesh<Args...>::getContainerBases(m2);

	// actual swap of all the containers and the components of the mesh
	// using pack expansion: swap will be called for each of the containers (or components!) that
	// compose the Mesh
	using std::swap;
	(swap((Args&) m1, (Args&) m2), ...);

	// Set to all elements their parent mesh
	m1.updateAllParentMeshPointers();
	m2.updateAllParentMeshPointers();

	// update all the pointers to m1 and m2: old base of m1 is now "old base" of m2, and viceversa
	(Mesh<Args...>::template updatePointersOfContainerType<Args>(m1, m2Bases), ...);
	(Mesh<Args...>::template updatePointersOfContainerType<Args>(m2, m1Bases), ...);
}

/**
 * @brief Assignment operator of the Mesh.
 * @param oth: the Mesh from which will create a copy and assign to this Mesh
 * @return a reference to this Mesh after the assignemnt.
 */
template<typename... Args> requires HasVertices<Args...>
Mesh<Args...>& Mesh<Args...>::operator=(Mesh<Args...> oth)
{
	swap(oth);
	return *this;
}

/**
 * @brief Returns the index of the given vertex in the VertexContainer of the Mesh.
 * @param v: a reference of a vertex of the Mesh.
 * @return the index of the given vertex.
 */
template<typename... Args> requires HasVertices<Args...>
uint Mesh<Args...>::index(const typename Mesh::VertexType& v) const
{
	using VertexContainer = typename Mesh::VertexContainer;
	return VertexContainer::index(&v);
}

/**
 * @brief Returns the index of the given vertex in the VertexContainer of the Mesh.
 * @param v: a pointer of a vertex of the Mesh.
 * @return the index of the given vertex.
 */
template<typename... Args> requires HasVertices<Args...>
uint Mesh<Args...>::index(const typename Mesh::VertexType* v) const
{
	using VertexContainer = typename Mesh::VertexContainer;
	return VertexContainer::index(v);
}

/**
 * @brief Add a new vertex into the mesh, returning the index of the added vertex.
 *
 * If the call of this function will cause a reallocation of the Vertex container, the function
 * will automatically take care of updating all the Vertex pointers contained in the Mesh.
 *
 * @return the index of the new vertex.
 */
template<typename... Args> requires HasVertices<Args...>
uint Mesh<Args...>::addVertex()
{
	using VertexContainer = typename Mesh::VertexContainer;

	return addElement<VertexContainer>();
}

/**
 * @brief Add a new vertex with the given coordinate into the mesh, returning the id of the added
 * vertex.
 *
 * If the call of this function will cause a reallocation of the Vertex container, the function
 * will automatically take care of updating all the Vertex pointers contained in the Mesh.
 *
 * @param p: coordinate of the new vertex.
 * @return the id of the new vertex.
 */
template<typename... Args> requires HasVertices<Args...>
uint Mesh<Args...>::addVertex(const typename Mesh::VertexType::CoordType& p)
{
	using VertexContainer = typename Mesh::VertexContainer;

	uint vid = addVertex();                   // using the previously defined addVertex function
	VertexContainer::vertex(vid).coord() = p; // set the coordinate to the vertex
	return vid;
}

/**
 * @brief Add an arbitrary number of n vertices, returning the id of the first added vertex.
 *
 * This means that, if you want to add 5 vertices and this member function returns 4, the added
 * vertices will have id from 4 to id 8 included.
 *
 * If the call of this function will cause a reallocation of the Vertex container, the function
 * will automatically take care of updating all the Vertex pointers contained in the Mesh.
 *
 * @param n: the number of vertices to add to the mesh.
 * @return the id of the first added vertex.
 */
template<typename... Args> requires HasVertices<Args...>
uint Mesh<Args...>::addVertices(uint n)
{
	using VertexContainer = typename Mesh::VertexContainer;

	return addElements<VertexContainer>(n);
}

/**
 * @brief Add an arbitrary number of vertices with the given coordinates, returning the id of the
 * first added vertex.
 *
 * You can call this member function like:
 *
 * @code{.cpp}
 * CoordType p0, p1, p2, p3;
 * // init coords...
 * m.addVertices(p0, p1, p2, p3);
 * @endcode
 *
 * The number of accepted Coordtype arguments is variable.
 *
 * If the call of this function will cause a reallocation of the Vertex container, the function
 * will automatically take care of updating all the Vertex pointers contained in the Mesh.
 *
 * @param p: first vertex coordinate
 * @param v: list of other vertex coordinates
 * @return the id of the first added vertex.
 */
template<typename... Args> requires HasVertices<Args...>
template<typename... VC>
uint Mesh<Args...>::addVertices(
	const typename Mesh::VertexType::CoordType& p,
	const VC&... v) // parameter pack of points
{
	using VertexContainer = typename Mesh::VertexContainer;
	uint vid              = VertexContainer::vertexContainerSize();
	reserveVertices(vid + sizeof...(VC) + 1); // reserve the new number of vertices

	addVertex(p);
	// pack expansion: will be translated at compile time as an addVertex() call for each argument
	// of the addVertices member function
	(addVertex(v), ...);

	return vid;
}

/**
 * @brief Reserve a number of vertices in the container of Vertices. This is useful when you know
 * (or you have an idea) of how much vertices are going to add into a newly of existing mesh.
 * Calling this function before any add_vertex() call will avoid unuseful reallocations of the
 * container, saving execution time.
 *
 * The filosofy of this function is similar to the one of the
 * [reserve()](https://en.cppreference.com/w/cpp/container/vector/reserve) function of the
 * [std::vector class](https://en.cppreference.com/w/cpp/container/vector).
 *
 * If the call of this function will cause a reallocation of the Vertex container, the function
 * will automatically take care of updating all the Vertex pointers contained in the Mesh.
 *
 * @param n: the new capacity of the vertex container.
 */
template<typename... Args> requires HasVertices<Args...>
void Mesh<Args...>::reserveVertices(uint n)
{
	using VertexContainer = typename Mesh::VertexContainer;

	reserveElements<VertexContainer>(n);
}

/**
 * @brief Compacts the Vertex Container, removing all the vertices marked as deleted. Vertices
 * indices will change accordingly. The function will automatically take care of updating all the
 * Vertex pointers contained in the Mesh.
 */
template<typename... Args> requires HasVertices<Args...>
void Mesh<Args...>::compactVertices()
{
	using VertexContainer = typename Mesh::VertexContainer;

	compactElements<VertexContainer>();
}

/**
 * @brief Returns the index of the given face in the FaceContainer of the Mesh.
 *
 * This function will be available only **if the Mesh has the Face Container**.
 *
 * @param f: a reference of a face of the Mesh.
 * @return the index of the given face.
 */
template<typename... Args> requires HasVertices<Args...>
template<HasFaces M>
uint Mesh<Args...>::index(const typename M::FaceType& f) const
{
	using FaceContainer = typename M::FaceContainer;
	return FaceContainer::index(&f);
}

/**
 * @brief Returns the index of the given face in the FaceContainer of the Mesh.
 *
 * This function will be available only **if the Mesh has the Face Container**.
 *
 * @param f: a pointer of a face of the Mesh.
 * @return the index of the given face.
 */
template<typename... Args> requires HasVertices<Args...>
template<HasFaces M>
uint Mesh<Args...>::index(const typename M::FaceType* f) const
{
	using FaceContainer = typename M::FaceContainer;
	return FaceContainer::index(f);
}

template<typename... Args> requires HasVertices<Args...>
template<HasFaces M>
uint Mesh<Args...>::addFace()
{
	using FaceContainer = typename M::FaceContainer;

	return addElement<FaceContainer>();
}

template<typename... Args> requires HasVertices<Args...>
template<HasFaces M, typename... V>
uint Mesh<Args...>::addFace(V... args)
{
	using Face          = typename M::FaceType;
	using FaceContainer = typename M::FaceContainer;

	uint  fid = addFace();
	Face& f   = FaceContainer::face(fid);

	constexpr uint n = sizeof...(args);
	static_assert(n >= 3, "Faces must have at least 3 vertices");
	if constexpr (Face::VERTEX_NUMBER < 0) {
		if constexpr (!comp::HasFaceHalfEdgePointers<Face>) {
			f.resizeVertices(n);
		}
		else {
			addHalfEdgesToFace(n, f);
		}
	}
	else {
		static_assert(n == Face::VERTEX_NUMBER, "Wrong number of vertices in Mesh::addFace.");
	}

	addFaceHelper(f, args...);
	return fid;
}

template<typename... Args> requires HasVertices<Args...>
template<HasFaces M, typename Iterator>
uint Mesh<Args...>::addFace(Iterator begin, Iterator end)
{
	using Face          = typename M::FaceType;
	using FaceContainer = typename M::FaceContainer;
	using VertexContainer = typename Mesh<Args...>::VertexContainer;

	if (begin == end) return UINT_NULL;

	uint  fid = addFace();
	Face& f   = FaceContainer::face(fid);
	
	if constexpr (comp::HasFaceHalfEdgePointers<Face>) {
		using HalfEdgeContainer = typename Mesh<Args...>::HalfEdgeContainer;
		using HalfEdge          = typename HalfEdgeContainer::HalfEdgeType;
		using Vertex = typename Mesh<Args...>::VertexType;
		HalfEdge* first = nullptr;
		HalfEdge* prev = nullptr;
		HalfEdge* curr;
		for (Iterator it = begin; it != end; ++it) {
			Vertex* v = &VertexContainer::vertex(*it);
			uint heid = addHalfEdge();
			curr = &HalfEdgeContainer::halfEdge(heid);
			curr->fromVertex() = v;
			v->halfEdge() = curr;
			if (prev != nullptr) {
				prev->next() = curr;
				curr->prev() = prev;
			}
			if (first == nullptr)
				first = curr;
			prev = curr;
		}
		first->prev() = curr;
		curr->next() = first;
		f.outerHalfEdge() = first;
	}
	else {
		if constexpr (Face::VERTEX_NUMBER < 0) {
			uint n = std::distance(begin, end);
			f.resizeVertices(n);
		}

		unsigned int i = 0;
		for (Iterator it = begin; it != end; ++it) {
			f.vertex(i) = &VertexContainer::vertex(*it);
			++i;
		}
	}

	return fid;
}

template<typename... Args> requires HasVertices<Args...>
template<HasFaces M>
uint Mesh<Args...>::addFaces(uint n)
{
	using FaceContainer = typename M::FaceContainer;

	return addElements<FaceContainer>(n);
}

template<typename... Args> requires HasVertices<Args...>
template<HasFaces M>
void Mesh<Args...>::reserveFaces(uint n)
{
	using FaceContainer = typename M::FaceContainer;

	reserveElements<FaceContainer>(n);
}

template<typename... Args> requires HasVertices<Args...>
template<HasFaces M>
void Mesh<Args...>::compactFaces()
{
	using FaceContainer = typename M::FaceContainer;

	compactElements<FaceContainer>();
}

template<typename... Args> requires HasVertices<Args...>
template<HasFaces M>
bool Mesh<Args...>::isPerFaceWedgeColorsEnabled() const
	requires internal::OptionalWedgeColorsConcept<M>
{
	// if there is a WedgeColors component in the Face element
	if constexpr (face::HasOptionalWedgeColors<typename M::FaceType>) {
		return M::FaceContainer::isPerFaceWedgeColorsEnabled(); // use the container function
	}
	else { // otherwise, due to the OptionalWedgeColorsConcept, we are in a Dcel
		// Dcel having half edges with optional color - to be used as wedge colors
		return M::HalfEdgeContainer::isPerHalfEdgeColorEnabled();
	}
}

template<typename... Args> requires HasVertices<Args...>
template<HasFaces M>
void Mesh<Args...>::enablePerFaceWedgeColors() requires internal::OptionalWedgeColorsConcept<M>
{
	// if there is a WedgeColors component in the Face element
	if constexpr (face::HasOptionalWedgeColors<typename M::FaceType>) {
		M::FaceContainer::enablePerFaceWedgeColors(); // use the container function
	}
	else { // otherwise, due to the OptionalWedgeColorsConcept, we are in a Dcel
		// Dcel having half edges with optional color - to be used as wedge colors
		return M::HalfEdgeContainer::enablePerHalfEdgeColor();
	}
}

template<typename... Args> requires HasVertices<Args...>
template<HasFaces M>
void Mesh<Args...>::disablePerFaceWedgeColors() requires internal::OptionalWedgeColorsConcept<M>
{
	// if there is a WedgeColors component in the Face element
	if constexpr (face::HasOptionalWedgeColors<typename M::FaceType>) {
		M::FaceContainer::disablePerFaceWedgeColors(); // use the container function
	}
	else { // otherwise, due to the OptionalWedgeColorsConcept, we are in a Dcel
		// Dcel having half edges with optional color - to be used as wedge colors
		return M::HalfEdgeContainer::disablePerHalfEdgeColor();
	}
}

template<typename... Args> requires HasVertices<Args...>
template<HasFaces M>
bool Mesh<Args...>::isPerFaceWedgeTexCoordsEnabled() const
	requires internal::OptionalWedgeTexCoordsConcept<M>
{
	// if there is a WedgeTexCoords component in the Face element
	if constexpr (face::HasOptionalWedgeTexCoords<typename M::FaceType>) {
		return M::FaceContainer::isPerFaceWedgeTexCoordsEnabled(); // use the container function
	}
	else { // otherwise, due to the OptionalWedgeTexCoordsConcept, we are in a Dcel
		// Dcel having half edges with optional tex coords - to be used as wedge tex coords
		return M::HalfEdgeContainer::isPerHalfEdgeTexCoordEnabled();
	}
}

template<typename... Args> requires HasVertices<Args...>
template<HasFaces M>
void Mesh<Args...>::enablePerFaceWedgeTexCoords()
	requires internal::OptionalWedgeTexCoordsConcept<M>
{
	// if there is a WedgeTexCoords component in the Face element
	if constexpr (face::HasOptionalWedgeTexCoords<typename M::FaceType>) {
		M::FaceContainer::enablePerFaceWedgeTexCoords(); // use the container function
	}
	else { // otherwise, due to the OptionalWedgeTexCoordsConcept, we are in a Dcel
		// Dcel having half edges with optional tex coords - to be used as wedge tex coords
		return M::HalfEdgeContainer::enablePerHalfEdgeTexCoord();
	}
}

template<typename... Args> requires HasVertices<Args...>
template<HasFaces M>
void Mesh<Args...>::disablePerFaceWedgeTexCoords()
	requires internal::OptionalWedgeTexCoordsConcept<M>
{
	// if there is a WedgeTexCoords component in the Face element
	if constexpr (face::HasOptionalWedgeTexCoords<typename M::FaceType>) {
		M::FaceContainer::disablePerFaceWedgeTexCoords(); // use the container function
	}
	else { // otherwise, due to the OptionalWedgeTexCoordsConcept, we are in a Dcel
		// Dcel having half edges with optional tex coords - to be used as wedge tex coords
		return M::HalfEdgeContainer::disablePerHalfEdgeTexCoord();
	}
}

/**
 * @brief Returns the index of the given edge in the EdgeContainer of the Mesh.
 * @param e: a reference of an edge of the Mesh.
 * @return the index of the given edge.
 */
template<typename... Args> requires HasVertices<Args...>
template<HasEdges M>
uint Mesh<Args...>::index(const typename M::EdgeType& e) const
{
	using EdgeContainer = typename M::EdgeContainer;
	return EdgeContainer::index(&e);
}

/**
 * @brief Returns the index of the given edge in the EdgeContainer of the Mesh.
 * @param e: a pointer of an edge of the Mesh.
 * @return the index of the given edge.
 */
template<typename... Args> requires HasVertices<Args...>
template<HasEdges M>
uint Mesh<Args...>::index(const typename M::EdgeType* e) const
{
	using EdgeContainer = typename M::EdgeContainer;
	return EdgeContainer::index(e);
}

template<typename... Args> requires HasVertices<Args...>
template<HasEdges M>
uint Mesh<Args...>::addEdge()
{
	using EdgeContainer = typename M::EdgeContainer;

	return addElement<EdgeContainer>();
}

/**
 * @brief Add an arbitrary number of n edges, returning the id of the first added edge.
 *
 * This means that, if you want to add 5 edges and this member function returns 4, the added
 * edges will have id from 4 to id 8 included.
 *
 * If the call of this function will cause a reallocation of the EdgeContainer, the function
 * will automatically take care of updating all the Edge pointers contained in the Mesh.
 *
 * This function will be available only **if the Mesh has the Edge Container**.
 *
 * @param n: the number of edges to add to the mesh.
 * @return the id of the first added edge.
 */
template<typename... Args> requires HasVertices<Args...>
template<HasEdges M>
uint Mesh<Args...>::addEdges(uint n)
{
	using EdgeContainer = typename M::EdgeContainer;

	return addElements<EdgeContainer>(n);
}

/**
 * @brief Reserve a number of edges in the container of Edges. This is useful when you know
 * (or you have an idea) of how much edges are going to add into a newly of existing mesh.
 * Calling this function before any addEdge() call will avoid unuseful reallocations of the
 * container, saving execution time.
 *
 * The filosofy of this function is similar to the one of the
 * [reserve()](https://en.cppreference.com/w/cpp/container/vector/reserve) function of the
 * [std::vector class](https://en.cppreference.com/w/cpp/container/vector).
 *
 * If the call of this function will cause a reallocation of the Edge container, the function
 * will automatically take care of updating all the Edge pointers contained in the Mesh.
 *
 * This function will be available only **if the Mesh has the Edge Container**.
 *
 * @param n: the new capacity of the edge container.
 */
template<typename... Args> requires HasVertices<Args...>
template<HasEdges M>
void Mesh<Args...>::reserveEdges(uint n)
{
	using EdgeContainer = typename M::EdgeContainer;

	reserveElements<EdgeContainer>(n);
}

/**
 * @brief Compacts the EdgeContainer, removing all the edges marked as deleted. Edges indices
 * will change accordingly. The function will automatically take care of updating all the Edge
 * pointers contained in the Mesh.
 *
 * This function will be available only **if the Mesh has the Edge Container**.
 */
template<typename... Args> requires HasVertices<Args...>
template<HasEdges M>
void Mesh<Args...>::compactEdges()
{
	using EdgeContainer = typename M::EdgeContainer;

	compactElements<EdgeContainer>();
}

/**
 * @brief Returns the index of the given Halfedge in the HalfEdgeContainer of the Mesh.
 * @param e: a reference of an Halfedge of the Mesh.
 * @return the index of the given Halfedge.
 */
template<typename... Args> requires HasVertices<Args...>
template<HasHalfEdges M>
uint Mesh<Args...>::index(const typename M::HalfEdgeType& e) const
{
	using HalfEdgeContainer = typename M::HalfEdgeContainer;
	return HalfEdgeContainer::index(&e);
}

/**
 * @brief Returns the index of the given Halfedge in the HalfEdgeContainer of the Mesh.
 * @param e: a pointer of an Halfedge of the Mesh.
 * @return the index of the given Halfedge.
 */
template<typename... Args> requires HasVertices<Args...>
template<HasHalfEdges M>
uint Mesh<Args...>::index(const typename M::HalfEdgeType* e) const
{
	using HalfEdgeContainer = typename M::HalfEdgeContainer;
	return HalfEdgeContainer::index(e);
}

template<typename... Args> requires HasVertices<Args...>
template<HasHalfEdges M>
uint Mesh<Args...>::addHalfEdge()
{
	using HalfEdgeContainer = typename M::HalfEdgeContainer;

	return addElement<HalfEdgeContainer>();
}

/**
 * @brief Add an arbitrary number of n Halfedges, returning the id of the first added Halfedge.
 *
 * This means that, if you want to add 5 Halfedges and this member function returns 4, the added
 * Halfedges will have id from 4 to id 8 included.
 *
 * If the call of this function will cause a reallocation of the HalfEdgeContainer, the function
 * will automatically take care of updating all the HalfEdge pointers contained in the Mesh.
 *
 * This function will be available only **if the Mesh has the HalfEdge Container**.
 *
 * @param n: the number of Halfedges to add to the mesh.
 * @return the id of the first added Halfedge.
 */
template<typename... Args> requires HasVertices<Args...>
template<HasHalfEdges M>
uint Mesh<Args...>::addHalfEdges(uint n)
{
	using HalfEdgeContainer = typename M::HalfEdgeContainer;

	return addElements<HalfEdgeContainer>(n);
}

template<typename... Args> requires HasVertices<Args...>
template<typename M> requires HasHalfEdges<M> && HasFaces<M>
uint Mesh<Args...>::addHalfEdgesToFace(uint n, typename M::FaceType& f)
{
	using HalfEdge = typename Mesh<Args...>::HalfEdgeType;
	using HalfEdgeContainer = typename Mesh<Args...>::HalfEdgeContainer;

	uint first = addHalfEdges(n);
	HalfEdge* hef =  &HalfEdgeContainer::halfEdge(first);
	uint curr;
	HalfEdge* prev = nullptr;
	for (curr = first; curr < HalfEdgeContainer::halfEdgeNumber(); ++curr) {
		HalfEdge& he = HalfEdgeContainer::halfEdge(curr);
		he.face() = &f;
		if (prev != nullptr) {
			he.prev() = prev;
			prev->next() = &he;
		}
		prev = &he;
	}
	if (prev != nullptr) {
		hef->prev() = prev;
		prev->next() = hef;
	}
	f.outerHalfEdge() = hef;
	return first;
}

/**
 * @brief Reserve a number of Halfedges in the container of HalfEdges. This is useful when you know
 * (or you have an idea) of how much Halfedges are going to add into a newly of existing mesh.
 * Calling this function before any addHalfEdge() call will avoid unuseful reallocations of the
 * container, saving execution time.
 *
 * The filosofy of this function is similar to the one of the
 * [reserve()](https://en.cppreference.com/w/cpp/container/vector/reserve) function of the
 * [std::vector class](https://en.cppreference.com/w/cpp/container/vector).
 *
 * If the call of this function will cause a reallocation of the HalfEdge container, the function
 * will automatically take care of updating all the HalfEdge pointers contained in the Mesh.
 *
 * This function will be available only **if the Mesh has the HalfEdge Container**.
 *
 * @param n: the new capacity of the Halfedge container.
 */
template<typename... Args> requires HasVertices<Args...>
template<HasHalfEdges M>
void Mesh<Args...>::reserveHalfEdges(uint n)
{
	using HalfEdgeContainer = typename M::HalfEdgeContainer;

	reserveElements<HalfEdgeContainer>(n);
}

/**
 * @brief Compacts the HalfEdgeContainer, removing all the Halfedges marked as deleted. HalfEdges
 * indices will change accordingly. The function will automatically take care of updating all the
 * HalfEdge pointers contained in the Mesh.
 *
 * This function will be available only **if the Mesh has the HalfEdge Container**.
 */
template<typename... Args> requires HasVertices<Args...>
template<HasHalfEdges M>
void Mesh<Args...>::compactHalfEdges()
{
	using HalfEdgeContainer = typename M::HalfEdgeContainer;

	compactElements<HalfEdgeContainer>();
}

/*********************
 * Protected Members *
 *********************/

template<typename... Args> requires HasVertices<Args...>
template<typename Cont>
uint Mesh<Args...>::addElement()
{
	using Element = typename Cont::ElementType;

	// If the base pointer of the container of elements changes, it means that all the elements
	// pointers contained in the elements need to be updated

	Element* oldBase = Cont::vec.data();
	uint     eid     = Cont::addElement(this);
	Element* newBase = Cont::vec.data();
	if (oldBase != nullptr && oldBase != newBase) { // if true, pointer of container is changed
		// change all the element pointers in the containers
		(updatePointers<Args>(oldBase, newBase), ...);
	}
	return eid;
}

template<typename... Args> requires HasVertices<Args...>
template<typename Cont>
uint Mesh<Args...>::addElements(uint n)
{
	using Element = typename Cont::ElementType;

	// If the base pointer of the container of elements changes, it means that all the elements
	// pointers contained in the other elements need to be updated

	Element* oldBase = Cont::vec.data();
	uint     eid     = Cont::addElements(n, this); // add the number elements
	Element* newBase = Cont::vec.data();

	if (oldBase != nullptr && oldBase != newBase) { // if true, pointer of container is changed
		// change all the element pointers in the other containers
		(updatePointers<Args>(oldBase, newBase), ...);
	}
	return eid;
}

template<typename... Args> requires HasVertices<Args...>
template<typename Cont>
void Mesh<Args...>::reserveElements(uint n)
{
	using Element = typename Cont::ElementType;

	Element* oldBase = Cont::vec.data();
	Cont::reserveElements(n, this);
	Element* newBase = Cont::vec.data();
	if (oldBase != nullptr && oldBase != newBase)
		(updatePointers<Args>(oldBase, newBase), ...);
}

template<typename... Args> requires HasVertices<Args...>
template<typename Cont>
void Mesh<Args...>::compactElements()
{
	if constexpr(mesh::ElementContainerConcept<Cont>) {
		if (Cont::elementNumber() != Cont::elementContainerSize()) {
			auto* oldBase = Cont::vec.data();
			std::vector<int> newIndices = Cont::compactElements();
			auto* newBase = Cont::vec.data();
			assert(oldBase == newBase);

			(updatePointersAfterCompact<Args>(oldBase, newIndices), ...);
		}
	}
}

template<typename... Args> requires HasVertices<Args...>
template<typename Cont>
void Mesh<Args...>::clearElements()
{
	if constexpr(mesh::ElementContainerConcept<Cont>) {
		Cont::clearElements();
	}
}

template<typename... Args> requires HasVertices<Args...>
template<typename Cont, typename Element>
void Mesh<Args...>::updatePointers(
	const Element* oldBase,
	const Element* newBase)
{
	if constexpr(mesh::ElementContainerConcept<Cont>) {
		Cont::updatePointers(oldBase, newBase);
	}
}

template<typename... Args> requires HasVertices<Args...>
template<typename Cont, typename Element>
void Mesh<Args...>::updatePointersAfterCompact(
	const Element*          base,
	const std::vector<int>& newIndices)
{
	if constexpr(mesh::ElementContainerConcept<Cont>) {
		Cont::updatePointersAfterCompact(base, newIndices);
	}
}

template<typename... Args> requires HasVertices<Args...>
template<HasFaces M>
void Mesh<Args...>::addFaceHelper(typename M::FaceType&)
{
	// base case: no need to add any other vertices
}

template<typename... Args> requires HasVertices<Args...>
template<HasFaces M, typename... V>
void Mesh<Args...>::addFaceHelper(
	typename M::FaceType&   f,
	typename Mesh<Args...>::VertexType* v,
	V... args)
{
	using FaceContainer = typename Mesh<Args...>::FaceContainer;

	// position on which add the vertex
	const std::size_t n = f.vertexNumber() - sizeof...(args) - 1;
	f.vertex(n)         = v;   // set the vertex
	addFaceHelper(f, args...); // set the remanining vertices, recursive variadics
}

template<typename... Args> requires HasVertices<Args...>
template<HasFaces M, typename... V>
void Mesh<Args...>::addFaceHelper(typename M::FaceType& f, uint vid, V... args)
{
	using FaceContainer   = typename Mesh<Args...>::FaceContainer;
	using VertexContainer = typename Mesh<Args...>::VertexContainer;

	// position on which add the vertex
	const std::size_t n = f.vertexNumber() - sizeof...(args) - 1;
	f.vertex(n)         = &VertexContainer::vertex(vid); // set the vertex
	addFaceHelper(f, args...); // set the remanining vertices, recursive variadics
}

template<typename... Args> requires HasVertices<Args...>
template<typename Cont, typename OtherMeshType>
void Mesh<Args...>::enableSameOptionalComponentsOf(const OtherMeshType& m)
{
	if constexpr(mesh::ElementContainerConcept<Cont>) {
		Cont::enableOptionalComponentsOf(m);
	}
}

template<typename... Args> requires HasVertices<Args...>
void Mesh<Args...>::updateAllParentMeshPointers()
{
	(setParentMeshPointers<Args>(), ...);
}

template<typename... Args> requires HasVertices<Args...>
template<typename Cont>
void Mesh<Args...>::setParentMeshPointers()
{
	if constexpr(mesh::ElementContainerConcept<Cont>) {
		Cont::setParentMeshPointers(this);
	}
}

template<typename... Args> requires HasVertices<Args...>
template<typename Cont, typename OthMesh>
void Mesh<Args...>::importContainersAndComponents(const OthMesh &m)
{
	if constexpr(mesh::ElementContainerConcept<Cont>) {
		Cont::importFrom(m, this);
	}
	else {
		Cont::importFrom(m);
	}
}

/**
 * This function will import, for a given container of this mesh that is passed as a template
 * parameter Cont, all the pointers of all the elements from the other mesh m.
 */
template<typename... Args> requires HasVertices<Args...>
template<typename Cont, typename OthMesh>
void Mesh<Args...>::importPointers(const OthMesh &m)
{
	// will loop again on Args. Args will be the element pointers imported on Cont
	(importPointersOfElement<Cont, Args>(m), ...);
}

template<typename... Args> requires HasVertices<Args...>
template<typename Cont, typename ElemCont, typename OthMesh>
void Mesh<Args...>::importPointersOfElement(const OthMesh& m)
{
	// if Cont and ElemCont are containers (could be mesh components)
	if constexpr(mesh::ElementContainerConcept<Cont> && mesh::ElementContainerConcept<ElemCont>) {
		// import in Cont the ElemCont pointers from m
		Cont::importPointersFrom(m, ElemCont::vec.data());
	}
}

/**
 * @brief This function manages the case where we try to import into a TriMesh a PolyMesh
 * Faces have been already imported, but without vertex pointers and other components that
 * depend on the number of vertices (e.g. wedges)
 */
template<typename... Args> requires HasVertices<Args...>
template<typename OthMesh>
void Mesh<Args...>::manageImportTriFromPoly(const OthMesh &m)
{
	using VertexType = typename Mesh<Args...>::VertexType;
	using MVertexType = typename OthMesh::VertexType;
	using MCoordType = typename MVertexType::CoordType;
	using FaceType = typename Mesh<Args...>::FaceType;
	using MFaceType = typename OthMesh::FaceType;

	using VertexContainer = typename Mesh<Args...>::VertexContainer;
	using FaceContainer   = typename Mesh<Args...>::FaceContainer;

	// if this is not a triangle mesh nor a polygon mesh (meaning that we can't control the
	// number of vertex pointers in this mesh), and this mesh does not have the same
	// number of vertex pointers of the other, it means that we don't know how to convert
	// these type of meshes (e.g. we don't know how to convert a polygon mesh into a quad
	// mesh, or convert a quad mesh into a pentagonal mesh...)
	static_assert(
		!(FaceType::VERTEX_NUMBER != 3 && FaceType::VERTEX_NUMBER > 0 &&
		  FaceType::VERTEX_NUMBER != MFaceType::VERTEX_NUMBER),
		"Cannot import from that type of Mesh. Don't know how to convert faces.");

	// we need to manage conversion from poly or faces with cardinality > 3 (e.g. quads) to
	// triangle meshes. In this case, we triangulate the polygon using the earcut algorithm.
	if constexpr (
		FaceType::VERTEX_NUMBER == 3 &&
		(MFaceType::VERTEX_NUMBER > 3 || MFaceType::VERTEX_NUMBER < 0)) {

		VertexType* base = VertexContainer::vec.data();
		const MVertexType* mvbase = &m.vertex(0);

		for (const MFaceType& mf : m.faces()) {
			// if the current face has the same number of vertices of this faces (3), then
			// the vertex pointers have been correctly imported from the import pointers
			// function. The import pointers function does nothing when importing from a face
			// with at least 4 vertices
			if (mf.vertexNumber() != FaceType::VERTEX_NUMBER) {
				// triangulate mf; the first triangle of the triangulation will be
				// this->face(m.index(mf));
				// the other triangles will be added at the end of the container
				std::vector<uint> tris =
					Polygon<MCoordType>::earCut(mf.vertices() | vcl::views::coords);
				FaceType& f = FaceContainer::face(m.index(mf));
				importTriPointersHelper(f, mf, base, mvbase, tris, 0);

				// number of other faces to add
				uint nf = tris.size() / 3 - 1;
				uint fid = FaceContainer::addElements(nf, this);

				uint i = 3; // index that cycles into tris
				for (; fid < FaceContainer::faceContainerSize(); ++fid) {
					FaceType& f = FaceContainer::face(fid);
					importTriPointersHelper(f, mf, base, mvbase, tris, i);
					i+=3;
				}
			}
		}
	}
}

/**
 * @brief This function manages the case where we try to import into a Dcel another type of Mesh.
 * Faces have been already imported, but not vertex pointers (half edges still need to be created)
 * and other components that depend on the number of vertices (e.g. adjacent faces and wedges)
 */
template<typename... Args> requires HasVertices<Args...>
template<typename OthMesh>
void Mesh<Args...>::manageImportDcelFromMesh(const OthMesh &m)
{
	using VertexType = typename Mesh<Args...>::VertexType;
	using MVertexType = typename OthMesh::VertexType;
	using FaceType = typename Mesh<Args...>::FaceType;
	using MFaceType = typename OthMesh::FaceType;

	using VertexContainer = typename Mesh<Args...>::VertexContainer;
	using FaceContainer = typename Mesh<Args...>::FaceContainer;

	// base and mvbase are needed to convert vertex pointers from other to this mesh
	VertexType* base = VertexContainer::vec.data();
	const MVertexType* mvbase = &m.vertex(0);

	for (const MFaceType& mf : m.faces()) {
		// f is the face with same index of mf in mesh m
		FaceType& f = FaceContainer::face(m.index(mf));

		// add mf.vertexNumber() half edges to this mesh, and all these half edges are adjacent to
		// face f (all next and prev relations are set here, and therefore they will allow to
		// iterate over f components)
		addHalfEdgesToFace(mf.vertexNumber(), f);

		// this can be optimized
		// set each vertex of f computing the right pointers from mesh m and face mf
		for (uint j = 0; j < mf.vertexNumber(); ++j) {
			f.vertex(j) = base + (mf.vertex(j) - mvbase);
		}

		// todo: adjacent faces and wedges
	}
}

template<typename... Args> requires HasVertices<Args...>
template<typename FaceType, typename MFaceType, typename VertexType, typename MVertexType>
void Mesh<Args...>::importTriPointersHelper(
	FaceType&                f,
	const MFaceType&         mf,
	VertexType*              base,
	const MVertexType*       mvbase,
	const std::vector<uint>& tris,
	uint                     basetri)
{
	f.importFrom(mf); // import all the components from mf
	for (uint i = basetri, j = 0; i < basetri+3; i++, j++) {
		f.vertex(j) = base + (mf.vertex(tris[i]) - mvbase);

		// wedge colors
		if constexpr(face::HasWedgeColors<FaceType> && face::HasWedgeColors<MFaceType>) {
			if (comp::isWedgeColorsEnabledOn(f) && comp::isWedgeColorsEnabledOn(mf)) {
				f.wedgeColor(j) = mf.wedgeColor(tris[i]);
			}
		}

		// wedge texcoords
		if constexpr(face::HasWedgeTexCoords<FaceType> && face::HasWedgeTexCoords<MFaceType>) {
			if (comp::isWedgeTexCoordsEnabledOn(f) && comp::isWedgeTexCoordsEnabledOn(mf)) {
				f.wedgeTexCoord(j) =
					mf.wedgeTexCoord(tris[i])
						.template cast<typename FaceType::WedgeTexCoordType::ScalarType>();
			}
		}
	}
}

/**
 * This function sets the Ith position of the array bases, where I is an index of a container in
 * a TypeWrapper of containers
 *
 * In the Ith position will be placed the base pointer of the vector of the elements contained
 * in the container Cont.
 */
template<typename... Args> requires HasVertices<Args...>
template<uint I, typename Cont, typename Array, typename... A>
void Mesh<Args...>::setContainerBase(const Mesh<A...>& m, Array& bases)
{
	// since this function is called using pack expansion, it means that Cont could be a mesh
	// component and not a cointainer. We check if Cont is a container
	if constexpr (mesh::ElementContainerConcept<Cont>) {
		static_assert(I >= 0 && I != UINT_NULL);
		bases[I] = m.Cont::vec.data();
	}
}

template<typename... Args> requires HasVertices<Args...>
template<typename... A>
auto Mesh<Args...>::getContainerBases(const Mesh<A...>& m)
{
	using Containers = typename Mesh<A...>::Containers;

	// the number of containers in Mesh<A...>
	constexpr uint N_CONTAINERS = NumberOfTypes<Containers>::value;
	// each element of this array will contain the base pointer of the vector of elements contained
	// in each container of Mesh<A...>
	std::array<const void*, N_CONTAINERS> bases;

	// for each container/component of Mesh<A...>, we set call the function that sets
	// the base of the container in its index
	(setContainerBase<IndexInTypes<A, Containers>::value, A>(m, bases), ...);

	return bases;
}

/**
 * This function is called for each container of the mesh.
 *
 * In general, for each container, we need to update all the pointers contained in it,
 * that may of any element of the mesh (example: in the VertexContainer there could be
 * Vertex pointers, but also Face or Edge pointers).
 *
 * Here in this function, we loop into the containers of the Mesh m using pack expansion, and
 * we use the Cont type to choose which pointers type we are updating.
 *
 * bases contains the old bases (the ones of the other mesh) for each container.
 */
template<typename... Args> requires HasVertices<Args...>
template<typename Cont, typename Array, typename... A>
void Mesh<Args...>::updatePointersOfContainerType(Mesh<A...>& m, const Array& bases)
{
	// since this function is called using pack expansion, it means that Cont could be a mesh
	// component and not a cointainer. We check if Cont is a container
	if constexpr (mesh::ElementContainerConcept<Cont>) {
		// The element type contained in the container
		// We need it to get back the actual type of the element from the old bases
		using ElType = typename Cont::ElementType;

		using Containers = typename Mesh<A...>::Containers;
		constexpr uint I = IndexInTypes<Cont, Containers>::value;
		static_assert(I >= 0 && I != UINT_NULL);

		// for each Container A in m, we update the pointers of ElType.
		// old base is contained in the array bases, the new base is the base of the container
		(m.template updatePointers<A>((const ElType*)bases[I], m.Cont::vec.data()), ...);
	}
}

template<typename... Args> requires HasVertices<Args...>
template<typename El>
auto& Mesh<Args...>::customComponents()
{
	using ElCont = typename GetContainerOf<El>::type;

	return ElCont::ccVecMap;
}

template<typename... Args> requires HasVertices<Args...>
template<typename El>
const auto& Mesh<Args...>::customComponents() const
{
	using ElCont = typename GetContainerOf<El>::type;

	return ElCont::ccVecMap;
}

template<typename... Args> requires HasVertices<Args...>
template<typename El>
auto& Mesh<Args...>::verticalComponents()
{
	using ElCont = typename GetContainerOf<El>::type;

	return ElCont::vcVecTuple;
}

template<typename... Args> requires HasVertices<Args...>
template<typename El>
const auto& Mesh<Args...>::verticalComponents() const
{
	using ElCont = typename GetContainerOf<El>::type;

	return ElCont::vcVecTuple;
}

} // namespace vcl
