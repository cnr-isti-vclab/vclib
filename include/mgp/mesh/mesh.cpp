/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "mesh.h"

namespace mgp {

/**
 * @brief Mesh::Mesh Copy constructor of the Mesh. Will create a deep copy of the given input mesh,
 * taking care of copying everithing and then update all the references
 * @param oth
 */
template<class... Args>
Mesh<Args...>::Mesh(const Mesh<Args...>& oth) :
		mesh::Container<Args>(
			oth)... // call auto copy constructors for all the container elements and properties
{
	// update all the optional container references
	updateAllOptionalContainerReferences();

	// update references into the vertex container
	if constexpr (mesh::hasVertices<Mesh<Args...>>()) {
		using VertexContainer = typename Mesh<Args...>::VertexContainer;
		// just run the same function that we use when vector is reallocated, but using
		// as old base the base of the other vertex container data
		updateVertexReferences(oth.vertices.data(), VertexContainer::vertices.data());
	}

	// update references into the face container
	if constexpr (mesh::hasFaces<Mesh<Args...>>()) {
		using FaceContainer = typename Mesh<Args...>::FaceContainer;
		// just run the same function that we use when vector is reallocated, but using
		// as old base the base of the other vertex container data
		updateFaceReferences(oth.faces.data(), FaceContainer::faces.data());
	}
}

template<class... Args>
Mesh<Args...>::Mesh(Mesh<Args...>&& oth)
{
	swap(oth);
}

template<class... Args>
void Mesh<Args...>::clear()
{
	if constexpr(mesh::hasVertices<Mesh<Args...>>()) {
		using VertexContainer = typename Mesh<Args...>::VertexContainer;
		VertexContainer::clearVertices();
	}
	if constexpr(mesh::hasFaces<Mesh<Args...>>()) {
		using FaceContainer = typename Mesh<Args...>::FaceContainer;
		FaceContainer::clearFaces();
	}
}

template<class... Args>
void Mesh<Args...>::swap(Mesh& m2)
{
	mgp::swap(*this, m2);
}

template<class... Args>
Mesh<Args...>& Mesh<Args...>::operator=(Mesh<Args...> oth)
{
	swap(oth);
	return *this;
}

template<class... Args>
void Mesh<Args...>::updateAllOptionalContainerReferences()
{
	// if there the optional vertex container, I need to update, for each vertex of the
	// new mesh, the containerPointer
	if constexpr (
		mesh::hasVertices<Mesh<Args...>>() && mesh::hasVertexOptionalContainer<Mesh<Args...>>()) {
		using VertexContainer = typename Mesh<Args...>::VertexContainer;
		for (auto& v : VertexContainer::vertexIterator(true)) {
			VertexContainer::setContainerPointer(v);
		}
	}

	// if there is the optional face container, I need to update, for each face of the
	// new mesh, the containerPointer
	if constexpr (
		mesh::hasFaces<Mesh<Args...>>() && mesh::hasFaceOptionalContainer<Mesh<Args...>>()) {
		using FaceContainer = typename Mesh<Args...>::FaceContainer;
		for (auto& f : FaceContainer::faceIterator(true)) {
			FaceContainer::setContainerPointer(f);
		}
	}
}

template<class... Args>
template<typename U>
mesh::ReturnIfHasVertexContainer<U, unsigned int> mgp::Mesh<Args...>::addVertex()
{
	using Vertex          = typename U::VertexType;
	using VertexContainer = typename U::VertexContainer;

	Vertex*      oldBase = VertexContainer::vertices.data();
	unsigned int vid     = VertexContainer::addVertex();
	Vertex*      newBase = VertexContainer::vertices.data();
	if (oldBase != nullptr && oldBase != newBase)
		updateVertexReferences(oldBase, newBase);
	return vid;
}

template<class... Args>
template<typename U>
mesh::ReturnIfHasVertexContainer<U, unsigned int> mgp::Mesh<Args...>::addVertices(unsigned int n)
{
	using Vertex          = typename U::VertexType;
	using VertexContainer = typename U::VertexContainer;

	Vertex*      oldBase = VertexContainer::vertices.data();
	unsigned int vid     = VertexContainer::addVertices(n);
	Vertex*      newBase = VertexContainer::vertices.data();
	if (oldBase != nullptr && oldBase != newBase)
		updateVertexReferences(oldBase, newBase);
	return vid;
}

template<class... Args>
template<typename U>
mesh::ReturnIfHasVertexContainer<U, void> mgp::Mesh<Args...>::reserveVertices(unsigned int n)
{
	using Vertex          = typename U::VertexType;
	using VertexContainer = typename U::VertexContainer;

	Vertex*      oldBase = VertexContainer::vertices.data();
	VertexContainer::reserveVertices(n);
	Vertex*      newBase = VertexContainer::vertices.data();
	if (oldBase != nullptr && oldBase != newBase)
		updateVertexReferences(oldBase, newBase);
}

template<class... Args>
template<typename U>
mesh::ReturnIfHasVertexContainer<U, void> mgp::Mesh<Args...>::compactVertices()
{
	using Vertex          = typename U::VertexType;
	using VertexContainer = typename U::VertexContainer;

	Vertex*      oldBase = VertexContainer::vertices.data();
	std::vector<int> newIndices = VertexContainer::compactVertices();
	Vertex*      newBase = VertexContainer::vertices.data();
	assert(oldBase == newBase);

	updateVertexReferencesAfterCompact(oldBase, newIndices);
}

template<class... Args>
template<typename U>
mesh::ReturnIfHasFaceContainer<U, unsigned int> Mesh<Args...>::addFace()
{
	using Face          = typename U::FaceType;
	using FaceContainer = typename U::FaceContainer;

	Face*        oldBase = FaceContainer::faces.data();
	unsigned int fid     = FaceContainer::addFace();
	Face*        newBase = FaceContainer::faces.data();
	if (oldBase != nullptr && oldBase != newBase)
		updateFaceReferences(oldBase, newBase);
	return fid;
}

template<class... Args>
template<typename U>
mesh::ReturnIfHasFaceContainer<U, unsigned int> Mesh<Args...>::addFaces(unsigned int n)
{
	using Face          = typename U::FaceType;
	using FaceContainer = typename U::FaceContainer;

	Face*        oldBase = FaceContainer::faces.data();
	unsigned int fid     = FaceContainer::addFaces(n);
	Face*        newBase = FaceContainer::faces.data();
	if (oldBase != nullptr && oldBase != newBase)
		updateFaceReferences(oldBase, newBase);
	return fid;
}

template<class... Args>
template<typename U>
mesh::ReturnIfHasFaceContainer<U, void> Mesh<Args...>::reserveFaces(unsigned int n)
{
	using Face          = typename U::FaceType;
	using FaceContainer = typename U::FaceContainer;

	Face*      oldBase = FaceContainer::faces.data();
	FaceContainer::reserveFaces(n);
	Face*      newBase = FaceContainer::faces.data();
	if (oldBase != nullptr && oldBase != newBase)
		updateFaceReferences(oldBase, newBase);
}

template<class... Args>
template<typename U>
mesh::ReturnIfHasFaceContainer<U, void> mgp::Mesh<Args...>::compactFaces()
{
	using Face          = typename U::FaceType;
	using FaceContainer = typename U::FaceContainer;

	Face*      oldBase = FaceContainer::faces.data();
	std::vector<int> newIndices = FaceContainer::compactFaces();
	Face*      newBase = FaceContainer::faces.data();
	assert(oldBase == newBase);

	updateFaceReferencesAfterCompact(oldBase, newIndices);
}

template<class... Args>
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

template<class... Args>
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

template<class... Args>
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

template<class... Args>
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

template<class...A>
inline void swap(Mesh<A...>& m1, Mesh<A...>& m2)
{
	void* m1BaseV = nullptr;
	void* m2BaseV = nullptr;
	void* m1BaseF = nullptr;
	void* m2BaseF = nullptr;

	// save the bases of the containers before swap
	if constexpr (mesh::hasVertices<Mesh<A...>>()) {
		m1BaseV = m1.vertices.data();
		m2BaseV = m2.vertices.data();
	}
	if constexpr (mesh::hasFaces<Mesh<A...>>()) {
		m1BaseF = m1.faces.data();
		m2BaseF = m2.faces.data();
	}

	// actual swap of all the containers and the properties of the mesh
	using std::swap;
	(swap((mgp::mesh::Container<A>&) m1, (mgp::mesh::Container<A>&) m2), ...);

	// set to all the elements, the new pointer of the optional component
	m1.updateAllOptionalContainerReferences();
	m2.updateAllOptionalContainerReferences();

	// update all the references to m1 and m2: old base of m1 is now "old base" of m2, and viceversa
	if constexpr (mesh::hasVertices<Mesh<A...>>()) {
		using VertexType = typename Mesh<A...>::VertexType;
		m1.updateVertexReferences((VertexType*)m2BaseV, m1.vertices.data());
		m2.updateVertexReferences((VertexType*)m1BaseV, m2.vertices.data());
	}
	if constexpr (mesh::hasFaces<Mesh<A...>>()) {
		using FaceType = typename Mesh<A...>::FaceType;
		m1.updateFaceReferences((FaceType*)m2BaseF, m1.faces.data());
		m2.updateFaceReferences((FaceType*)m1BaseF, m2.faces.data());
	}
}

} // namespace mgp
