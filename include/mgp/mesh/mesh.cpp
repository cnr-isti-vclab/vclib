/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "mesh.h"

namespace mgp {

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
		mesh::Container<Args>(
			oth)... // call auto copy constructors for all the container elements and components
{
	// update all the optional container references
	updateAllOptionalContainerReferences();

	// update references into the vertex container
	if constexpr (mesh::hasVertices<Mesh<Args...>>()) {
		using VertexContainer = typename Mesh<Args...>::VertexContainer;
		// just run the same function that we use when vector is reallocated, but using
		// as old base the base of the other vertex container data
		updateVertexReferences(oth.vertsVec.data(), VertexContainer::vertsVec.data());
	}

	// update references into the face container
	if constexpr (mesh::hasFaces<Mesh<Args...>>()) {
		using FaceContainer = typename Mesh<Args...>::FaceContainer;
		// just run the same function that we use when vector is reallocated, but using
		// as old base the base of the other vertex container data
		updateFaceReferences(oth.FaceContainer::vec.data(), FaceContainer::FaceContainer::vec.data());
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
	swap(oth); //use copy ans swap idiom: this (empty) mesh is swapped with the input one
}

/**
 * @brief Clears all the Elements contained in the mesh.
 */
template<typename... Args>
void Mesh<Args...>::clear()
{
	if constexpr(mesh::hasVertices<Mesh<Args...>>()) {
		using VertexContainer = typename Mesh<Args...>::VertexContainer;
		VertexContainer::clearVertices(); // clear vertices, only if the mesh has vertices
	}
	if constexpr(mesh::hasFaces<Mesh<Args...>>()) {
		using FaceContainer = typename Mesh<Args...>::FaceContainer;
		FaceContainer::clearFaces(); // clear faces, only if the mesh has faces
	}
}

template<typename... Args>
template<typename U>
mesh::ReturnIfHasVertexContainer<U, unsigned int> Mesh<Args...>::index(const typename Mesh::VertexType& v) const
{
	using VertexContainer = typename U::VertexContainer;
	return VertexContainer::index(&v);
}

template<typename... Args>
template<typename U>
mesh::ReturnIfHasVertexContainer<U, unsigned int> Mesh<Args...>::index(const typename Mesh::VertexType* v) const
{
	using VertexContainer = typename U::VertexContainer;
	return VertexContainer::index(v);
}

/**
 * @brief Add a new vertex into the mesh, returning the id of the added vertex.
 *
 * If the call of this function will cause a reallocation of the Vertex container, the function
 * will automatically take care of updating all the Vertex references contained in the Mesh.
 *
 * @note This function will be available only if the Mesh has the Vertex Container.
 *
 * @return the id of the new vertex.
 */
template<typename... Args>
template<typename U>
mesh::ReturnIfHasVertexContainer<U, unsigned int> Mesh<Args...>::addVertex()
{
	using Vertex          = typename U::VertexType;
	using VertexContainer = typename U::VertexContainer;

	// If the base pointer of the container of vertices changes, it means that all the vertex
	// references contained in the other elements need to be updated (the ones contained in the
	// vertex container are updated automatically)

	Vertex*      oldBase = VertexContainer::vertsVec.data();
	unsigned int vid     = VertexContainer::addVertex();
	Vertex*      newBase = VertexContainer::vertsVec.data();
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
 * @note This function will be available only if the Mesh has the Vertex Container.
 *
 * @param p: coordinate of the new vertex.
 * @return the id of the new vertex.
 */
template<typename... Args>
template<typename U>
mesh::ReturnIfHasVertexContainer<U, unsigned int> Mesh<Args...>::addVertex(
	const typename Mesh::VertexType::CoordType& p)
{
	using VertexContainer = typename U::VertexContainer;

	unsigned int vid = addVertex(); // using the previously defined addVertex function
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
 * @note This function will be available only if the Mesh has the Vertex Container.
 *
 * @param n: the number of vertices to add to the mesh.
 * @return the id of the first added vertex.
 */
template<typename... Args>
template<typename U>
mesh::ReturnIfHasVertexContainer<U, unsigned int> Mesh<Args...>::addVertices(unsigned int n)
{
	using Vertex          = typename U::VertexType;
	using VertexContainer = typename U::VertexContainer;

	// If the base pointer of the container of vertices changes, it means that all the vertex
	// references contained in the other elements need to be updated (the ones contained in the
	// vertex container are updated automatically)

	Vertex*      oldBase = VertexContainer::vertsVec.data();
	unsigned int vid     = VertexContainer::addVertices(n); // add the number of vertices
	Vertex*      newBase = VertexContainer::vertsVec.data();


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
 * @note This function will be available only if the Mesh has the Vertex Container.
 *
 * @param v: list of vertices
 * @return the id of the first added vertex.
 */
template<typename... Args>
template<typename ...VC, typename U>
mesh::ReturnIfHasVertexContainer<U, unsigned int> Mesh<Args...>::addVertices(
	const VC&... v) // parameter pack of points
{
	using VertexContainer = typename U::VertexContainer;
	unsigned int vid = VertexContainer::vertexContainerSize();
	reserveVertices(vid + sizeof...(VC)); // reserve the new number of vertices

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
 * @note This function will be available only if the Mesh has the Vertex Container.
 *
 * @param n: the new capacity of the vertex container.
 */
template<typename... Args>
template<typename U>
mesh::ReturnIfHasVertexContainer<U, void> Mesh<Args...>::reserveVertices(unsigned int n)
{
	using Vertex          = typename U::VertexType;
	using VertexContainer = typename U::VertexContainer;

	Vertex*      oldBase = VertexContainer::vertsVec.data();
	VertexContainer::reserveVertices(n);
	Vertex*      newBase = VertexContainer::vertsVec.data();
	if (oldBase != nullptr && oldBase != newBase)
		updateVertexReferences(oldBase, newBase);
}

/**
 * @brief Compacts the Vertex Container, removing all the vertices marked as deleted. Vertices ids
 * will change assigning to each vertex id their new position in the container. The function
 * will automatically take care of updating all the Vertex references contained in the Mesh.
 *
 * @note This function will be available only if the Mesh has the Vertex Container.
 */
template<typename... Args>
template<typename U>
mesh::ReturnIfHasVertexContainer<U, void> Mesh<Args...>::compactVertices()
{
	using Vertex          = typename U::VertexType;
	using VertexContainer = typename U::VertexContainer;

	Vertex*      oldBase = VertexContainer::vertsVec.data();
	std::vector<int> newIndices = VertexContainer::compactVertices();
	Vertex*      newBase = VertexContainer::vertsVec.data();
	assert(oldBase == newBase);

	updateVertexReferencesAfterCompact(oldBase, newIndices);
}

template<typename... Args>
template<typename U>
mesh::ReturnIfHasFaceContainer<U, unsigned int> Mesh<Args...>::index(const typename Mesh::FaceType& f) const
{
	using FaceContainer = typename U::FaceContainer;
	return FaceContainer::index(&f);
}

template<typename... Args>
template<typename U>
mesh::ReturnIfHasFaceContainer<U, unsigned int> Mesh<Args...>::index(const typename Mesh::FaceType* f) const
{
	using FaceContainer = typename U::FaceContainer;
	return FaceContainer::index(f);
}

template<typename... Args>
template<typename U>
mesh::ReturnIfHasFaceContainer<U, unsigned int> Mesh<Args...>::addFace()
{
	using Face          = typename U::FaceType;
	using FaceContainer = typename U::FaceContainer;

	Face*        oldBase = FaceContainer::vec.data();
	unsigned int fid     = FaceContainer::addFace();
	Face*        newBase = FaceContainer::vec.data();
	if (oldBase != nullptr && oldBase != newBase)
		updateFaceReferences(oldBase, newBase);
	return fid;
}

template<typename... Args>
template<typename U, typename... V>
mesh::ReturnIfHasFaceContainer<U, unsigned int> Mesh<Args...>::addFace(V... args)
{
	return addFace({args...});
}

template<typename... Args>
template<typename U>
mesh::ReturnIfHasFaceContainer<U, unsigned int> Mesh<Args...>::addFace(
	const std::vector<typename Mesh::VertexType*>& v)
{
	using FaceContainer = typename U::FaceContainer;

	unsigned int fid = addFace();

	FaceContainer::face(fid).setVertices(v);

	return fid;
}

template<typename... Args>
template<typename U>
mesh::ReturnIfHasFaceContainer<U, unsigned int> Mesh<Args...>::addFaces(unsigned int n)
{
	using Face          = typename U::FaceType;
	using FaceContainer = typename U::FaceContainer;

	Face*        oldBase = FaceContainer::facesVec.data();
	unsigned int fid     = FaceContainer::addFaces(n);
	Face*        newBase = FaceContainer::facesVec.data();
	if (oldBase != nullptr && oldBase != newBase)
		updateFaceReferences(oldBase, newBase);
	return fid;
}

template<typename... Args>
template<typename U>
mesh::ReturnIfHasFaceContainer<U, void> Mesh<Args...>::reserveFaces(unsigned int n)
{
	using Face          = typename U::FaceType;
	using FaceContainer = typename U::FaceContainer;

	Face*      oldBase = FaceContainer::vec.data();
	FaceContainer::reserveFaces(n);
	Face*      newBase = FaceContainer::vec.data();
	if (oldBase != nullptr && oldBase != newBase)
		updateFaceReferences(oldBase, newBase);
}

template<typename... Args>
template<typename U>
mesh::ReturnIfHasFaceContainer<U, void> Mesh<Args...>::compactFaces()
{
	using Face          = typename U::FaceType;
	using FaceContainer = typename U::FaceContainer;

	Face*      oldBase = FaceContainer::vec.data();
	std::vector<int> newIndices = FaceContainer::compactFaces();
	Face*      newBase = FaceContainer::vec.data();
	assert(oldBase == newBase);

	updateFaceReferencesAfterCompact(oldBase, newIndices);
}

/**
 * @brief Swaps this mesh with the other input Mesh m2.
 * @param m2: the Mesh to swap with this Mesh.
 */
template<typename... Args>
void Mesh<Args...>::swap(Mesh& m2)
{
	mgp::swap(*this, m2);
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
template<typename U>
mesh::ReturnIfHasVertexContainer<U, void> Mesh<Args...>::updateVertexReferences(
	const typename U::VertexType* oldBase,
	const typename U::VertexType* newBase)
{
	// update vertex references in the Face Container, if it exists
	if constexpr (mgp::mesh::hasFaces<U>()) {
		using FaceContainer = typename U::FaceContainer;
		FaceContainer::updateVertexReferences(oldBase, newBase);
	}
}

template<typename... Args>
template<typename U>
mesh::ReturnIfHasVertexContainer<U, void> Mesh<Args...>::updateVertexReferencesAfterCompact(
	const typename U::VertexType* base,
	const std::vector<int>& newIndices)
{
	// update vertex references in the Face Container, if it exists
	if constexpr (mgp::mesh::hasFaces<U>()) {
		using FaceContainer = typename U::FaceContainer;
		FaceContainer::updateVertexReferencesAfterCompact(base, newIndices);
	}
}

template<typename... Args>
template<typename U>
mesh::ReturnIfHasFaceContainer<U, void> Mesh<Args...>::updateFaceReferences(
	const typename U::FaceType* oldBase,
	const typename U::FaceType* newBase)
{
	// update face references in the Vertex Container, if it exists
	if constexpr (mgp::mesh::hasVertices<U>()) {
		using VertexContainer = typename U::VertexContainer;
		VertexContainer::updateFaceReferences(oldBase, newBase);
	}
}

template<typename... Args>
template<typename U>
mesh::ReturnIfHasFaceContainer<U, void> Mesh<Args...>::updateFaceReferencesAfterCompact(
	const typename U::FaceType* base,
	const std::vector<int>& newIndices)
{
	// update vertex references in the Face Container, if it exists
	if constexpr (mgp::mesh::hasVertices<U>()) {
		using VertexContainer = typename U::VertexContainer;
		VertexContainer::updateFaceReferencesAfterCompact(base, newIndices);
	}
}

template<typename... Args>
void Mesh<Args...>::updateAllOptionalContainerReferences()
{
	// if there is the optional vertex container, I need to update, for each vertex of the
	// new mesh, the containerPointer
	if constexpr (
		mesh::hasVertices<Mesh<Args...>>() && mesh::hasVertexOptionalContainer<Mesh<Args...>>()) {
		using VertexContainer = typename Mesh<Args...>::VertexContainer;
		for (auto& v : VertexContainer::vertices(true)) {
			VertexContainer::setContainerPointer(v);
		}
	}

	// if there is the optional face container, I need to update, for each face of the
	// new mesh, the containerPointer
	if constexpr (
		mesh::hasFaces<Mesh<Args...>>() && mesh::hasFaceOptionalContainer<Mesh<Args...>>()) {
		using FaceContainer = typename Mesh<Args...>::FaceContainer;
		for (auto& f : FaceContainer::faces(true)) {
			FaceContainer::setContainerPointer(f);
		}
	}
}

/**
 * @brief Swaps two meshes of the same type
 * @param m1
 * @param m2
 */
template<typename...A>
inline void swap(Mesh<A...>& m1, Mesh<A...>& m2)
{
	// container bases of verts and facesVec for m1 and m2
	void* m1BaseV = nullptr;
	void* m2BaseV = nullptr;
	void* m1BaseF = nullptr;
	void* m2BaseF = nullptr;

	// save the bases of the containers before swap
	if constexpr (mesh::hasVertices<Mesh<A...>>()) {
		m1BaseV = m1.vertsVec.data();
		m2BaseV = m2.vertsVec.data();
	}
	if constexpr (mesh::hasFaces<Mesh<A...>>()) {
		using FaceContainer = typename Mesh<A...>::FaceContainer;
		m1BaseF = m1.FaceContainer::vec.data();
		m2BaseF = m2.FaceContainer::vec.data();
	}

	// actual swap of all the containers and the components of the mesh
	// using pack expansion: swap will be called for each of the containers (or components!) that
	// compose the Mesh
	using std::swap;
	(swap((mgp::mesh::Container<A>&) m1, (mgp::mesh::Container<A>&) m2), ...);

	// set to all the elements, the new pointer of the optional containers
	m1.updateAllOptionalContainerReferences();
	m2.updateAllOptionalContainerReferences();

	// update all the references to m1 and m2: old base of m1 is now "old base" of m2, and viceversa
	if constexpr (mesh::hasVertices<Mesh<A...>>()) {
		using VertexType = typename Mesh<A...>::VertexType;
		m1.updateVertexReferences((VertexType*)m2BaseV, m1.vertsVec.data());
		m2.updateVertexReferences((VertexType*)m1BaseV, m2.vertsVec.data());
	}
	if constexpr (mesh::hasFaces<Mesh<A...>>()) {
		using FaceType = typename Mesh<A...>::FaceType;
		using FaceContainer = typename Mesh<A...>::FaceContainer;
		m1.updateFaceReferences((FaceType*)m2BaseF, m1.FaceContainer::vec.data());
		m2.updateFaceReferences((FaceType*)m1BaseF, m2.FaceContainer::vec.data());
	}
}

} // namespace mgp
