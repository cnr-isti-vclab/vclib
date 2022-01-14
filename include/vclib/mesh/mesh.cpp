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

#include "mesh.h"

namespace vcl {

/**
 * @brief Empty constructor, constructs an empty mesh.
 */
template<typename... Args>
Mesh<Args...>::Mesh()
{
}

/**
 * @brief Copy constructor of the Mesh. Will create a deep copy of the given input mesh,
 * taking care of copying everithing and then update all the references
 *
 * @param oth: the mesh from which constructo this Mesh.
 */
template<typename... Args>
Mesh<Args...>::Mesh(const Mesh<Args...>& oth) :
		Args(oth)... // call auto copy constructors for all the container elements and components
{
	// update all the optional container references
	updateAllOptionalContainerReferences();

	// update references into the vertex container
	if constexpr (mesh::hasVertices<Mesh<Args...>>()) {
		using VertexContainer = typename Mesh<Args...>::VertexContainer;
		// just run the same function that we use when vector is reallocated, but using
		// as old base the base of the other vertex container data
		updateVertexReferences(oth.VertexContainer::vec.data(), VertexContainer::vec.data());
	}

	// update references into the face container
	if constexpr (mesh::hasFaces<Mesh<Args...>>()) {
		using FaceContainer = typename Mesh<Args...>::FaceContainer;
		// just run the same function that we use when vector is reallocated, but using
		// as old base the base of the other vertex container data
		updateFaceReferences(oth.FaceContainer::vec.data(), FaceContainer::vec.data());
	}

	// update references into the edge container
	if constexpr (mesh::hasEdges<Mesh<Args...>>()) {
		using EdgeContainer = typename Mesh<Args...>::EdgeContainer;
		// just run the same function that we use when vector is reallocated, but using
		// as old base the base of the other vertex container data
		updateEdgeReferences(oth.EdgeContainer::vec.data(), EdgeContainer::vec.data());
	}
}

/**
 * @brief Move constructor, moves the given mesh into this one, without any other
 * resource acquisition.
 *
 * @param oth: the mesh that will be moved into this Mesh.
 */
template<typename... Args>
Mesh<Args...>::Mesh(Mesh<Args...>&& oth)
{
	swap(oth); // use copy ans swap idiom: this (empty) mesh is swapped with the input one
}

/**
 * @brief Clears all the Elements contained in the mesh.
 * @todo manage also other components
 */
template<typename... Args>
void Mesh<Args...>::clear()
{
	if constexpr (mesh::hasVertices<Mesh<Args...>>()) {
		using VertexContainer = typename Mesh<Args...>::VertexContainer;
		VertexContainer::clearVertices(); // clear vertices, only if the mesh has vertices
	}
	if constexpr (mesh::hasFaces<Mesh<Args...>>()) {
		using FaceContainer = typename Mesh<Args...>::FaceContainer;
		FaceContainer::clearFaces(); // clear faces, only if the mesh has faces
	}
	if constexpr (mesh::hasEdges<Mesh<Args...>>()) {
		using EdgeContainer = typename Mesh<Args...>::EdgeContainer;
		EdgeContainer::clearEdges(); // clear faces, only if the mesh has edges
	}
}

/**
 * @brief Returns the index of the given vertex in the VertexContainer of the Mesh.
 * @param v: a reference of a vertex of the Mesh.
 * @return the index of the given vertex.
 */
template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasVertices<M>(), uint)
Mesh<Args...>::index(const typename M::VertexType& v) const
{
	using VertexContainer = typename M::VertexContainer;
	return VertexContainer::index(&v);
}

/**
 * @brief Returns the index of the given vertex in the VertexContainer of the Mesh.
 * @param v: a pointer of a vertex of the Mesh.
 * @return the index of the given vertex.
 */
template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasVertices<M>(), uint)
Mesh<Args...>::index(const typename M::VertexType* v) const
{
	using VertexContainer = typename M::VertexContainer;
	return VertexContainer::index(v);
}

