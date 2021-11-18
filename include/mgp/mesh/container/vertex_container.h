/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_CONTAINER_VERTEX_CONTAINER_H
#define MGP_MESH_CONTAINER_VERTEX_CONTAINER_H

#include <mgp/mesh/elements/vertex.h>

#include "element_container.h"
#include "../iterators/container_iterator.h"
#include "../iterators/container_range_iterator.h"

namespace mgp::mesh {

class VertexContainerTriggerer
{
};

/**
 * @brief The Vertex Container class, will be used when the template argument given to the Mesh is a
 * mgp::Vertex.
 *
 * This class adds a container (vector) of vertices to the Mesh, making available the accessors
 * members to the vertices, the vertex number, iterators... This class will also take care to add
 * enablers/disablers of the eventual optional components of the vertex.
 */
template<typename T>
class VertexContainer : public ElementContainer<T>, public VertexContainerTriggerer
{
	static_assert(
		mgp::vert::hasBitFlags<T>(),
		"You should include BitFlags as Vertex component in your Mesh definition.");

	static_assert(
		mgp::vert::hasCoordinate<T>(),
		"You should include Coordinate as Vertex component in your Mesh definition.");

	using Base = ElementContainer<T>;
	using VertexContainerType = VertexContainer<T>;

public:
	using VertexType          = T;
	using VertexCoordType     = typename T::CoordType;
	using VertexIterator      = ContainerIterator<T>;
	using ConstVertexIterator = ConstContainerIterator<T>;
	using VertexRangeIterator = ContainerRangeIterator<VertexContainerType, VertexIterator>;
	using ConstVertexRangeIterator =
		ConstContainerRangeIterator<VertexContainerType, ConstVertexIterator>;

	VertexContainer();

	const VertexType& vertex(unsigned int i) const;
	VertexType&       vertex(unsigned int i);

	unsigned int vertexNumber() const;
	unsigned int vertexContainerSize() const;
	unsigned int deletedVertexNumber() const;

	void deleteVertex(unsigned int i);
	void deleteVertex(const VertexType* v);

	unsigned int vertexIndexIfCompact(unsigned int id) const;

	VertexIterator           vertexBegin(bool jumpDeleted = true);
	VertexIterator           vertexEnd();
	ConstVertexIterator      vertexBegin(bool jumpDeleted = true) const;
	ConstVertexIterator      vertexEnd() const;
	VertexRangeIterator      vertices(bool jumpDeleted = true);
	ConstVertexRangeIterator vertices(bool jumpDeleted = true) const;

	// Color
	template<typename U = T>
	vert::ReturnIfHasOptionalColor<U, bool> isPerVertexColorEnabled() const;

	template<typename U = T>
	vert::ReturnIfHasOptionalColor<U, void> enablePerVertexColor();

	template<typename U = T>
	vert::ReturnIfHasOptionalColor<U, void> disablePerVertexColor();

	// Mutable Bit Flags
	template<typename U = T>
	vert::ReturnIfHasOptionalMutableBitFlags<U, bool> isPerVertexMutableBitFlagsEnabled() const;

	template<typename U = T>
	vert::ReturnIfHasOptionalMutableBitFlags<U, void> enablePerVertexMutableBitFlags();

	template<typename U = T>
	vert::ReturnIfHasOptionalMutableBitFlags<U, void> disablePerVertexMutableBitFlags();

	// Normal
	template<typename U = T>
	vert::ReturnIfHasOptionalNormal<U, bool> isPerVertexNormalEnabled() const;

	template<typename U = T>
	vert::ReturnIfHasOptionalNormal<U, void> enablePerVertexNormal();

	template<typename U = T>
	vert::ReturnIfHasOptionalNormal<U, void> disablePerVertexNormal();

	// Scalar
	template<typename U = T>
	vert::ReturnIfHasOptionalScalar<U, bool> isPerVertexScalarEnabled() const;

	template<typename U = T>
	vert::ReturnIfHasOptionalScalar<U, void> enablePerVertexScalar();

	template<typename U = T>
	vert::ReturnIfHasOptionalScalar<U, void> disablePerVertexScalar();

	// TexCoord
	template<typename U = T>
	vert::ReturnIfHasOptionalTexCoord<U, bool> isPerVertexTexCoordEnabled() const;

	template<typename U = T>
	vert::ReturnIfHasOptionalTexCoord<U, void> enablePerVertexTexCoord();

	template<typename U = T>
	vert::ReturnIfHasOptionalTexCoord<U, void> disablePerVertexTexCoord();

	// Adjacent Faces
	template<typename U = T>
	vert::ReturnIfHasOptionalAdjacentFaces<U, bool> isPerVertexAdjacentFacesEnabled() const;

	template<typename U = T>
	vert::ReturnIfHasOptionalAdjacentFaces<U, void> enablePerVertexAdjacentFaces();

	template<typename U = T>
	vert::ReturnIfHasOptionalAdjacentFaces<U, void> disablePerVertexAdjacentFaces();

	// Adjacent Vertices
	template<typename U = T>
	vert::ReturnIfHasOptionalAdjacentVertices<U, bool> isPerVertexAdjacentVerticesEnabled() const;

	template<typename U = T>
	vert::ReturnIfHasOptionalAdjacentVertices<U, void> enablePerVertexAdjacentVertices();

	template<typename U = T>
	vert::ReturnIfHasOptionalAdjacentVertices<U, void> disablePerVertexAdjacentVertices();

	// Custom Components
	template<typename K, typename U = T>
	vert::ReturnIfHasCustomComponents<U, void> addPerVertexCustomComponent(const std::string& name);

protected:
	/**
	 * @brief vn: the number of vertices in the container. Could be different from vertices.size()
	 * due to vertices marked as deleted into the container.
	 */
	unsigned int vn = 0;

	unsigned int index(const VertexType* v) const;

	void clearVertices();

	unsigned int addVertex();
	unsigned int addVertices(unsigned int nVertices);
	void         reserveVertices(unsigned int size);

	void setContainerPointer(VertexType& v);

	std::vector<int> compactVertices();

	void updateAfterAllocation(const T* oldBase, const T* newBase);

	void updateContainerPointers();

	void updateVertexReferences(const T* oldBase, const T* newBase);

	void updateVertexReferencesAfterCompact(const T* base, const std::vector<int>& newIndices);

	template<typename Face>
	void updateFaceReferences(const Face* oldBase, const Face* newBase);

	template<typename Face>
	void updateFaceReferencesAfterCompact(const Face* base, const std::vector<int>& newIndices);
};

/**
 * Detector to check if a class has (inherits) a VertexContainer
 */

template<typename T>
using hasVertexContainerT = std::is_base_of<VertexContainerTriggerer, T>;

template<typename U, typename T>
using ReturnIfHasVertexContainer = typename std::enable_if<hasVertexContainerT<U>::value, T>::type;

template<typename T>
constexpr bool hasVertices()
{
	return hasVertexContainerT<T>::value;
}

template<typename T>
constexpr bool hasVertexOptionalContainer()
{
	return comp::hasOptionalInfo<typename T::VertexType>();
}

} // namespace mgp::mesh

#include "vertex_container.cpp"

#endif // MGP_MESH_CONTAINER_VERTEX_CONTAINER_H
