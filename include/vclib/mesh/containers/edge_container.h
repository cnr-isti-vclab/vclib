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

#ifndef VCL_MESH_CONTAINER_EDGE_CONTAINER_H
#define VCL_MESH_CONTAINER_EDGE_CONTAINER_H

#include <vclib/iterators/container_iterator.h>
#include <vclib/iterators/container_range_iterator.h>
#include <vclib/mesh/elements/edge.h>

#include "../components/vertical/vectors/custom_component_vector_handle.h"
#include "element_container.h"

namespace vcl::mesh {

template<EdgeConcept T>
class EdgeContainer : protected ElementContainer<T>, public EdgeContainerTriggerer
{
	template <EdgeConcept U>
	friend class EdgeContainer;

	using EdgeContainerType = EdgeContainer<T>;
	using Base = ElementContainer<T>;

public:
	using Edge              = T;
	using EdgeType          = T;
	using EdgeIterator      = typename Base::ElementIterator;
	using ConstEdgeIterator = typename Base::ConstElementIterator;
	using EdgeRangeIterator = typename Base::ElementRangeIterator;
	using ConstEdgeRangeIterator = typename Base::ConstElementRangeIterator;

	EdgeContainer();

	const EdgeType& edge(uint i) const;
	EdgeType&       edge(uint i);

	uint edgeNumber() const;
	uint edgeContainerSize() const;
	uint deletedEdgeNumber() const;

	void deleteEdge(uint i);
	void deleteEdge(const EdgeType* e);

	uint             edgeIndexIfCompact(uint id) const;
	std::vector<int> edgeCompactIndices() const;

	EdgeIterator           edgeBegin(bool jumpDeleted = true);
	EdgeIterator           edgeEnd();
	ConstEdgeIterator      edgeBegin(bool jumpDeleted = true) const;
	ConstEdgeIterator      edgeEnd() const;
	EdgeRangeIterator      edges(bool jumpDeleted = true);
	ConstEdgeRangeIterator edges(bool jumpDeleted = true) const;

	void enableAllPerEdgeOptionalComponents();
	void disableAllPerEdgeOptionalComponents();

	// AdjacentEdges
	template<typename U = T>
	VCL_ENABLE_IF(edge::HasOptionalAdjacentEdges<U>, bool)
	isPerEdgeAdjacentEdgesEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(edge::HasOptionalAdjacentEdges<U>, void)
	enablePerEdgeAdjacentEdges();

	template<typename U = T>
	VCL_ENABLE_IF(edge::HasOptionalAdjacentEdges<U>, void)
	disablePerEdgeAdjacentEdges();

	// AdjacentFaces
	template<typename U = T>
	VCL_ENABLE_IF(edge::HasOptionalAdjacentFaces<U>, bool)
	isPerEdgeAdjacentFacesEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(edge::HasOptionalAdjacentFaces<U>, void)
	enablePerEdgeAdjacentFaces();

	template<typename U = T>
	VCL_ENABLE_IF(edge::HasOptionalAdjacentFaces<U>, void)
	disablePerEdgeAdjacentFaces();

	// Color
	template<typename U = T>
	VCL_ENABLE_IF(edge::HasOptionalColor<U>, bool)
	isPerEdgeColorEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(edge::HasOptionalColor<U>, void)
	enablePerEdgeColor();

	template<typename U = T>
	VCL_ENABLE_IF(edge::HasOptionalColor<U>, void)
	disablePerEdgeColor();

	// Mark
	template<typename U = T>
	VCL_ENABLE_IF(edge::hasOptionalMark<U>(), bool)
	isPerEdgeMarkEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(edge::hasOptionalMark<U>(), void)
	enablePerEdgeMark();

	template<typename U = T>
	VCL_ENABLE_IF(edge::hasOptionalMark<U>(), void)
	disablePerEdgeMark();

	// Scalar
	template<typename U = T>
	VCL_ENABLE_IF(edge::hasOptionalScalar<U>(), bool)
	isPerEdgeScalarEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(edge::hasOptionalScalar<U>(), void)
	enablePerEdgeScalar();

	template<typename U = T>
	VCL_ENABLE_IF(edge::hasOptionalScalar<U>(), void)
	disablePerEdgeScalar();

	// Custom Components
	template<typename U = T>
	VCL_ENABLE_IF(edge::HasCustomComponents<U>, bool)
	hasPerEdgeCustomComponent(const std::string& name) const;

	template<typename U = T>
	VCL_ENABLE_IF(edge::HasCustomComponents<U>, std::vector<std::string>)
	getAllPerEdgeCustomComponentNames() const;

	template<typename K, typename U = T>
	VCL_ENABLE_IF(edge::HasCustomComponents<U>, bool)
	isPerEdgeCustomComponentOfType(const std::string& name) const;

	template<typename K, typename U = T>
	VCL_ENABLE_IF(edge::HasCustomComponents<U>, std::vector<std::string>)
	getPerEdgeCustomComponentNamesOfType() const;

	template<typename K, typename U = T>
	VCL_ENABLE_IF(edge::HasCustomComponents<U>, void)
	addPerEdgeCustomComponent(const std::string& name);

	template<typename U = T>
	VCL_ENABLE_IF(edge::HasCustomComponents<U>, void)
	deletePerEdgeCustomComponent(const std::string& name);

	template<typename K, typename U = T>
	VCL_ENABLE_IF(edge::HasCustomComponents<U>, CustomComponentVectorHandle<K>)
	getPerEdgeCustomComponentVectorHandle(const std::string& name);

	template<typename K, typename U = T>
	VCL_ENABLE_IF(edge::HasCustomComponents<U>, ConstCustomComponentVectorHandle<K>)
	getPerEdgeCustomComponentVectorHandle(const std::string& name) const;

protected:
	uint index(const EdgeType* e) const;

	void clearEdges();

	uint addEdge();
	uint addEdges(uint nEdges);
	void reserveEdges(uint size);

	std::vector<int> compactEdges();

	template<typename Mesh>
	void enableOptionalComponentsOf(const Mesh& m);

	template<typename Mesh>
	void importFrom(const Mesh& m);

	template<typename Mesh, typename Vertex>
	void importVertexReferencesFrom(const Mesh& m, Vertex* base);

	template<typename Mesh, typename Face>
	void importFaceReferencesFrom(const Mesh& m, Face* base);

	template<typename Mesh>
	void importEdgeReferencesFrom(const Mesh& m, T* base);
};

} // namespace vcl::mesh

#include "edge_container.cpp"

#endif // VCL_MESH_CONTAINER_EDGE_CONTAINER_H
