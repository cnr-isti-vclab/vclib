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

#ifndef VCL_MESH_CONTAINER_HALF_EDGE_CONTAINER_H
#define VCL_MESH_CONTAINER_HALF_EDGE_CONTAINER_H

#include <vclib/iterators/container_iterator.h>
#include <vclib/iterators/container_range_iterator.h>
#include <vclib/mesh/elements/half_edge.h>

#include "../components/vertical/vectors/custom_component_vector_handle.h"
#include "element_container.h"

namespace vcl::mesh {

template<HalfEdgeConcept T>
class HalfEdgeContainer : protected ElementContainer<T>, public HalfEdgeContainerTriggerer
{
	template <HalfEdgeConcept U>
	friend class HalfEdgeContainer;

	using HalfEdgeContainerType = HalfEdgeContainer<T>;
	using Base = ElementContainer<T>;

public:
	using HalfEdge                   = T;
	using HalfEdgeType               = T;
	using HalfEdgeIterator           = typename Base::ElementIterator;
	using ConstHalfEdgeIterator      = typename Base::ConstElementIterator;
	using HalfEdgeRangeIterator      = typename Base::ElementRangeIterator;
	using ConstHalfEdgeRangeIterator = typename Base::ConstElementRangeIterator;

	HalfEdgeContainer();

	const HalfEdgeType& halfEdge(uint i) const;
	HalfEdgeType&       halfEdge(uint i);

	uint halfEdgeNumber() const;
	uint halfEdgeContainerSize() const;
	uint deletedHalfEdgeNumber() const;

	void deleteHalfEdge(uint i);
	void deleteHalfEdge(const HalfEdgeType* e);

	uint             halfEdgeIndexIfCompact(uint id) const;
	std::vector<int> halfEdgeCompactIndices() const;

	HalfEdgeIterator           halfEdgeBegin(bool jumpDeleted = true);
	HalfEdgeIterator           halfEdgeEnd();
	ConstHalfEdgeIterator      halfEdgeBegin(bool jumpDeleted = true) const;
	ConstHalfEdgeIterator      halfEdgeEnd() const;
	HalfEdgeRangeIterator      halfEdges(bool jumpDeleted = true);
	ConstHalfEdgeRangeIterator halfEdges(bool jumpDeleted = true) const;

	// Color
	template<typename U = T>
	VCL_ENABLE_IF(hedge::hasOptionalColor<U>(), bool)
	isPerHalfEdgeColorEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(hedge::hasOptionalColor<U>(), void)
	enablePerHalfEdgeColor();

	template<typename U = T>
	VCL_ENABLE_IF(hedge::hasOptionalColor<U>(), void)
	disablePerHalfEdgeColor();

	// Mark
	template<typename U = T>
	VCL_ENABLE_IF(hedge::hasOptionalMark<U>(), bool)
	isPerHalfEdgeMarkEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(hedge::hasOptionalMark<U>(), void)
	enablePerHalfEdgeMark();

	template<typename U = T>
	VCL_ENABLE_IF(hedge::hasOptionalMark<U>(), void)
	disablePerHalfEdgeMark();

	// Scalar
	template<typename U = T>
	VCL_ENABLE_IF(hedge::hasOptionalScalar<U>(), bool)
	isPerHalfEdgeScalarEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(hedge::hasOptionalScalar<U>(), void)
	enablePerHalfEdgeScalar();

	template<typename U = T>
	VCL_ENABLE_IF(hedge::hasOptionalScalar<U>(), void)
	disablePerHalfEdgeScalar();

	// Custom Components
	template<typename U = T>
	VCL_ENABLE_IF(hedge::hasCustomComponents<U>(), bool)
	hasPerHalfEdgeCustomComponent(const std::string& name) const;

	template<typename U = T>
	VCL_ENABLE_IF(hedge::hasCustomComponents<U>(), std::vector<std::string>)
	getAllPerHalfEdgeCustomComponentNames() const;

	template<typename K, typename U = T>
	VCL_ENABLE_IF(hedge::hasCustomComponents<U>(), bool)
	isPerHalfEdgeCustomComponentOfType(const std::string& name) const;

	template<typename K, typename U = T>
	VCL_ENABLE_IF(hedge::hasCustomComponents<U>(), std::vector<std::string>)
	getPerHalfEdgeCustomComponentNamesOfType() const;

	template<typename K, typename U = T>
	VCL_ENABLE_IF(hedge::hasCustomComponents<U>(), void)
	addPerHalfEdgeCustomComponent(const std::string& name);

	template<typename U = T>
	VCL_ENABLE_IF(hedge::hasCustomComponents<U>(), void)
	deletePerHalfEdgeCustomComponent(const std::string& name);

	template<typename K, typename U = T>
	VCL_ENABLE_IF(hedge::hasCustomComponents<U>(), CustomComponentVectorHandle<K>)
	getPerHalfEdgeCustomComponentVectorHandle(const std::string& name);

	template<typename K, typename U = T>
	VCL_ENABLE_IF(hedge::hasCustomComponents<U>(), ConstCustomComponentVectorHandle<K>)
	getPerHalfEdgeCustomComponentVectorHandle(const std::string& name) const;

protected:
	uint index(const HalfEdgeType* e) const;

	void clearHalfEdges();

	uint addHalfEdge();
	uint addHalfEdges(uint nEdges);
	void reserveHalfEdges(uint size);

	std::vector<int> compactHalfEdges();

	template<typename Mesh>
	void enableOptionalComponentsOf(const Mesh& m);

	template<typename Mesh>
	void importFrom(const Mesh& m);
};

} // namespace vcl::mesh

#include "half_edge_container.cpp"

#endif // VCL_MESH_CONTAINER_HALF_EDGE_CONTAINER_H
