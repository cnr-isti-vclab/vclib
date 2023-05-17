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

#ifndef VCL_MESH_CONTAINER_HALF_EDGE_CONTAINER_H
#define VCL_MESH_CONTAINER_HALF_EDGE_CONTAINER_H

#include <vclib/mesh/containers/custom_component_vector_handle.h>
#include <vclib/mesh/elements/half_edge.h>
#include <vclib/mesh/elements/half_edge_components.h>

#include "element_container.h"

namespace vcl::mesh {

/**
 * @brief The HalfEdgeContainer class
 *
 * @ingroup containers
 */
template<HalfEdgeConcept T>
class HalfEdgeContainer : public ElementContainer<T>
{
	template <HalfEdgeConcept U>
	friend class HalfEdgeContainer;

	using HalfEdgeContainerType = HalfEdgeContainer<T>;
	using Base = ElementContainer<T>;

public:
	using HalfEdge              = T;
	using HalfEdgeType          = T;
	using HalfEdgeIterator      = typename Base::ElementIterator;
	using ConstHalfEdgeIterator = typename Base::ConstElementIterator;

	HalfEdgeContainer();

	const HalfEdgeType& halfEdge(uint i) const;
	HalfEdgeType&       halfEdge(uint i);

	uint halfEdgeNumber() const;
	uint halfEdgeContainerSize() const;
	uint deletedHalfEdgeNumber() const;

	uint addHalfEdge();
	uint addHalfEdges(uint n);

	template<typename M = typename Base::ParentMeshType> requires HasFaces<M>
	uint addHalfEdgesToFace(uint n, typename M::FaceType& f);

	void reserveHalfEdges(uint n);
	void compactHalfEdges();

	void deleteHalfEdge(uint i);
	void deleteHalfEdge(const HalfEdgeType* e);

	uint             halfEdgeIndexIfCompact(uint id) const;
	std::vector<int> halfEdgeCompactIndices() const;

	HalfEdgeIterator      halfEdgeBegin(bool jumpDeleted = true);
	HalfEdgeIterator      halfEdgeEnd();
	ConstHalfEdgeIterator halfEdgeBegin(bool jumpDeleted = true) const;
	ConstHalfEdgeIterator halfEdgeEnd() const;
	auto                  halfEdges(bool jumpDeleted = true);
	auto                  halfEdges(bool jumpDeleted = true) const;

	void enableAllPerHalfEdgeOptionalComponents();
	void disableAllPerHalfEdgeOptionalComponents();

	// Color
	bool isPerHalfEdgeColorEnabled() const requires hedge::HasOptionalColor<T>;
	void enablePerHalfEdgeColor() requires hedge::HasOptionalColor<T>;
	void disablePerHalfEdgeColor() requires hedge::HasOptionalColor<T>;

	// Mark
	bool isPerHalfEdgeMarkEnabled() const requires hedge::HasOptionalMark<T>;
	void enablePerHalfEdgeMark() requires hedge::HasOptionalMark<T>;
	void disablePerHalfEdgeMark() requires hedge::HasOptionalMark<T>;

	// Quality
	bool isPerHalfEdgeQualityEnabled() const requires hedge::HasOptionalQuality<T>;
	void enablePerHalfEdgeQuality() requires hedge::HasOptionalQuality<T>;
	void disablePerHalfEdgeQuality() requires hedge::HasOptionalQuality<T>;

	// TexCoord
	bool isPerHalfEdgeTexCoordEnabled() const requires hedge::HasOptionalTexCoord<T>;
	void enablePerHalfEdgeTexCoord() requires hedge::HasOptionalTexCoord<T>;
	void disablePerHalfEdgeTexCoord() requires hedge::HasOptionalTexCoord<T>;

	// Custom Components
	bool hasPerHalfEdgeCustomComponent(const std::string& name) const
		requires hedge::HasCustomComponents<T>;

	std::vector<std::string> perHalfEdgeCustomComponentNames() const
		requires hedge::HasCustomComponents<T>;

	template<typename K>
	bool isPerHalfEdgeCustomComponentOfType(const std::string& name) const
		requires hedge::HasCustomComponents<T>;

	std::type_index perHalfEdgeCustomComponentType(const std::string& name) const
		requires hedge::HasCustomComponents<T>;

	template<typename K>
	std::vector<std::string> perHalfEdgeCustomComponentNamesOfType() const
		requires hedge::HasCustomComponents<T>;

	template<typename K>
	void addPerHalfEdgeCustomComponent(const std::string& name)
		requires hedge::HasCustomComponents<T>;

	void deletePerHalfEdgeCustomComponent(const std::string& name)
		requires hedge::HasCustomComponents<T>;

	template<typename K>
	CustomComponentVectorHandle<K>
	perHalfEdgeCustomComponentVectorHandle(const std::string& name)
		requires hedge::HasCustomComponents<T>;

	template<typename K>
	ConstCustomComponentVectorHandle<K> perHalfEdgeCustomComponentVectorHandle(
		const std::string& name) const requires hedge::HasCustomComponents<T>;
};

} // namespace vcl::mesh

#include "half_edge_container.cpp"

#endif // VCL_MESH_CONTAINER_HALF_EDGE_CONTAINER_H
