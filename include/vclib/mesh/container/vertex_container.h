/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

#ifndef VCL_MESH_CONTAINER_VERTEX_CONTAINER_H
#define VCL_MESH_CONTAINER_VERTEX_CONTAINER_H

#include <vclib/mesh/elements/vertex.h>

#include "element_container.h"
#include "../iterators/container_iterator.h"
#include "../iterators/container_range_iterator.h"

namespace vcl::mesh {

class VertexContainerTriggerer
{
};

/**
 * @brief The Vertex Container class, will be used when the template argument given to the Mesh is a
 * Vertex.
 *
 * This class adds a container (vector) of vertices to the Mesh, making available the accessors
 * members to the vertices, the vertex number, iterators... This class will also take care to add
 * enablers/disablers of the eventual optional components of the vertex.
 */
template<typename T>
class VertexContainer : public ElementContainer<T>, public VertexContainerTriggerer
{
	// Sanity checks for the Vertex
	static_assert(
		vcl::vert::hasBitFlags<T>(),
		"You should include BitFlags as Vertex component in your Mesh definition.");
	static_assert(
		vcl::vert::hasCoordinate<T>(),
		"You should include Coordinate as Vertex component in your Mesh definition.");

	using Base = ElementContainer<T>;
	using VertexContainerType = VertexContainer<T>;

public:
	using VertexType          = T;
	using VertexCoordType     = typename T::CoordType;
	using VertexIterator      = ContainerIterator<std::vector, T>;
	using ConstVertexIterator = ConstContainerIterator<std::vector, T>;
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

	// Color
	template<typename U = T>
	vert::ReturnIfHasOptionalColor<U, bool> isPerVertexColorEnabled() const;

	template<typename U = T>
	vert::ReturnIfHasOptionalColor<U, void> enablePerVertexColor();

	template<typename U = T>
	vert::ReturnIfHasOptionalColor<U, void> disablePerVertexColor();

	// Mark
	template<typename U = T>
	vert::ReturnIfHasOptionalMark<U, bool> isPerVertexMarkEnabled() const;

	template<typename U = T>
	vert::ReturnIfHasOptionalMark<U, void> enablePerVertexMark();

	template<typename U = T>
	vert::ReturnIfHasOptionalMark<U, void> disablePerVertexMark();

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

	// PrincipalCurvature
	template<typename U = T>
	vert::ReturnIfHasOptionalPrincipalCurvature<U, bool> isPerVertexPrincipalCurvatureEnabled() const;

	template<typename U = T>
	vert::ReturnIfHasOptionalPrincipalCurvature<U, void> enablePerVertexPrincipalCurvature();

	template<typename U = T>
	vert::ReturnIfHasOptionalPrincipalCurvature<U, void> disablePerVertexPrincipalCurvature();

	// TexCoord
	template<typename U = T>
	vert::ReturnIfHasOptionalTexCoord<U, bool> isPerVertexTexCoordEnabled() const;

	template<typename U = T>
	vert::ReturnIfHasOptionalTexCoord<U, void> enablePerVertexTexCoord();

	template<typename U = T>
	vert::ReturnIfHasOptionalTexCoord<U, void> disablePerVertexTexCoord();

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

} // namespace vcl::mesh

#include "vertex_container.cpp"

#endif // VCL_MESH_CONTAINER_VERTEX_CONTAINER_H