/**
 * @brief Add a new vertex into the mesh, returning the index of the added vertex.
 *
 * If the call of this function will cause a reallocation of the Vertex container, the function
 * will automatically take care of updating all the Vertex references contained in the Mesh.
 *
 * This function will be available only **if the Mesh has the Vertex Container**.
 *
 * @return the index of the new vertex.
 */
template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasVertices<M>(), uint)
Mesh<Args...>::addVertex()
{
	using Vertex          = typename M::VertexType;
	using VertexContainer = typename M::VertexContainer;

	// If the base pointer of the container of vertices changes, it means that all the vertex
	// references contained in the other elements need to be updated (the ones contained in the
	// vertex container are updated automatically)

	Vertex* oldBase = VertexContainer::vec.data();
	uint    vid     = VertexContainer::addVertex();
	Vertex* newBase = VertexContainer::vec.data();
	if (oldBase != nullptr && oldBase != newBase) { // if true, pointer of container is changed
		// change all the vertex references in the other containers
		updateVertexReferences(oldBase, newBase);
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
 * This function will be available only **if the Mesh has the Vertex Container**.
 *
 * @param p: coordinate of the new vertex.
 * @return the id of the new vertex.
 */
template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasVertices<M>(), uint)
Mesh<Args...>::addVertex(const typename M::VertexType::CoordType& p)
{
	using VertexContainer = typename M::VertexContainer;

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
 * This function will be available only **if the Mesh has the Vertex Container**.
 *
 * @param n: the number of vertices to add to the mesh.
 * @return the id of the first added vertex.
 */
template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasVertices<M>(), uint)
Mesh<Args...>::addVertices(uint n)
{
	using Vertex          = typename M::VertexType;
	using VertexContainer = typename M::VertexContainer;

	// If the base pointer of the container of vertices changes, it means that all the vertex
	// references contained in the other elements need to be updated (the ones contained in the
	// vertex container are updated automatically)

	Vertex* oldBase = VertexContainer::vec.data();
	uint    vid     = VertexContainer::addVertices(n); // add the number of vertices
	Vertex* newBase = VertexContainer::vec.data();

	if (oldBase != nullptr && oldBase != newBase) { // if true, pointer of container is changed
		// change all the vertex references in the other containers
		updateVertexReferences(oldBase, newBase);
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
 * This function will be available only **if the Mesh has the Vertex Container**.
 *
 * @param v: list of vertices
 * @return the id of the first added vertex.
 */
template<typename... Args>
template<typename... VC, typename M>
VCL_ENABLE_IF(mesh::hasVertices<M>(), uint)
Mesh<Args...>::addVertices(
	const typename M::VertexType::CoordType& p,
	const VC&... v) // parameter pack of points
{
	using VertexContainer = typename M::VertexContainer;
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
 * This function will be available only **if the Mesh has the Vertex Container**.
 *
 * @param n: the new capacity of the vertex container.
 */
template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasVertices<M>(), void)
Mesh<Args...>::reserveVertices(uint n)
{
	using Vertex          = typename M::VertexType;
	using VertexContainer = typename M::VertexContainer;

	Vertex* oldBase = VertexContainer::vec.data();
	VertexContainer::reserveVertices(n);
	Vertex* newBase = VertexContainer::vec.data();
	if (oldBase != nullptr && oldBase != newBase)
		updateVertexReferences(oldBase, newBase);
}

/**
 * @brief Compacts the Vertex Container, removing all the vertices marked as deleted. Vertices
 * indices will change accordingly. The function will automatically take care of updating all the
 * Vertex references contained in the Mesh.
 *
 * This function will be available only **if the Mesh has the Vertex Container**.
 */
template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasVertices<M>(), void)
Mesh<Args...>::compactVertices()
{
	using Vertex          = typename M::VertexType;
	using VertexContainer = typename M::VertexContainer;

	Vertex*          oldBase    = VertexContainer::vec.data();
	std::vector<int> newIndices = VertexContainer::compactVertices();
	Vertex*          newBase    = VertexContainer::vec.data();
	assert(oldBase == newBase);

	updateVertexReferencesAfterCompact(oldBase, newIndices);
}

template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasFaces<M>(), uint)
Mesh<Args...>::index(const typename M::FaceType& f) const
{
	using FaceContainer = typename M::FaceContainer;
	return FaceContainer::index(&f);
}

template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasFaces<M>(), uint)
Mesh<Args...>::index(const typename M::FaceType* f) const
{
	using FaceContainer = typename M::FaceContainer;
	return FaceContainer::index(f);
}

