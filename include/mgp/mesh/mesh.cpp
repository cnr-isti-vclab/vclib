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
Mesh<Args...>::Mesh(const Mesh& oth)
{
	// if has vertices
	if constexpr (mesh::hasVertices<Mesh<Args...>>()) {
		using VertexContainer = typename Mesh<Args...>::VertexContainer;

		// copy the vertex container and the vertex number
		// note: references will be updated later
		VertexContainer::vertices = oth.vertices;
		VertexContainer::vn = oth.vn;

		// if there is also the optional vertex container, I need to update, for each vertex of the
		// new mesh, the containerPointer
		if constexpr (mesh::hasVertexOptionalContainer<Mesh<Args...>>()) {
			VertexContainer::optionalPropVector = oth.VertexContainer::optionalPropVector;
			for (auto& v : VertexContainer::vertexIterator(true)){
				VertexContainer::setContainerPointer(v);
			}
		}
	}

	// if has faces
	if constexpr (mesh::hasFaces<Mesh<Args...>>()) {
		using FaceContainer = typename Mesh<Args...>::FaceContainer;

		// copy the face container and the face number
		// note: references will be updated later
		FaceContainer::faces = oth.faces;
		FaceContainer::fn = oth.fn;

		// if there is also the optional face container, I need to update, for each face of the
		// new mesh, the containerPointer
		if constexpr (mesh::hasFaceOptionalContainer<Mesh<Args...>>()) {
			for (auto& f : FaceContainer::faceIterator(true)){
				FaceContainer::setContainerPointer(f);
			}
		}
	}

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
mesh::ReturnIfHasVertexContainer<U, void> mgp::Mesh<Args...>::reserveVertices(unsigned int i)
{
	using Vertex          = typename U::VertexType;
	using VertexContainer = typename U::VertexContainer;

	Vertex*      oldBase = VertexContainer::vertices.data();
	VertexContainer::reserveVertices(i);
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
mesh::ReturnIfHasFaceContainer<U, void> Mesh<Args...>::reserveFaces(unsigned int i)
{
	using Face          = typename U::FaceType;
	using FaceContainer = typename U::FaceContainer;

	Face*      oldBase = FaceContainer::faces.data();
	FaceContainer::reserveFaces(i);
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

} // namespace mgp
