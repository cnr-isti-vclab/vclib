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

#ifndef VCL_MESH_CONTAINER_EDGE_CONTAINER_H
#define VCL_MESH_CONTAINER_EDGE_CONTAINER_H

#include <vclib/mesh/containers/custom_component_vector_handle.h>
#include <vclib/mesh/elements/edge.h>
#include <vclib/mesh/elements/edge_components.h>

#include "element_container.h"

namespace vcl::mesh {

/**
 * @brief The EdgeContainer class
 *
 * @ingroup containers
 */
template<EdgeConcept T>
class EdgeContainer : public ElementContainer<T>
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

	EdgeContainer();

	const EdgeType& edge(uint i) const;
	EdgeType&       edge(uint i);

	uint edgeNumber() const;
	uint edgeContainerSize() const;
	uint deletedEdgeNumber() const;

	uint addEdge();
	uint addEdge(uint v0, uint v1);
	uint addEdge(typename T::VertexType* v0, typename T::VertexType* v1);
	uint addEdges(uint n);
	void reserveEdges(uint n);
	void compactEdges();

	void deleteEdge(uint i);
	void deleteEdge(const EdgeType* e);

	uint              edgeIndexIfCompact(uint id) const;
	std::vector<uint> edgeCompactIndices() const;

	void updateEdgeIndices(const std::vector<uint>& newIndices);

	EdgeIterator      edgeBegin(bool jumpDeleted = true);
	EdgeIterator      edgeEnd();
	ConstEdgeIterator edgeBegin(bool jumpDeleted = true) const;
	ConstEdgeIterator edgeEnd() const;
	auto              edges(bool jumpDeleted = true);
	auto              edges(bool jumpDeleted = true) const;

	void enableAllPerEdgeOptionalComponents();
	void disableAllPerEdgeOptionalComponents();

	// AdjacentEdges
	bool isPerEdgeAdjacentEdgesEnabled() const requires edge::HasOptionalAdjacentEdges<T>;
	void enablePerEdgeAdjacentEdges() requires edge::HasOptionalAdjacentEdges<T>;
	void disablePerEdgeAdjacentEdges() requires edge::HasOptionalAdjacentEdges<T>;

	// AdjacentFaces
	bool isPerEdgeAdjacentFacesEnabled() const requires edge::HasOptionalAdjacentFaces<T>;
	void enablePerEdgeAdjacentFaces() requires edge::HasOptionalAdjacentFaces<T>;
	void disablePerEdgeAdjacentFaces() requires edge::HasOptionalAdjacentFaces<T>;

	// Color
	bool isPerEdgeColorEnabled() const requires edge::HasOptionalColor<T>;
	void enablePerEdgeColor() requires edge::HasOptionalColor<T>;
	void disablePerEdgeColor() requires edge::HasOptionalColor<T>;

	// Mark
	bool isPerEdgeMarkEnabled() const requires edge::HasOptionalMark<T>;
	void enablePerEdgeMark() requires edge::HasOptionalMark<T>;
	void disablePerEdgeMark() requires edge::HasOptionalMark<T>;

	// Quality
	bool isPerEdgeQualityEnabled() const requires edge::HasOptionalQuality<T>;
	void enablePerEdgeQuality() requires edge::HasOptionalQuality<T>;
	void disablePerEdgeQuality() requires edge::HasOptionalQuality<T>;

	// Custom Components
	bool hasPerEdgeCustomComponent(const std::string& name) const
		requires edge::HasCustomComponents<T>;

	std::vector<std::string> perEdgeCustomComponentNames() const
		requires edge::HasCustomComponents<T>;

	template<typename K>
	bool isPerEdgeCustomComponentOfType(const std::string& name) const
		requires edge::HasCustomComponents<T>;

	std::type_index perEdgeCustomComponentType(const std::string& name) const
		requires edge::HasCustomComponents<T>;

	template<typename K>
	std::vector<std::string> perEdgeCustomComponentNamesOfType() const
		requires edge::HasCustomComponents<T>;

	template<typename K>
	void addPerEdgeCustomComponent(const std::string& name)
		requires edge::HasCustomComponents<T>;

	void deletePerEdgeCustomComponent(const std::string& name)
		requires edge::HasCustomComponents<T>;

	template<typename K>
	CustomComponentVectorHandle<K> perEdgeCustomComponentVectorHandle(const std::string& name)
		requires edge::HasCustomComponents<T>;

	template<typename K>
	ConstCustomComponentVectorHandle<K>
	perEdgeCustomComponentVectorHandle(const std::string& name) const
		requires edge::HasCustomComponents<T>;
};

} // namespace vcl::mesh

#include "edge_container.cpp"

#endif // VCL_MESH_CONTAINER_EDGE_CONTAINER_H