template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasFaces<M>(), uint)
Mesh<Args...>::addFace()
{
	using Face          = typename M::FaceType;
	using FaceContainer = typename M::FaceContainer;

	Face* oldBase = FaceContainer::vec.data();
	uint  fid     = FaceContainer::addFace();
	Face* newBase = FaceContainer::vec.data();
	if (oldBase != nullptr && oldBase != newBase)
		updateFaceReferences(oldBase, newBase);
	return fid;
}

template<typename... Args>
template<typename M, typename... V>
VCL_ENABLE_IF(mesh::hasFaces<M>() && mesh::hasVertices<M>(), uint)
Mesh<Args...>::addFace(V... args)
{
	using Face          = typename M::FaceType;
	using FaceContainer = typename M::FaceContainer;

	uint  fid = addFace();
	Face& f   = FaceContainer::face(fid);

	constexpr uint n = sizeof...(args);
	if constexpr (Face::VERTEX_NUMBER < 0) {
		f.resizeVertices(n);
	}
	else {
		static_assert(n == Face::VERTEX_NUMBER, "Wrong number of vertices in Mesh::addFace.");
	}

	addFaceHelper(f, args...);
	return fid;
}

template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasFaces<M>(), uint)
Mesh<Args...>::addFaces(uint n)
{
	using Face          = typename M::FaceType;
	using FaceContainer = typename M::FaceContainer;

	Face* oldBase = FaceContainer::vec.data();
	uint  fid     = FaceContainer::addFaces(n);
	Face* newBase = FaceContainer::vec.data();
	if (oldBase != nullptr && oldBase != newBase)
		updateFaceReferences(oldBase, newBase);
	return fid;
}

template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasFaces<M>(), void)
Mesh<Args...>::reserveFaces(uint n)
{
	using Face          = typename M::FaceType;
	using FaceContainer = typename M::FaceContainer;

	Face* oldBase = FaceContainer::vec.data();
	FaceContainer::reserveFaces(n);
	Face* newBase = FaceContainer::vec.data();
	if (oldBase != nullptr && oldBase != newBase)
		updateFaceReferences(oldBase, newBase);
}

template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasFaces<M>(), void)
Mesh<Args...>::compactFaces()
{
	using Face          = typename M::FaceType;
	using FaceContainer = typename M::FaceContainer;

	Face*            oldBase    = FaceContainer::vec.data();
	std::vector<int> newIndices = FaceContainer::compactFaces();
	Face*            newBase    = FaceContainer::vec.data();
	assert(oldBase == newBase);

	updateFaceReferencesAfterCompact(oldBase, newIndices);
}

/**
 * @brief Returns the index of the given edge in the EdgeContainer of the Mesh.
 * @param e: a reference of an edge of the Mesh.
 * @return the index of the given edge.
 */
template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasEdges<M>(), uint)
Mesh<Args...>::index(const typename M::EdgeType& e) const
{
	using EdgeContainer = typename M::EdgeContainer;
	return EdgeContainer::index(&e);
}

/**
 * @brief Returns the index of the given edge in the EdgeContainer of the Mesh.
 * @param e: a pointer of an edge of the Mesh.
 * @return the index of the given edge.
 */
template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasEdges<M>(), uint)
Mesh<Args...>::index(const typename M::EdgeType* e) const
{
	using EdgeContainer = typename M::EdgeContainer;
	return EdgeContainer::index(e);
}

