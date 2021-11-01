/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_CONTAINER_VERTEX_CONTAINER_H
#define MGP_MESH_CONTAINER_VERTEX_CONTAINER_H

#include "container_t.h"

#include "vertex_optional_container.h"
#include "../iterators/container_iterator.h"
#include "../iterators/container_range_iterator.h"

namespace mgp::mesh {

class VertexContainerTriggerer {};

// to shorten triggerer for Vertex class
template<class T>
using IfIsVertex = std::enable_if_t<std::is_base_of<VertexTriggerer, T>::value>;

/**
 * @brief The Vertex Container class, will be used when the template argument given to the Mesh is a
 * mgp::Vertex.
 *
 * This class adds a container (vector) of vertices to the Mesh, making available the accessors
 * members to the vertices, the vertex number, iterators... This class will also take care to add
 * enablers/disablers of the eventual optional components of the vertex.
 */
template<class T>
class Container<T, IfIsVertex<T>> :
		public VertexOptionalContainer<T>, public VertexContainerTriggerer
{
	static_assert(
		mgp::vert::hasBitFlags<T>(),
		"You should include BitFlags as Vertex component in your Mesh definition.");

protected:
	// types:
	using VertexContainer = Container<T, IfIsVertex<T>>;
	using OptionalVertexContainer = VertexOptionalContainer<T>;

public:
	using VertexType               = T;
	using VertexIterator           = ContainerIterator<T>;
	using ConstVertexIterator      = ConstContainerIterator<T>;
	using VertexRangeIterator      = ContainerRangeIterator<VertexContainer, VertexIterator>;
	using ConstVertexRangeIterator = ConstContainerRangeIterator<VertexContainer, ConstVertexIterator>;

	Container();

	const VertexType& vertex(unsigned int i) const;
	VertexType&       vertex(unsigned int i);

	unsigned int vertexNumber() const;
	unsigned int vertexContainerSize() const;
	unsigned int deletedVertexNumber() const;

	void deleteVertex(unsigned int i);

	VertexIterator           vertexBegin(bool jumpDeleted = true);
	VertexIterator           vertexEnd();
	ConstVertexIterator      vertexBegin(bool jumpDeleted = true) const;
	ConstVertexIterator      vertexEnd() const;
	VertexRangeIterator      vertexIterator(bool jumpDeleted = true);
	ConstVertexRangeIterator vertexIterator(bool jumpDeleted = true) const;

protected:
	/**
	 * @brief vertices: the vector of vertices, where each vertex contains only its static
	 * components. Optional components will be contained in the optionalComponentsVector.
	 */
	std::vector<T> vertices;

	/**
	 * @brief vn: the number of vertices in the container. Could be different from vertices.size()
	 * due to vertices marked as deleted into the container.
	 */
	unsigned int vn = 0;

	unsigned int addVertex();
	unsigned int addVertices(unsigned int nVertices);
	void reserveVertices(unsigned int size);

	std::vector<int> compactVertices();

	template<typename Face>
	void updateFaceReferences(const Face* oldBase, const Face* newBase);

	template<typename Face>
	void updateFaceReferencesAfterCompact(const Face* base, const std::vector<int>& newIndices);
};

/**
 * Detector to check if a class has (inherits) a VertexContainer
 */

template <typename  T>
using hasVertexContainerT = std::is_base_of<VertexContainerTriggerer, T>;

template<typename U, typename T>
using ReturnIfHasVertexContainer = typename std::enable_if<hasVertexContainerT<U>::value, T>::type;

template <typename T>
constexpr bool hasVertices() { return mesh::hasVertexContainerT<T>::value;}

} // namespace mgp::mesh

#include "vertex_container.cpp"

#endif // MGP_MESH_CONTAINER_VERTEX_CONTAINER_H
