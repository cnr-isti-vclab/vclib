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

#ifndef VCL_MESH_COMPONENTS_ADJACENT_EDGES_H
#define VCL_MESH_COMPONENTS_ADJACENT_EDGES_H

#include <vclib/concepts/mesh/components/adjacent_edges.h>
#include <vclib/views/view.h>

#include "bases/pointers_container_component.h"

namespace vcl::comp {

/**
 * @brief The AdjacentEdges class is a container of Edge pointers. It could be
 * used by any Element to save adjacencies information (also the Edge element
 * itself).
 *
 * It is a random access container having static or dynamic size, depending on
 * the value of N (a negative number means dynamic).
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component.
 *
 * For example, if you have a Vertex Element `v` that has the AdjacentEdges
 * component, you'll be able to access to this component member functions from
 * `v`:
 *
 * @code{.cpp}
 * v.adjEdgesNumber();
 * @endcode
 *
 * @note This component can be *Tied To Vertex Number*: it means that the size
 * of the container, if dynamic, will change automatically along the Vertex
 * Number of the Component. Check the `TTVN` template value on the
 * specialization of your component to check if it is tied to the Vertex Number.
 * For further details check the documentation of the @ref ContainerComponent
 * class.
 *
 * @ingroup components
 */
template<
	typename Edge,
	int  N,
	bool TTVN,
	typename ElementType = void,
	bool OPT             = false>
class AdjacentEdges :
		public PointersContainerComponent<
			ADJACENT_EDGES,
			Edge,
			N,
			ElementType,
			OPT,
			TTVN>
{
	using Base = PointersContainerComponent<
		ADJACENT_EDGES,
		Edge,
		N,
		ElementType,
		OPT,
		TTVN>;

public:
	/**
	 * @brief Expose the type of the Adjacent Edge.
	 */
	using AdjacentEdgeType = Edge;

	/* Iterator Types declaration */

	using AdjacentEdgeIterator      = typename Base::Iterator;
	using ConstAdjacentEdgeIterator = typename Base::ConstIterator;

	/**
	 * @brief Static size of the container. If the container is dynamic, this
	 * value will be negative and you should use the adjEdgesNumber() member
	 * function.
	 */
	static const int ADJ_EDGE_NUMBER = Base::SIZE;

	/* Constructor and isEnabled */

	void init();

	bool isEnabled() const;
	bool isAdjEdgesEnabled() const;

	/* Member functions */

	uint adjEdgesNumber() const;

	Edge*&      adjEdge(uint i);
	const Edge* adjEdge(uint i) const;
	Edge*&      adjEdgeMod(int i);
	const Edge* adjEdgeMod(int i) const;

	void setAdjEdge(Edge* f, uint i);
	void setAdjEdges(const std::vector<Edge*>& list);

	bool containsAdjEdge(const Edge* e) const;

	AdjacentEdgeIterator      findAdjEdge(const Edge* e);
	ConstAdjacentEdgeIterator findAdjEdge(const Edge* e) const;

	uint indexOfAdjEdge(const Edge* e) const;

	/* Member functions specific for vector of adjacent edges */

	void resizeAdjEdges(uint n) requires (N < 0 && !TTVN);
	void pushAdjEdge(Edge* e) requires (N < 0 && !TTVN);
	void insertAdjEdge(uint i, Edge* e) requires (N < 0 && !TTVN);
	void eraseAdjEdge(uint i) requires (N < 0 && !TTVN);
	void clearAdjEdges() requires (N < 0 && !TTVN);

	/* Iterator Member functions */

	AdjacentEdgeIterator      adjEdgeBegin();
	AdjacentEdgeIterator      adjEdgeEnd();
	ConstAdjacentEdgeIterator adjEdgeBegin() const;
	ConstAdjacentEdgeIterator adjEdgeEnd() const;
	auto                      adjEdges();
	auto                      adjEdges() const;

protected:
	// Component interface function
	template <typename Element>
	void importFrom(const Element& e);

	// PointersComponent interface functions
	template<typename Element, typename ElEType>
	void importPointersFrom(const Element& e, Edge* base, const ElEType* ebase);

	void updatePointers(const Edge* oldBase, const Edge* newBase);

	void updatePointers(const Edge* base, const std::vector<uint>& newIndices);

	// ContainerComponent interface functions
	void resize(uint n) requires (N < 0);
	void pushBack(Edge* e = nullptr) requires (N < 0);
	void insert(uint i, Edge* e = nullptr) requires (N < 0);
	void erase(uint i) requires (N < 0);
	void clear() requires (N < 0);

private:
	template<typename Element, typename ElEType>
	void importPtrsFrom(const Element& e, Edge* base, const ElEType* ebase);
};

/* Detector function to check if a class has AdjacentEdges enabled */

template <typename T>
bool isAdjacentEdgesEnabledOn(const T& element);

} // namespace vcl::comp

#include "adjacent_edges.cpp"

#endif // VCL_MESH_COMPONENTS_ADJACENT_EDGES_H