template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasEdges<M>(), uint)
Mesh<Args...>::addEdge()
{
	using Edge          = typename M::EdgeType;
	using EdgeContainer = typename M::EdgeContainer;

	Edge* oldBase = EdgeContainer::vec.data();
	uint  eid     = EdgeContainer::addEdge();
	Edge* newBase = EdgeContainer::vec.data();
	if (oldBase != nullptr && oldBase != newBase)
		updateEdgeReferences(oldBase, newBase);
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
template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasEdges<M>(), uint)
Mesh<Args...>::addEdges(uint n)
{
	using Edge          = typename M::EdgeType;
	using EdgeContainer = typename M::EdgeContainer;

	// If the base pointer of the container of edges changes, it means that all the edge
	// references contained in the other elements need to be updated (the ones contained in the
	// edge container are updated automatically)

	Edge* oldBase = EdgeContainer::vec.data();
	uint  eid     = EdgeContainer::addEdges(n); // add the number of vertices
	Edge* newBase = EdgeContainer::vec.data();

	if (oldBase != nullptr && oldBase != newBase) { // if true, pointer of container is changed
		// change all the vertex references in the other containers
		updateEdgeReferences(oldBase, newBase);
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
template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasEdges<M>(), void)
Mesh<Args...>::reserveEdges(uint n)
{
	using Edge          = typename M::EdgeType;
	using EdgeContainer = typename M::EdgeContainer;

	Edge* oldBase = EdgeContainer::vec.data();
	EdgeContainer::reserveEdges(n);
	Edge* newBase = EdgeContainer::vec.data();
	if (oldBase != nullptr && oldBase != newBase)
		updateEdgeReferences(oldBase, newBase);
}

/**
 * @brief Compacts the EdgeContainer, removing all the edges marked as deleted. Edges indices
 * will change accordingly. The function will automatically take care of updating all the Edge
 * references contained in the Mesh.
 *
 * This function will be available only **if the Mesh has the Edge Container**.
 */
template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasEdges<M>(), void)
Mesh<Args...>::compactEdges()
{
	using Edge          = typename M::EdgeType;
	using EdgeContainer = typename M::EdgeContainer;

	Edge*            oldBase    = EdgeContainer::vec.data();
	std::vector<int> newIndices = EdgeContainer::compactEdges();
	Edge*            newBase    = EdgeContainer::vec.data();
	assert(oldBase == newBase);

	updateEdgeReferencesAfterCompact(oldBase, newIndices);
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
template<typename... Args>
template<typename OtherMeshType>
void Mesh<Args...>::enableSameOptionalComponentsOf(const OtherMeshType& m)
{
	// enable all optional components of this Mesh depending on what's available in the
	// OtherMeshType

	if constexpr (vcl::mesh::hasVertices<Mesh<Args...>>()) {
		using VertexContainer = typename Mesh<Args...>::VertexContainer;
		VertexContainer::enableOptionalComponentsOf(m);
	}

	if constexpr (vcl::mesh::hasFaces<Mesh<Args...>>()) {
		using FaceContainer = typename Mesh<Args...>::FaceContainer;
		FaceContainer::enableOptionalComponentsOf(m);
	}

	if constexpr (vcl::mesh::hasEdges<Mesh<Args...>>()) {
		using EdgeContainer = typename Mesh<Args...>::EdgeContainer;
		EdgeContainer::enableOptionalComponentsOf(m);
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
 * should call the function enableSameOptionalComponentsOf(m) before this function.
 *
 * @param[in] m: the mesh from which import all the data.
 */
template<typename... Args>
template<typename OtherMeshType>
void Mesh<Args...>::importFrom(const OtherMeshType& m)
{
	(Args::importFrom(m), ...);

	// after importing ordinary components, I need to convert the references between containers
	// to each importElReferencesFrom member function of a container, I need to pass:
	// - the mesh from which import the references
	// - the base of the Element container, that is used to compute the new reference of the element
	// note: per element E references are already updated in the element E container of the mesh

	if constexpr(mesh::hasVertices<Mesh<Args...>>()) {
		using VertexContainer = typename Mesh<Args...>::VertexContainer;

		// import face references in the vertex container
		if constexpr(mesh::hasFaces<Mesh<Args...>>()) {
			using FaceContainer = typename Mesh<Args...>::FaceContainer;
			VertexContainer::importFaceReferencesFrom(m, FaceContainer::vec.data());
		}

		// import edge references in the vertex container
		if constexpr(mesh::hasEdges<Mesh<Args...>>()) {
			using EdgeContainer = typename Mesh<Args...>::EdgeContainer;
			VertexContainer::importEdgeReferencesFrom(m, EdgeContainer::vec.data());
		}
	}

	if constexpr(mesh::hasFaces<Mesh<Args...>>()) {
		using FaceContainer = typename Mesh<Args...>::FaceContainer;

		// import vertex references in the face container
		if constexpr(mesh::hasVertices<Mesh<Args...>>()) {
			using VertexContainer = typename Mesh<Args...>::VertexContainer;
			FaceContainer::importVertexReferencesFrom(m, VertexContainer::vec.data());
		}

		// import edge references in the face container
		if constexpr(mesh::hasEdges<Mesh<Args...>>()) {
			using EdgeContainer = typename Mesh<Args...>::EdgeContainer;
			FaceContainer::importEdgeReferencesFrom(m, EdgeContainer::vec.data());
		}
	}

	if constexpr(mesh::hasEdges<Mesh<Args...>>()) {
		using EdgeContainer = typename Mesh<Args...>::EdgeContainer;

		// import vertex references in the edge container
		if constexpr(mesh::hasVertices<Mesh<Args...>>()) {
			using VertexContainer = typename Mesh<Args...>::VertexContainer;
			EdgeContainer::importVertexReferencesFrom(m, VertexContainer::vec.data());
		}

		// import face references in the edge container
		if constexpr(mesh::hasEdges<Mesh<Args...>>()) {
			using FaceContainer = typename Mesh<Args...>::FaceContainer;
			EdgeContainer::importFaceReferencesFrom(m, FaceContainer::vec.data());
		}
	}
}

/**
 * @brief Swaps this mesh with the other input Mesh m2.
 * @param m2: the Mesh to swap with this Mesh.
 */
template<typename... Args>
void Mesh<Args...>::swap(Mesh& m2)
{
	vcl::swap(*this, m2);
}

template<typename... Args>
Mesh<Args...>& Mesh<Args...>::operator=(Mesh<Args...> oth)
{
	swap(oth);
	return *this;
}

/*********************
 * Protected Members *
 *********************/

template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasVertices<M>(), void)
Mesh<Args...>::updateVertexReferences(
	const typename M::VertexType* oldBase,
	const typename M::VertexType* newBase)
{
	// update vertex references in the Face Container, if it exists
	if constexpr (vcl::mesh::hasFaces<M>()) {
		using FaceContainer = typename M::FaceContainer;
		FaceContainer::updateVertexReferences(oldBase, newBase);
	}

	// update vertex references in the Edge Container, if it exists
	if constexpr (vcl::mesh::hasEdges<M>()) {
		using EdgeContainer = typename M::EdgeContainer;
		EdgeContainer::updateVertexReferences(oldBase, newBase);
	}
}

template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasVertices<M>(), void)
Mesh<Args...>::updateVertexReferencesAfterCompact(
	const typename M::VertexType* base,
	const std::vector<int>&       newIndices)
{
	// update vertex references in the Face Container, if it exists
	if constexpr (vcl::mesh::hasFaces<M>()) {
		using FaceContainer = typename M::FaceContainer;
		FaceContainer::updateVertexReferencesAfterCompact(base, newIndices);
	}

	// update vertex references in the Edge Container, if it exists
	if constexpr (vcl::mesh::hasEdges<M>()) {
		using EdgeContainer = typename M::EdgeContainer;
		EdgeContainer::updateVertexReferencesAfterCompact(base, newIndices);
	}
}

template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasFaces<M>(), void)
Mesh<Args...>::updateFaceReferences(
	const typename M::FaceType* oldBase,
	const typename M::FaceType* newBase)
{
	// update face references in the Vertex Container, if it exists
	if constexpr (vcl::mesh::hasVertices<M>()) {
		using VertexContainer = typename M::VertexContainer;
		VertexContainer::updateFaceReferences(oldBase, newBase);
	}

	// update face references in the Edge Container, if it exists
	if constexpr (vcl::mesh::hasEdges<M>()) {
		using EdgeContainer = typename M::EdgeContainer;
		EdgeContainer::updateFaceReferences(oldBase, newBase);
	}
}

