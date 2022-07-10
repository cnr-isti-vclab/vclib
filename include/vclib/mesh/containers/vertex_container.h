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

#ifndef VCL_MESH_CONTAINER_VERTEX_CONTAINER_H
#define VCL_MESH_CONTAINER_VERTEX_CONTAINER_H

#include <vclib/mesh/components/vertical/vectors/custom_component_vector_handle.h>
#include <vclib/iterators/container_iterator.h>
#include <vclib/iterators/container_range_iterator.h>
#include <vclib/mesh/elements/vertex.h>

#include "element_container.h"

namespace vcl::mesh {

/**
 * @brief The Vertex Container class, will be used when the template argument given to the Mesh is a
 * Vertex.
 *
 * This class adds a container (vector) of vertices to the Mesh, making available the accessors
 * members to the vertices, the vertex number, iterators... This class will also take care to add
 * enablers/disablers of the eventual optional components of the vertex.
 *
 * This container can be templated on a type that satisfies the VertexConcept concept.
 */
template<vcl::VertexConcept T>
class VertexContainer : protected ElementContainer<T>, public VertexContainerTriggerer
{
	template <VertexConcept U>
	friend class VertexContainer;

	using VertexContainerType = VertexContainer<T>;
	using Base                = ElementContainer<T>;

public:
	using Vertex                   = T;
	using VertexType               = T;
	using VertexIterator           = typename Base::ElementIterator;
	using ConstVertexIterator      = typename Base::ConstElementIterator;
	using VertexRangeIterator      = typename Base::ElementRangeIterator;
	using ConstVertexRangeIterator = typename Base::ConstElementRangeIterator;

	VertexContainer();

	const VertexType& vertex(uint i) const;
	VertexType&       vertex(uint i);

	uint vertexNumber() const;
	uint vertexContainerSize() const;
	uint deletedVertexNumber() const;

	void deleteVertex(uint i);
	void deleteVertex(const VertexType* v);

	uint             vertexIndexIfCompact(uint id) const;
	std::vector<int> vertexCompactIndices() const;

	VertexIterator           vertexBegin(bool jumpDeleted = true);
	VertexIterator           vertexEnd();
	ConstVertexIterator      vertexBegin(bool jumpDeleted = true) const;
	ConstVertexIterator      vertexEnd() const;
	VertexRangeIterator      vertices(bool jumpDeleted = true);
	ConstVertexRangeIterator vertices(bool jumpDeleted = true) const;

	void enableAllPerVertexOptionalComponents();
	void disableAllPerVertexOptionalComponents();

	// Adjacent Edges
	template<typename U = T>
	VCL_ENABLE_IF(vert::HasOptionalAdjacentEdges<U>, bool)
	isPerVertexAdjacentEdgesEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(vert::HasOptionalAdjacentEdges<U>, void)
	enablePerVertexAdjacentEdges();

	template<typename U = T>
	VCL_ENABLE_IF(vert::HasOptionalAdjacentEdges<U>, void)
	disablePerVertexAdjacentEdges();

	// Adjacent Faces
	template<typename U = T>
	VCL_ENABLE_IF(vert::HasOptionalAdjacentFaces<U>, bool)
	isPerVertexAdjacentFacesEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(vert::HasOptionalAdjacentFaces<U>, void)
	enablePerVertexAdjacentFaces();

	template<typename U = T>
	VCL_ENABLE_IF(vert::HasOptionalAdjacentFaces<U>, void)
	disablePerVertexAdjacentFaces();

	// Adjacent Vertices
	template<typename U = T>
	VCL_ENABLE_IF(vert::HasOptionalAdjacentVertices<U>, bool)
	isPerVertexAdjacentVerticesEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(vert::HasOptionalAdjacentVertices<U>, void)
	enablePerVertexAdjacentVertices();

	template<typename U = T>
	VCL_ENABLE_IF(vert::HasOptionalAdjacentVertices<U>, void)
	disablePerVertexAdjacentVertices();

	// Color
	template<typename U = T>
	VCL_ENABLE_IF(vert::HasOptionalColor<U>, bool)
	isPerVertexColorEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(vert::HasOptionalColor<U>, void)
	enablePerVertexColor();

	template<typename U = T>
	VCL_ENABLE_IF(vert::HasOptionalColor<U>, void)
	disablePerVertexColor();

