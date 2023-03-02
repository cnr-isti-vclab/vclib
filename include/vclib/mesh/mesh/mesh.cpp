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

#include "mesh_algorithms.h"

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
 * taking care of copying everithing and then update all the references
 *
 * @param oth: the mesh from which constructo this Mesh.
 */
template<typename... Args> requires HasVertices<Args...>
Mesh<Args...>::Mesh(const Mesh<Args...>& oth) :
		Args(oth)... // call auto copy constructors for all the container elements and components
{
	// Set to all elements their parent mesh (this)
	updateAllParentMeshPointers();

	// update references into the vertex container
	using VertexContainer = typename Mesh<Args...>::VertexContainer;
	// just run the same function that we use when vector is reallocated, but using
	// as old base the base of the other vertex container data
	(updateReferences<Args>(oth.VertexContainer::vec.data(), VertexContainer::vec.data()), ...);

	// update references into the face container
	if constexpr (mesh::HasFaceContainer<Mesh<Args...>>) {
		using FaceContainer = typename Mesh<Args...>::FaceContainer;
		// just run the same function that we use when vector is reallocated, but using
		// as old base the base of the other face container data
		(updateReferences<Args>(oth.FaceContainer::vec.data(), FaceContainer::vec.data()), ...);
	}

	// update references into the edge container
	if constexpr (mesh::HasEdgeContainer<Mesh<Args...>>) {
		using EdgeContainer = typename Mesh<Args...>::EdgeContainer;
		// just run the same function that we use when vector is reallocated, but using
		// as old base the base of the other edge container data
		(updateReferences<Args>(oth.EdgeContainer::vec.data(), EdgeContainer::vec.data()), ...);
	}

	// update references into the half edge container
	if constexpr (mesh::HasHalfEdgeContainer<Mesh<Args...>>) {
		using HalfEdgeContainer = typename Mesh<Args...>::HalfEdgeContainer;
		// just run the same function that we use when vector is reallocated, but using
		// as old base the base of the other half edge container data
		(updateReferences<Args>(oth.HalfEdgeContainer::vec.data(), HalfEdgeContainer::vec.data()), ...);
	}
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
	(clearContainer<Args>(), ...);
}