template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasFaces<M>(), void)
Mesh<Args...>::updateFaceReferencesAfterCompact(
	const typename M::FaceType* base,
	const std::vector<int>&     newIndices)
{
	// update face references in the Vertex Container, if it exists
	if constexpr (vcl::mesh::hasVertices<M>()) {
		using VertexContainer = typename M::VertexContainer;
		VertexContainer::updateFaceReferencesAfterCompact(base, newIndices);
	}

	// update face references in the Edge Container, if it exists
	if constexpr (vcl::mesh::hasEdges<M>()) {
		using EdgeContainer = typename M::EdgeContainer;
		EdgeContainer::updateFaceReferencesAfterCompact(base, newIndices);
	}
}

template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasEdges<M>(), void)
Mesh<Args...>::updateEdgeReferences(
	const typename M::EdgeType* oldBase,
	const typename M::EdgeType* newBase)
{
	// update edge references in the Vertex Container, if it exists
	if constexpr (vcl::mesh::hasVertices<M>()) {
		using VertexContainer = typename M::VertexContainer;
		VertexContainer::updateEdgeReferences(oldBase, newBase);
	}

	// update edge references in the Face Container, if it exists
	if constexpr (vcl::mesh::hasFaces<M>()) {
		using FaceContainer = typename M::FaceContainer;
		FaceContainer::updateEdgeReferences(oldBase, newBase);
	}
}

