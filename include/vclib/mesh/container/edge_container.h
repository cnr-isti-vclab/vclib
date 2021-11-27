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

#include <vclib/mesh/elements/edge.h>

#include "../iterators/container_iterator.h"
#include "../iterators/container_range_iterator.h"
#include "element_container.h"

namespace vcl::mesh {

class EdgeContainerTriggerer
{
};

template<typename T>
class EdgeContainer : protected ElementContainer<T>, public EdgeContainerTriggerer
{
	using Base              = ElementContainer<T>;
	using EdgeContainerType = EdgeContainer<T>;
public:
	using EdgeType          = T;
	using EdgeIterator      = ContainerIterator<std::vector, T>;
	using ConstEdgeIterator = ConstContainerIterator<std::vector, T>;
	using EdgeRangeIterator = ContainerRangeIterator<EdgeContainerType, EdgeIterator>;
	using ConstEdgeRangeIterator =
		ConstContainerRangeIterator<EdgeContainerType, ConstEdgeIterator>;

	EdgeContainer();

	const EdgeType& edge(uint i) const;
	EdgeType& edge(uint i);

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

protected:
	/**
	 * @brief en: the number of edges in the container. Could be different from the container size
	 * due to edges marked as deleted into the container.
	 */
	uint en = 0;

	uint index(const EdgeType* e) const;

	void clearEdges();

	uint addEdge();
	uint addEdges(uint nEdges);
	void reserveEdges(uint size);

	void setContainerPointer(EdgeType& e);

	std::vector<int> compactEdges();

	void updateAfterAllocation(const T* oldBase, const T* newBase);

	void updateContainerPointers();

	void updateEdgeReferences(const T* oldBase, const T* newBase);

	void updateEdgeReferencesAfterCompact(const T* base, const std::vector<int>& newIndices);

	template<typename Vertex>
	void updateVertexReferences(const Vertex* oldBase, const Vertex* newBase);

	template<typename Vertex>
	void updateVertexReferencesAfterCompact(const Vertex* base, const std::vector<int>& newIndices);

	template<typename Face>
	void updateFaceReferences(const Face* oldBase, const Face* newBase);

	template<typename Face>
	void updateFaceReferencesAfterCompact(const Face* base, const std::vector<int>& newIndices);
};

} // namespace vcl::mesh

#include "edge_container.cpp"

#endif // VCL_MESH_CONTAINER_EDGE_CONTAINER_H
