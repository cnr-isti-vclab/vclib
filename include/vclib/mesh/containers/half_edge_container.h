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
class HalfEdgeContainer : protected ElementContainer<T>
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

	// Scalar
	bool isPerHalfEdgeScalarEnabled() const requires hedge::HasOptionalScalar<T>;
	void enablePerHalfEdgeScalar() requires hedge::HasOptionalScalar<T>;
	void disablePerHalfEdgeScalar() requires hedge::HasOptionalScalar<T>;

	// Custom Components
	bool hasPerHalfEdgeCustomComponent(
		const std::string& name) const requires hedge::HasCustomComponents<T>;

	std::vector<std::string> getAllPerHalfEdgeCustomComponentNames()
		const requires hedge::HasCustomComponents<T>;

	template<typename K>
	bool isPerHalfEdgeCustomComponentOfType(
		const std::string& name) const requires hedge::HasCustomComponents<T>;

	template<typename K>
	std::vector<std::string> getPerHalfEdgeCustomComponentNamesOfType()
		const requires hedge::HasCustomComponents<T>;

	template<typename K>
	void addPerHalfEdgeCustomComponent(const std::string& name)
		requires hedge::HasCustomComponents<T>;

	void deletePerHalfEdgeCustomComponent(
		const std::string& name) requires hedge::HasCustomComponents<T>;

	template<typename K>
	CustomComponentVectorHandle<K> getPerHalfEdgeCustomComponentVectorHandle(
		const std::string& name) requires hedge::HasCustomComponents<T>;

	template<typename K>
	ConstCustomComponentVectorHandle<K> getPerHalfEdgeCustomComponentVectorHandle(
		const std::string& name) const requires hedge::HasCustomComponents<T>;

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

	template<typename Mesh, typename Vertex>
	void importVertexReferencesFrom(const Mesh& m, Vertex* base);

	template<typename Mesh, typename Face>
	void importFaceReferencesFrom(const Mesh& m, Face* base);

	template<typename Mesh>
	void importHalfEdgeReferencesFrom(const Mesh& m, T* base);
};

} // namespace vcl::mesh

#include "half_edge_container.cpp"

#endif // VCL_MESH_CONTAINER_HALF_EDGE_CONTAINER_H