template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasEdges<M>(), void)
Mesh<Args...>::updateEdgeReferencesAfterCompact(
	const typename M::EdgeType* base,
	const std::vector<int>&     newIndices)
{
	// update edge references in the Vertex Container, if it exists
	if constexpr (vcl::mesh::hasVertices<M>()) {
		using VertexContainer = typename M::VertexContainer;
		VertexContainer::updateEdgeReferencesAfterCompact(base, newIndices);
	}

	// update edge references in the Face Container, if it exists
	if constexpr (vcl::mesh::hasEdges<M>()) {
		using FaceContainer = typename M::FaceContainer;
		FaceContainer::updateEdgeReferencesAfterCompact(base, newIndices);
	}
}

template<typename... Args>
void Mesh<Args...>::updateAllOptionalContainerReferences()
{
	// if there is the optional vertex container, I need to update, for each vertex of the
	// new mesh, the containerPointer
	if constexpr (mesh::hasVertexOptionalContainer<Mesh<Args...>>()) {
		using VertexContainer = typename Mesh<Args...>::VertexContainer;
		for (auto& v : VertexContainer::vertices(true)) {
			VertexContainer::setContainerPointer(v);
		}
	}

	// if there is the optional face container, I need to update, for each face of the
	// new mesh, the containerPointer
	if constexpr (mesh::hasFaceOptionalContainer<Mesh<Args...>>()) {
		using FaceContainer = typename Mesh<Args...>::FaceContainer;
		for (auto& f : FaceContainer::faces(true)) {
			FaceContainer::setContainerPointer(f);
		}
	}

	// if there is the optional edge container, I need to update, for each edge of the
	// new mesh, the containerPointer
	if constexpr (mesh::hasEdgeOptionalContainer<Mesh<Args...>>()) {
		using EdgeContainer = typename Mesh<Args...>::EdgeContainer;
		for (auto& e : EdgeContainer::edges(true)) {
			EdgeContainer::setContainerPointer(e);
		}
	}
}

template<typename... Args>
template<typename M>
VCL_ENABLE_IF(mesh::hasFaces<M>() && mesh::hasVertices<M>(), void)
Mesh<Args...>::addFaceHelper(typename M::FaceType&)
{
	// base case: no need to add any other vertices
}

template<typename... Args>
template<typename M, typename... V>
VCL_ENABLE_IF(mesh::hasFaces<M>() && mesh::hasVertices<M>(), void)
Mesh<Args...>::addFaceHelper(typename M::FaceType& f, typename M::VertexType* v, V... args)
{
	using FaceContainer = typename Mesh<Args...>::FaceContainer;

	// position on which add the vertex
	const std::size_t n = f.vertexNumber() - sizeof...(args) - 1;
	f.vertex(n)         = v;   // set the vertex
	addFaceHelper(f, args...); // set the remanining vertices, recursive variadics
}