template<typename... Args> requires HasVertices<Args...>
void Mesh<Args...>::compact()
{
	(compactContainer<Args>(), ...);
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
 * // do stuff
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

	using VertexContainer = typename Mesh<Args...>::VertexContainer;
	VertexContainer::enableOptionalComponentsOf(m);

	if constexpr (vcl::mesh::HasFaceContainer<Mesh<Args...>>) {
		using FaceContainer = typename Mesh<Args...>::FaceContainer;
		FaceContainer::enableOptionalComponentsOf(m);
	}

	if constexpr (vcl::mesh::HasEdgeContainer<Mesh<Args...>>) {
		using EdgeContainer = typename Mesh<Args...>::EdgeContainer;
		EdgeContainer::enableOptionalComponentsOf(m);
	}

	if constexpr (vcl::mesh::HasHalfEdgeContainer<Mesh<Args...>>) {
		using HalfEdgeContainer = typename Mesh<Args...>::HalfEdgeContainer;
		HalfEdgeContainer::enableOptionalComponentsOf(m);
	}
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
	// References are not managed here, since they need additional parameters to be imported
	
	(importContainersAndComponents<Args>(m), ...);

	// Set to all elements their parent mesh (this)
	updateAllParentMeshPointers();

	// after importing ordinary components, we need to convert the references between containers.
	// each container can import more than one reference type, e.g.:
	// - VertexContainer could import vertex references (adjacent vertices), face references
	//   (adjacent faces), and so on;
	// - FaceContainer will always import vertex references, but could also import face references
	//   (adjacent faces), edge references (adjacent edges)...
	// for each container of this Mesh, we'll call the importReferences passing the container (Args)
	// as template parameter. This parameter will be used to call all the possible import functions
	// available (vertices, faces, edges, half edges)

	(importReferences<Args>(m), ...);

	if constexpr(mesh::HasFaceContainer<Mesh<Args...>> && mesh::HasFaceContainer<OtherMeshType>) {
		// Now I need to manage imports between different types of meshes (same type of meshes are
		// already managed from importFrom and importReferencesFrom member functions).
		//
		// Generally speaking, Polygon or Dcel meshes can import from any other type of mesh.
		// We need to take care when this mesh has static vertex references number in the face
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
			// triangulation of polygons and create additional triangle faces for each of the imported
			// polygons
			// this function statically asserts that the import can be done.
			manageImportTriFromPoly(m);
		}
		else { // if this is a dcel mesh
			// manage import from another non-dcel mesh (no need to manage the import from another dcel)
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
	vcl::swap(*this, m2);
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
 * will automatically take care of updating all the Vertex references contained in the Mesh.
 *
 * @return the index of the new vertex.
 */
template<typename... Args> requires HasVertices<Args...>
uint Mesh<Args...>::addVertex()
{
	using Vertex          = typename Mesh::VertexType;
	using VertexContainer = typename Mesh::VertexContainer;

	// If the base pointer of the container of vertices changes, it means that all the vertex
	// references contained in the other elements need to be updated (the ones contained in the
	// vertex container are updated automatically)

	Vertex* oldBase = VertexContainer::vec.data();
	uint    vid     = VertexContainer::addElement(this);
	Vertex* newBase = VertexContainer::vec.data();
	if (oldBase != nullptr && oldBase != newBase) { // if true, pointer of container is changed
		// change all the vertex references in the other containers
		(updateReferences<Args>(oldBase, newBase), ...);
	}
	return vid;
}

/**
 * @brief Add a new vertex with the given coordinate into the mesh, returning the id of the added
 * vertex.
 *
 * If the call of this function will cause a reallocation of the Vertex container, the function
 * will automatically take care of updating all the Vertex references contained in the Mesh.
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
 * will automatically take care of updating all the Vertex references contained in the Mesh.
 *
 * @param n: the number of vertices to add to the mesh.
 * @return the id of the first added vertex.
 */
template<typename... Args> requires HasVertices<Args...>
uint Mesh<Args...>::addVertices(uint n)
{
	using Vertex          = typename Mesh::VertexType;
	using VertexContainer = typename Mesh::VertexContainer;

	// If the base pointer of the container of vertices changes, it means that all the vertex
	// references contained in the other elements need to be updated (the ones contained in the
	// vertex container are updated automatically)

	Vertex* oldBase = VertexContainer::vec.data();
	uint    vid     = VertexContainer::addElements(n, this); // add the number of vertices
	Vertex* newBase = VertexContainer::vec.data();

	if (oldBase != nullptr && oldBase != newBase) { // if true, pointer of container is changed
		// change all the vertex references in the other containers
		(updateReferences<Args>(oldBase, newBase), ...);
	}
	return vid;
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
 * will automatically take care of updating all the Vertex references contained in the Mesh.
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
 * will automatically take care of updating all the Vertex references contained in the Mesh.
 *
 * @param n: the new capacity of the vertex container.
 */
template<typename... Args> requires HasVertices<Args...>
void Mesh<Args...>::reserveVertices(uint n)
{
	using Vertex          = typename Mesh::VertexType;
	using VertexContainer = typename Mesh::VertexContainer;

	Vertex* oldBase = VertexContainer::vec.data();
	VertexContainer::reserveElements(n, this);
	Vertex* newBase = VertexContainer::vec.data();
	if (oldBase != nullptr && oldBase != newBase)
		(updateReferences<Args>(oldBase, newBase), ...);
}

/**
 * @brief Compacts the Vertex Container, removing all the vertices marked as deleted. Vertices
 * indices will change accordingly. The function will automatically take care of updating all the
 * Vertex references contained in the Mesh.
 */
template<typename... Args> requires HasVertices<Args...>
void Mesh<Args...>::compactVertices()
{
	using Vertex          = typename Mesh::VertexType;
	using VertexContainer = typename Mesh::VertexContainer;

	if (VertexContainer::vertexNumber() != VertexContainer::vertexContainerSize()) {

		Vertex*          oldBase    = VertexContainer::vec.data();
		std::vector<int> newIndices = VertexContainer::compactElements();
		Vertex*          newBase    = VertexContainer::vec.data();
		assert(oldBase == newBase);

		(updateReferencesAfterCompact<Args>(oldBase, newIndices), ...);
	}
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
	using Face          = typename M::FaceType;
	using FaceContainer = typename M::FaceContainer;

	Face* oldBase = FaceContainer::vec.data();
	uint  fid     = FaceContainer::addElement(this);
	Face* newBase = FaceContainer::vec.data();
	if (oldBase != nullptr && oldBase != newBase)
		(updateReferences<Args>(oldBase, newBase), ...);
	return fid;
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
		if constexpr (!comp::HasFaceHalfEdgeReference<Face>) {
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

	if (begin == end) return -1;

	uint  fid = addFace();
	Face& f   = FaceContainer::face(fid);

	if constexpr (Face::VERTEX_NUMBER < 0) {
		if constexpr (!comp::HasFaceHalfEdgeReference<Face>) {
			uint n = std::distance(begin, end);
			f.resizeVertices(n);
		}
	}

	if constexpr (comp::HasFaceHalfEdgeReference<Face>) {
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
	using Face          = typename M::FaceType;
	using FaceContainer = typename M::FaceContainer;

	Face* oldBase = FaceContainer::vec.data();
	uint  fid     = FaceContainer::addElements(n, this);
	Face* newBase = FaceContainer::vec.data();
	if (oldBase != nullptr && oldBase != newBase)
		(updateReferences<Args>(oldBase, newBase), ...);
	return fid;
}

template<typename... Args> requires HasVertices<Args...>
template<HasFaces M>
void Mesh<Args...>::reserveFaces(uint n)
{
	using Face          = typename M::FaceType;
	using FaceContainer = typename M::FaceContainer;

	Face* oldBase = FaceContainer::vec.data();
	FaceContainer::reserveElements(n, this);
	Face* newBase = FaceContainer::vec.data();
	if (oldBase != nullptr && oldBase != newBase)
		(updateReferences<Args>(oldBase, newBase), ...);
}

template<typename... Args> requires HasVertices<Args...>
template<HasFaces M>
void Mesh<Args...>::compactFaces()
{
	using Face          = typename M::FaceType;
	using FaceContainer = typename M::FaceContainer;

	if (FaceContainer::faceNumber() != FaceContainer::faceContainerSize()) {
		Face*            oldBase    = FaceContainer::vec.data();
		std::vector<int> newIndices = FaceContainer::compactElements();
		Face*            newBase    = FaceContainer::vec.data();
		assert(oldBase == newBase);
		
		(updateReferencesAfterCompact<Args>(oldBase, newIndices), ...);
	}
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
	using Edge          = typename M::EdgeType;
	using EdgeContainer = typename M::EdgeContainer;

	Edge* oldBase = EdgeContainer::vec.data();
	uint  eid     = EdgeContainer::addElement(this);
	Edge* newBase = EdgeContainer::vec.data();
	if (oldBase != nullptr && oldBase != newBase)
		(updateReferences<Args>(oldBase, newBase), ...);
	return eid;
}

/**
 * @brief Add an arbitrary number of n edges, returning the id of the first added edge.
 *
 * This means that, if you want to add 5 edges and this member function returns 4, the added
 * edges will have id from 4 to id 8 included.
 *
 * If the call of this function will cause a reallocation of the EdgeContainer, the function
 * will automatically take care of updating all the Edge references contained in the Mesh.
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
	using Edge          = typename M::EdgeType;
	using EdgeContainer = typename M::EdgeContainer;

	// If the base pointer of the container of edges changes, it means that all the edge
	// references contained in the other elements need to be updated (the ones contained in the
	// edge container are updated automatically)

	Edge* oldBase = EdgeContainer::vec.data();
	uint  eid     = EdgeContainer::addElements(n, this); // add the number of edges
	Edge* newBase = EdgeContainer::vec.data();

	if (oldBase != nullptr && oldBase != newBase) { // if true, pointer of container is changed
		// change all the vertex references in the other containers
		(updateReferences<Args>(oldBase, newBase), ...);
	}
	return eid;
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
 * will automatically take care of updating all the Edge references contained in the Mesh.
 *
 * This function will be available only **if the Mesh has the Edge Container**.
 *
 * @param n: the new capacity of the edge container.
 */
template<typename... Args> requires HasVertices<Args...>
template<HasEdges M>
void Mesh<Args...>::reserveEdges(uint n)
{
	using Edge          = typename M::EdgeType;
	using EdgeContainer = typename M::EdgeContainer;

	Edge* oldBase = EdgeContainer::vec.data();
	EdgeContainer::reserveElements(n, this);
	Edge* newBase = EdgeContainer::vec.data();
	if (oldBase != nullptr && oldBase != newBase)
		(updateReferences<Args>(oldBase, newBase), ...);
}

/**
 * @brief Compacts the EdgeContainer, removing all the edges marked as deleted. Edges indices
 * will change accordingly. The function will automatically take care of updating all the Edge
 * references contained in the Mesh.
 *
 * This function will be available only **if the Mesh has the Edge Container**.
 */
template<typename... Args> requires HasVertices<Args...>
template<HasEdges M>
void Mesh<Args...>::compactEdges()
{
	using Edge          = typename M::EdgeType;
	using EdgeContainer = typename M::EdgeContainer;

	if (EdgeContainer::edgeNumber() != EdgeContainer::edgeContainerSize()) {
		Edge*            oldBase    = EdgeContainer::vec.data();
		std::vector<int> newIndices = EdgeContainer::compactElements();
		Edge*            newBase    = EdgeContainer::vec.data();
		assert(oldBase == newBase);

		(updateReferencesAfterCompact<Args>(oldBase, newIndices), ...);
	}
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
	using HalfEdge          = typename M::HalfEdgeType;
	using HalfEdgeContainer = typename M::HalfEdgeContainer;

	HalfEdge* oldBase = HalfEdgeContainer::vec.data();
	uint      eid     = HalfEdgeContainer::addElement(this);
	HalfEdge* newBase = HalfEdgeContainer::vec.data();
	if (oldBase != nullptr && oldBase != newBase)
		(updateReferences<Args>(oldBase, newBase), ...);
	return eid;
}

/**
 * @brief Add an arbitrary number of n Halfedges, returning the id of the first added Halfedge.
 *
 * This means that, if you want to add 5 Halfedges and this member function returns 4, the added
 * Halfedges will have id from 4 to id 8 included.
 *
 * If the call of this function will cause a reallocation of the HalfEdgeContainer, the function
 * will automatically take care of updating all the HalfEdge references contained in the Mesh.
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
	using HalfEdge          = typename M::HalfEdgeType;
	using HalfEdgeContainer = typename M::HalfEdgeContainer;

	// If the base pointer of the container of Halfedges changes, it means that all the Halfedge
	// references contained in the other elements need to be updated (the ones contained in the
	// Halfedge container are updated automatically)

	HalfEdge* oldBase = HalfEdgeContainer::vec.data();
	uint      eid     = HalfEdgeContainer::addElements(n, this); // add the number of half edges
	HalfEdge* newBase = HalfEdgeContainer::vec.data();

	if (oldBase != nullptr && oldBase != newBase) { // if true, pointer of container is changed
		// change all the half edge references in the other containers
		(updateReferences<Args>(oldBase, newBase), ...);
	}
	return eid;
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
 * will automatically take care of updating all the HalfEdge references contained in the Mesh.
 *
 * This function will be available only **if the Mesh has the HalfEdge Container**.
 *
 * @param n: the new capacity of the Halfedge container.
 */
template<typename... Args> requires HasVertices<Args...>
template<HasHalfEdges M>
void Mesh<Args...>::reserveHalfEdges(uint n)
{
	using HalfEdge          = typename M::HalfEdgeType;
	using HalfEdgeContainer = typename M::HalfEdgeContainer;

	HalfEdge* oldBase = HalfEdgeContainer::vec.data();
	HalfEdgeContainer::reserveElements(n, this);
	HalfEdge* newBase = HalfEdgeContainer::vec.data();
	if (oldBase != nullptr && oldBase != newBase)
		(updateReferences<Args>(oldBase, newBase), ...);
}

/**
 * @brief Compacts the HalfEdgeContainer, removing all the Halfedges marked as deleted. HalfEdges
 * indices will change accordingly. The function will automatically take care of updating all the
 * HalfEdge references contained in the Mesh.
 *
 * This function will be available only **if the Mesh has the HalfEdge Container**.
 */
template<typename... Args> requires HasVertices<Args...>
template<HasHalfEdges M>
void Mesh<Args...>::compactHalfEdges()
{
	using HalfEdge          = typename M::HalfEdgeType;
	using HalfEdgeContainer = typename M::HalfEdgeContainer;

	if (HalfEdgeContainer::HalfedgeNumber() != HalfEdgeContainer::HalfedgeContainerSize()) {
		HalfEdge*        oldBase    = HalfEdgeContainer::vec.data();
		std::vector<int> newIndices = HalfEdgeContainer::compactElements();
		HalfEdge*        newBase    = HalfEdgeContainer::vec.data();
		assert(oldBase == newBase);
		
		(updateReferencesAfterCompact<Args>(oldBase, newBase), ...);
	}
}

/*********************
 * Protected Members *
 *********************/

template<typename... Args> requires HasVertices<Args...>
template<typename Cont, typename Element>
void Mesh<Args...>::updateReferences(
	const Element* oldBase,
	const Element* newBase)
{
	if constexpr(mesh::IsElementContainer<Cont>) {
		Cont::updateReferences(oldBase, newBase);
	}
}

template<typename... Args> requires HasVertices<Args...>
template<typename Cont, typename Element>
void Mesh<Args...>::updateReferencesAfterCompact(
	const Element*          base,
	const std::vector<int>& newIndices)
{
	if constexpr(mesh::IsElementContainer<Cont>) {
		Cont::updateReferencesAfterCompact(base, newIndices);
	}
}

template<typename... Args> requires HasVertices<Args...>
void Mesh<Args...>::updateAllParentMeshPointers()
{
	(setParentMeshPointers<Args>(), ...);
}

template<typename... Args> requires HasVertices<Args...>
template<typename Cont>
void Mesh<Args...>::clearContainer()
{
	if constexpr(mesh::IsElementContainer<Cont>) {
		Cont::clearElements();
	}
}

template<typename... Args> requires HasVertices<Args...>
template<typename Cont>
void Mesh<Args...>::compactContainer()
{
	if constexpr(mesh::IsElementContainer<Cont>) {
		auto* oldBase = Cont::vec.data();
		std::vector<int> newIndices = Cont::compactElements();
		auto* newBase = Cont::vec.data();
		assert(oldBase == newBase);

		// todo
		//(updateReferencesAfterCompact<Args>(oldBase, newIndices), ...);
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
template<typename Cont>
void Mesh<Args...>::setParentMeshPointers()
{
	if constexpr(mesh::IsElementContainer<Cont>) {
		Cont::setParentMeshPointers(this);
	}
}

template<typename... Args> requires HasVertices<Args...>
template<typename Cont, typename OthMesh>
void Mesh<Args...>::importContainersAndComponents(const OthMesh &m)
{
	if constexpr(mesh::IsElementContainer<Cont>) {
		Cont::importFrom(m, this);
	}
	else {
		Cont::importFrom(m);
	}
}

/**
 * This function will call, for a given container of this mesh that is passed as a template
 * parameter Cont, all the references of all the elements from the other mesh m.
 */
template<typename... Args> requires HasVertices<Args...>
template<typename Cont, typename OthMesh>
void Mesh<Args...>::importReferences(const OthMesh &m)
{
	using ThisMesh = Mesh<Args...>;

	// if Cont is a container (could be a mesh component)
	if constexpr(mesh::IsElementContainer<Cont>) {
		// will call the specific importVertexReferences of the Cont container.
		// it will take care to import the reference from tha same container type of m.
		Cont::importVertexReferencesFrom(m, &this->vertex(0));
		if constexpr (mesh::HasFaceContainer<ThisMesh>) {
			Cont::importFaceReferencesFrom(m, &this->face(0));
		}
		if constexpr (mesh::HasEdgeContainer<ThisMesh>) {
			Cont::importEdgeReferencesFrom(m, &this->edge(0));
		}
		if constexpr (mesh::HasHalfEdgeContainer<ThisMesh>) {
			Cont::importHalfEdgeReferencesFrom(m, &this->halfEdge(0));
		}
	}
}

/**
 * @brief This function manages the case where we try to import into a TriMesh a PolyMesh
 * Faces have been already imported, but without vertex references and other components that
 * depend on the number of vertices (e.g. wedges)
 */
template<typename... Args> requires HasVertices<Args...>
template<typename OthMesh>
void Mesh<Args...>::manageImportTriFromPoly(const OthMesh &m)
{
	using VertexType = typename Mesh<Args...>::VertexType;
	using MVertexType = typename OthMesh::VertexType;
	using FaceType = typename Mesh<Args...>::FaceType;
	using MFaceType = typename OthMesh::FaceType;

	using VertexContainer = typename Mesh<Args...>::VertexContainer;
	using FaceContainer   = typename Mesh<Args...>::FaceContainer;

	// if this is not a triangle mesh nor a polygon mesh (meaning that we can't control the
	// number of vertex references in this mesh), and this mesh does not have the same
	// number of vertex references of the other, it means that we don't know how to convert
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
			// the vertex references have been correctly imported from the import references
			// function. The import references function does nothing when importing from a face
			// with at least 4 vertices
			if (mf.vertexNumber() != FaceType::VERTEX_NUMBER) {
				// triangulate mf; the first triangle of the triangulation will be
				// this->face(m.index(mf));
				// the other triangles will be added at the end of the container
				std::vector<uint> tris = vcl::mesh::earCut(mf);
				FaceType& f = FaceContainer::face(m.index(mf));
				importTriReferencesHelper(f, mf, base, mvbase, tris, 0);

				// number of other faces to add
				uint nf = tris.size() / 3 - 1;
				uint fid = FaceContainer::addElements(nf, this);

				uint i = 3; // index that cycles into tris
				for (; fid < FaceContainer::faceContainerSize(); ++fid) {
					FaceType& f = FaceContainer::face(fid);
					importTriReferencesHelper(f, mf, base, mvbase, tris, i);
					i+=3;
				}
			}
		}
	}
}

/**
 * @brief This function manages the case where we try to import into a Dcel another type of Mesh.
 * Faces have been already imported, but not vertex references (half edges still need to be created)
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

	// base and mvbase are needed to convert vertex references from other to this mesh
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
		// set each vertex of f computing the right reference from mesh m and face mf
		for (uint j = 0; j < mf.vertexNumber(); ++j) {
			f.vertex(j) = base + (mf.vertex(j) - mvbase);
		}

		// todo: adjacent faces and wedges
	}
}

template<typename... Args> requires HasVertices<Args...>
template<typename FaceType, typename MFaceType, typename VertexType, typename MVertexType>
void Mesh<Args...>::importTriReferencesHelper(
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

template<typename... Args> requires HasVertices<Args...>
template<typename El>
auto& Mesh<Args...>::customComponents() requires ElementConcept<El>
{
	if constexpr (EdgeConcept<El>) {
		using EdgeContainer = typename Mesh<Args...>::EdgeContainer;
		return EdgeContainer::ccVecMap;
	}
	if constexpr (FaceConcept<El>) {
		using FaceContainer = typename Mesh<Args...>::FaceContainer;
		return FaceContainer::ccVecMap;
	}
	if constexpr (HalfEdgeConcept<El>) {
		using HalfEdgeContainer = typename Mesh<Args...>::HalfEdgeContainer;
		return HalfEdgeContainer::ccVecMap;
	}
	if constexpr (VertexConcept<El>) {
		using VertexContainer = typename Mesh<Args...>::VertexContainer;
		return VertexContainer::ccVecMap;
	}
}

template<typename... Args> requires HasVertices<Args...>
template<typename El>
const auto& Mesh<Args...>::customComponents() const requires ElementConcept<El>
{
	if constexpr (EdgeConcept<El>) {
		using EdgeContainer = typename Mesh<Args...>::EdgeContainer;
		return EdgeContainer::ccVecMap;
	}
	if constexpr (FaceConcept<El>) {
		using FaceContainer = typename Mesh<Args...>::FaceContainer;
		return FaceContainer::ccVecMap;
	}
	if constexpr (HalfEdgeConcept<El>) {
		using HalfEdgeContainer = typename Mesh<Args...>::HalfEdgeContainer;
		return HalfEdgeContainer::ccVecMap;
	}
	if constexpr (VertexConcept<El>) {
		using VertexContainer = typename Mesh<Args...>::VertexContainer;
		return VertexContainer::ccVecMap;
	}
}

template<typename... Args> requires HasVertices<Args...>
template<typename El>
auto& Mesh<Args...>::verticalComponents() requires ElementConcept<El>
{
	if constexpr (EdgeConcept<El>) {
		using EdgeContainer = typename Mesh<Args...>::EdgeContainer;
		return EdgeContainer::vcVecTuple;
	}
	if constexpr (FaceConcept<El>) {
		using FaceContainer = typename Mesh<Args...>::FaceContainer;
		return FaceContainer::vcVecTuple;
	}
	if constexpr (HalfEdgeConcept<El>) {
		using HalfEdgeContainer = typename Mesh<Args...>::HalfEdgeContainer;
		return HalfEdgeContainer::vcVecTuple;
	}
	if constexpr (VertexConcept<El>) {
		using VertexContainer = typename Mesh<Args...>::VertexContainer;
		return VertexContainer::vcVecTuple;
	}
}

template<typename... Args> requires HasVertices<Args...>
template<typename El>
const auto& Mesh<Args...>::verticalComponents() const requires ElementConcept<El>
{
	if constexpr (EdgeConcept<El>) {
		using EdgeContainer = typename Mesh<Args...>::EdgeContainer;
		return EdgeContainer::vcVecTuple;
	}
	if constexpr (FaceConcept<El>) {
		using FaceContainer = typename Mesh<Args...>::FaceContainer;
		return FaceContainer::vcVecTuple;
	}
	if constexpr (HalfEdgeConcept<El>) {
		using HalfEdgeContainer = typename Mesh<Args...>::HalfEdgeContainer;
		return HalfEdgeContainer::vcVecTuple;
	}
	if constexpr (VertexConcept<El>) {
		using VertexContainer = typename Mesh<Args...>::VertexContainer;
		return VertexContainer::vcVecTuple;
	}
}

/**
 * @brief Swaps two meshes of the same type
 */
template<typename... A> requires HasVertices<A...>
inline void swap(Mesh<A...>& m1, Mesh<A...>& m2)
{
	// container bases of verts and faces, and edges Vec for m1 and m2
	void* m1BaseV = nullptr;
	void* m2BaseV = nullptr;
	void* m1BaseF = nullptr;
	void* m2BaseF = nullptr;
	void* m1BaseE = nullptr;
	void* m2BaseE = nullptr;
	void* m1BaseHE = nullptr;
	void* m2BaseHE = nullptr;

	// save the bases of the containers before swap
	using VertexContainer = typename Mesh<A...>::VertexContainer;
	m1BaseV               = m1.VertexContainer::vec.data();
	m2BaseV               = m2.VertexContainer::vec.data();

	if constexpr (mesh::HasFaceContainer<Mesh<A...>>) {
		using FaceContainer = typename Mesh<A...>::FaceContainer;
		m1BaseF             = m1.FaceContainer::vec.data();
		m2BaseF             = m2.FaceContainer::vec.data();
	}
	if constexpr (mesh::HasEdgeContainer<Mesh<A...>>) {
		using EdgeContainer = typename Mesh<A...>::EdgeContainer;
		m1BaseE             = m1.EdgeContainer::vec.data();
		m2BaseE             = m2.EdgeContainer::vec.data();
	}
	if constexpr (mesh::HasHalfEdgeContainer<Mesh<A...>>) {
		using HalfEdgeContainer = typename Mesh<A...>::HalfEdgeContainer;
		m1BaseHE             = m1.HalfEdgeContainer::vec.data();
		m2BaseHE             = m2.HalfEdgeContainer::vec.data();
	}

	// actual swap of all the containers and the components of the mesh
	// using pack expansion: swap will be called for each of the containers (or components!) that
	// compose the Mesh
	using std::swap;
	(swap((A&) m1, (A&) m2), ...);

	// Set to all elements their parent mesh
	m1.updateAllParentMeshPointers();
	m2.updateAllParentMeshPointers();

	// update all the references to m1 and m2: old base of m1 is now "old base" of m2, and viceversa

	using VertexType      = typename Mesh<A...>::VertexType;
	(m1.template updateReferences<A>((VertexType*) m2BaseV, m1.VertexContainer::vec.data()), ...);
	(m2.template updateReferences<A>((VertexType*) m1BaseV, m2.VertexContainer::vec.data()), ...);

	if constexpr (mesh::HasFaceContainer<Mesh<A...>>) {
		using FaceType      = typename Mesh<A...>::FaceType;
		using FaceContainer = typename Mesh<A...>::FaceContainer;
		(m1.template updateReferences<A>((FaceType*) m2BaseF, m1.FaceContainer::vec.data()), ...);
		(m2.template updateReferences<A>((FaceType*) m1BaseF, m2.FaceContainer::vec.data()), ...);
	}
	if constexpr (mesh::HasEdgeContainer<Mesh<A...>>) {
		using EdgeType      = typename Mesh<A...>::EdgeType;
		using EdgeContainer = typename Mesh<A...>::EdgeContainer;
		(m1.template updateReferences<A>((EdgeType*) m2BaseE, m1.EdgeContainer::vec.data()), ...);
		(m2.template updateReferences<A>((EdgeType*) m1BaseE, m2.EdgeContainer::vec.data()), ...);
	}
	if constexpr (mesh::HasHalfEdgeContainer<Mesh<A...>>) {
		using HalfEdgeType      = typename Mesh<A...>::HalfEdgeType;
		using HalfEdgeContainer = typename Mesh<A...>::HalfEdgeContainer;
		(m1.template updateReferences<A>((HalfEdgeType*) m2BaseHE, m1.HalfEdgeContainer::vec.data()), ...);
		(m2.template updateReferences<A>((HalfEdgeType*) m1BaseHE, m2.HalfEdgeContainer::vec.data()), ...);
	}
}

} // namespace vcl