	// Mark
	template<typename U = T>
	VCL_ENABLE_IF(vert::hasOptionalMark<U>(), bool)
	isPerVertexMarkEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(vert::hasOptionalMark<U>(), void)
	enablePerVertexMark();

	template<typename U = T>
	VCL_ENABLE_IF(vert::hasOptionalMark<U>(), void)
	disablePerVertexMark();

	// Normal
	template<typename U = T>
	VCL_ENABLE_IF(vert::hasOptionalNormal<U>(), bool)
	isPerVertexNormalEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(vert::hasOptionalNormal<U>(), void)
	enablePerVertexNormal();

	template<typename U = T>
	VCL_ENABLE_IF(vert::hasOptionalNormal<U>(), void)
	disablePerVertexNormal();

	// PrincipalCurvature
	template<typename U = T>
	VCL_ENABLE_IF(vert::hasOptionalPrincipalCurvature<U>(), bool)
	isPerVertexPrincipalCurvatureEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(vert::hasOptionalPrincipalCurvature<U>(), void)
	enablePerVertexPrincipalCurvature();

	template<typename U = T>
	VCL_ENABLE_IF(vert::hasOptionalPrincipalCurvature<U>(), void)
	disablePerVertexPrincipalCurvature();

	// Scalar
	template<typename U = T>
	VCL_ENABLE_IF(vert::hasOptionalScalar<U>(), bool)
	isPerVertexScalarEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(vert::hasOptionalScalar<U>(), void)
	enablePerVertexScalar();

	template<typename U = T>
	VCL_ENABLE_IF(vert::hasOptionalScalar<U>(), void)
	disablePerVertexScalar();

	// TexCoord
	template<typename U = T>
	VCL_ENABLE_IF(vert::hasOptionalTexCoord<U>(), bool)
	isPerVertexTexCoordEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(vert::hasOptionalTexCoord<U>(), void)
	enablePerVertexTexCoord();

	template<typename U = T>
	VCL_ENABLE_IF(vert::hasOptionalTexCoord<U>(), void)
	disablePerVertexTexCoord();

	// Custom Components
	template<typename U = T>
	VCL_ENABLE_IF(vert::HasCustomComponents<U>, bool)
	hasPerVertexCustomComponent(const std::string& name) const;

	template<typename U = T>
	VCL_ENABLE_IF(vert::HasCustomComponents<U>, std::vector<std::string>)
	getAllPerVertexCustomComponentNames() const;

	template<typename K, typename U = T>
	VCL_ENABLE_IF(vert::HasCustomComponents<U>, bool)
	isPerVertexCustomComponentOfType(const std::string& name) const;

	template<typename K, typename U = T>
	VCL_ENABLE_IF(vert::HasCustomComponents<U>, std::vector<std::string>)
	getPerVertexCustomComponentNamesOfType() const;

	template<typename K, typename U = T>
	VCL_ENABLE_IF(vert::HasCustomComponents<U>, void)
	addPerVertexCustomComponent(const std::string& name);

	template<typename U = T>
	VCL_ENABLE_IF(vert::HasCustomComponents<U>, void)
	deletePerVertexCustomComponent(const std::string& name);

	template<typename K, typename U = T>
	VCL_ENABLE_IF(vert::HasCustomComponents<U>, CustomComponentVectorHandle<K>)
	getPerVertexCustomComponentVectorHandle(const std::string& name);

	template<typename K, typename U = T>
	VCL_ENABLE_IF(vert::HasCustomComponents<U>, ConstCustomComponentVectorHandle<K>)
	getPerVertexCustomComponentVectorHandle(const std::string& name) const;

protected:
	uint index(const VertexType* v) const;

	void clearVertices();

	uint addVertex();
	uint addVertices(uint nVertices);
	void reserveVertices(uint size);

	std::vector<int> compactVertices();

	template<typename Mesh>
	void enableOptionalComponentsOf(const Mesh& m);

	template<typename Mesh>
	void importFrom(const Mesh& m);

	template<typename Mesh>
	void importVertexReferencesFrom(const Mesh& m, T* base);

	template<typename Mesh, typename Face>
	void importFaceReferencesFrom(const Mesh& m, Face* base);

	template<typename Mesh, typename Edge>
	void importEdgeReferencesFrom(const Mesh& m, Edge* base);
};

} // namespace vcl::mesh

#include "vertex_container.cpp"

#endif // VCL_MESH_CONTAINER_VERTEX_CONTAINER_H