template<typename... Args>
template<typename M, typename... V>
VCL_ENABLE_IF(mesh::hasFaces<M>() && mesh::hasVertices<M>(), void)
Mesh<Args...>::addFaceHelper(typename M::FaceType& f, uint vid, V... args)
{
	using FaceContainer   = typename Mesh<Args...>::FaceContainer;
	using VertexContainer = typename Mesh<Args...>::VertexContainer;

	// position on which add the vertex
	const std::size_t n = f.vertexNumber() - sizeof...(args) - 1;
	f.vertex(n)         = &VertexContainer::vertex(vid); // set the vertex
	addFaceHelper(f, args...); // set the remanining vertices, recursive variadics
}

/**
 * @brief Swaps two meshes of the same type
 */
template<typename... A>
inline void swap(Mesh<A...>& m1, Mesh<A...>& m2)
{
	// container bases of verts and faces, and edges Vec for m1 and m2
	void* m1BaseV = nullptr;
	void* m2BaseV = nullptr;
	void* m1BaseF = nullptr;
	void* m2BaseF = nullptr;
	void* m1BaseE = nullptr;
	void* m2BaseE = nullptr;

	// save the bases of the containers before swap
	if constexpr (mesh::hasVertices<Mesh<A...>>()) {
		using VertexContainer = typename Mesh<A...>::VertexContainer;
		m1BaseV               = m1.VertexContainer::vec.data();
		m2BaseV               = m2.VertexContainer::vec.data();
	}
	if constexpr (mesh::hasFaces<Mesh<A...>>()) {
		using FaceContainer = typename Mesh<A...>::FaceContainer;
		m1BaseF             = m1.FaceContainer::vec.data();
		m2BaseF             = m2.FaceContainer::vec.data();
	}
	if constexpr (mesh::hasEdges<Mesh<A...>>()) {
		using EdgeContainer = typename Mesh<A...>::EdgeContainer;
		m1BaseE             = m1.EdgeContainer::vec.data();
		m2BaseE             = m2.EdgeContainer::vec.data();
	}

	// actual swap of all the containers and the components of the mesh
	// using pack expansion: swap will be called for each of the containers (or components!) that
	// compose the Mesh
	using std::swap;
	(swap((A&) m1, (A&) m2), ...);

	// set to all the elements, the new pointer of the optional containers
	m1.updateAllOptionalContainerReferences();
	m2.updateAllOptionalContainerReferences();

	// update all the references to m1 and m2: old base of m1 is now "old base" of m2, and viceversa
	if constexpr (mesh::hasVertices<Mesh<A...>>()) {
		using VertexType      = typename Mesh<A...>::VertexType;
		using VertexContainer = typename Mesh<A...>::VertexContainer;
		m1.updateVertexReferences((VertexType*) m2BaseV, m1.VertexContainer::vec.data());
		m2.updateVertexReferences((VertexType*) m1BaseV, m2.VertexContainer::vec.data());
	}
	if constexpr (mesh::hasFaces<Mesh<A...>>()) {
		using FaceType      = typename Mesh<A...>::FaceType;
		using FaceContainer = typename Mesh<A...>::FaceContainer;
		m1.updateFaceReferences((FaceType*) m2BaseF, m1.FaceContainer::vec.data());
		m2.updateFaceReferences((FaceType*) m1BaseF, m2.FaceContainer::vec.data());
	}
	if constexpr (mesh::hasEdges<Mesh<A...>>()) {
		using EdgeType      = typename Mesh<A...>::EdgeType;
		using EdgeContainer = typename Mesh<A...>::EdgeContainer;
		m1.updateEdgeReferences((EdgeType*) m2BaseE, m1.EdgeContainer::vec.data());
		m2.updateEdgeReferences((EdgeType*) m1BaseE, m2.EdgeContainer::vec.data());
	}
}

} // namespace vcl
