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

#include "adjacent_edges.h"

namespace vcl::comp {

/**
 * @brief Creates a Container of Adjacent Edges, stored as Edge Pointers.
 * If the size of the container is static, all the values will be set to nullptr.
 * If the size is dynamic, the number of values in the container is zero.
 */
template<typename Edge, int N>
AdjacentEdges<Edge, N>::AdjacentEdges() : Base()
{
}

/**
 * @brief Returns the number of adjacent edges of this element.
 * @return The number of adjacent edges of this element.
 */
template<typename Edge, int N>
uint AdjacentEdges<Edge, N>::adjEdgesNumber() const
{
	return Base::size();
}

/**
 * @brief Returns a reference of the pointer to the i-th adjacent edge of this element.
 *
 * You can use this function to set the i-th adjacent edge:
 *
 * @code{.cpp}
 * e.adjEdge(2) = &m.edge(k); // the second adj edge of e will point to the k-th edge of the mesh.
 * @endcode
 *
 * @param[in] i: the position of the required adjacent edge in the container; the value must be
 * between 0 and the number of adj edges.
 * @return The pointer to the i-th adjacent edge of this element.
 */
template<typename Edge, int N>
Edge*& AdjacentEdges<Edge, N>::adjEdge(uint i)
{
	return Base::at(i);
}

/**
 * @brief Returns a const pointer to the i-th adjacent edge of this element.
 * @param[in] i: the position of the required adjacent edge in the container; the value must be
 * between 0 and the number of adj edges.
 * @return The pointer to the i-th adjacent edge of this element.
 */
template<typename Edge, int N>
const Edge* AdjacentEdges<Edge, N>::adjEdge(uint i) const
{
	return Base::at(i);
}

/**
 * @brief Returns a reference of the pointer to the i-th adjacent edge of this element but using as
 * index the module between i and the number of adjacent edges. You can use this function if you
 * need to get the "next adjacent edge after position k", without check if it is less than the
 * number of adj edges. Works also for negative numbers:
 *
 * @code{.cpp}
 * k = pos; // some position of an adjacent edge
 * auto* next = e.adjEdgeMod(k+1); // the adj edge next to k, that may also be at pos 0
 * auto* last = e.adjEdgeMod(-1); // the adj edge in position adjEdgeNumber()-1
 * @endcode
 *
 * @param[in] i: the position of the required adjacent edge in the container, w.r.t. the position 0;
 * value is modularized on adjEdgeNumber().
 * @return The pointer to the required adjacent edge of this element.
 */
template<typename Edge, int N>
Edge*& AdjacentEdges<Edge, N>::adjEdgeMod(int i)
{
	return Base::atMod(i);
}

/**
 * @brief Same of adjEdgeMod, but returns a const Pointer to the adjacent edge.
 * @param[in] i: the position of the required adjacent edge in the container, w.r.t. the position 0;
 * value is modularized on adjEdgeNumber().
 * @return The pointer to the required adjacent edge of this element.
 */
template<typename Edge, int N>
const Edge* AdjacentEdges<Edge, N>::adjEdgeMod(int i) const
{
	return Base::atMod(i);
}

/**
 * @brief Sets the i-th adjacent edge of this element.
 * @param[in] e: The pointer to the adjacent edge to set to this element.
 * @param[in] i: the position in the container on which set the adj edge; the value must be
 * between 0 and the number of adj edges.
 */
template<typename Edge, int N>
void AdjacentEdges<Edge, N>::setAdjEdge(Edge* e, uint i)
{
	Base::set(e, i);
}

/**
 * @brief Sets all the adjacent edges of this element.
 *
 * If the size of the container is static, the size of the input vector must be the same one of the
 * container.
 *
 * @param[in] list: vector of adjacent edges to set.
 */
template<typename Edge, int N>
void AdjacentEdges<Edge, N>::setAdjEdges(const std::vector<Edge*>& list)
{
	Base::set(list);
}

template<typename Edge, int N>
bool AdjacentEdges<Edge, N>::containsAdjEdge(const Edge* e) const
{
	return Base::contains(e);
}

template<typename Edge, int N>
typename AdjacentEdges<Edge, N>::AdjacentEdgeIterator
AdjacentEdges<Edge, N>::findAdjEdge(const Edge* e)
{
	return Base::find(e);
}

template<typename Edge, int N>
typename AdjacentEdges<Edge, N>::ConstAdjacentEdgeIterator
AdjacentEdges<Edge, N>::findAdjEdge(const Edge* e) const
{
	return Base::find(e);
}

template<typename Edge, int N>
int AdjacentEdges<Edge, N>::indexOfAdjEdge(const Edge* e) const
{
	return Base::indexOf(e);
}

/**
 * @brief Resize the container of the adjacent edges to the given size.
 * @note This function is available only if the container of the Adjacent Edges is has dynamic
 * size.
 * @param n
 */
template<typename Edge, int N>
template<int M>
VCL_ENABLE_IF(M < 0, void)
	AdjacentEdges<Edge, N>::resizeAdjEdges(uint n)
{
	Base::resize(n);
}

template<typename Edge, int N>
template<int M>
VCL_ENABLE_IF(M < 0, void)
	AdjacentEdges<Edge, N>::pushAdjEdge(Edge* e)
{
	Base::pushBack(e);
}

template<typename Edge, int N>
template<int M>
VCL_ENABLE_IF(M < 0, void)
	AdjacentEdges<Edge, N>::insertAdjEdge(uint i, Edge* e)
{
	Base::insert(i, e);
}

template<typename Edge, int N>
template<int M>
VCL_ENABLE_IF(M < 0, void)
	AdjacentEdges<Edge, N>::eraseAdjEdge(uint i)
{
	Base::erase(i);
}

template<typename Edge, int N>
template<int M>
VCL_ENABLE_IF(M < 0, void)
	AdjacentEdges<Edge, N>::clearAdjEdges()
{
	Base::clear();
}

template<typename Edge, int N>
typename AdjacentEdges<Edge, N>::AdjacentEdgeIterator AdjacentEdges<Edge, N>::adjEdgeBegin()
{
	return Base::begin();
}

template<typename Edge, int N>
typename AdjacentEdges<Edge, N>::AdjacentEdgeIterator AdjacentEdges<Edge, N>::adjEdgeEnd()
{
	return Base::end();
}

template<typename Edge, int N>
typename AdjacentEdges<Edge, N>::ConstAdjacentEdgeIterator
AdjacentEdges<Edge, N>::adjEdgeBegin() const
{
	return Base::begin();
}

template<typename Edge, int N>
typename AdjacentEdges<Edge, N>::ConstAdjacentEdgeIterator
AdjacentEdges<Edge, N>::adjEdgeEnd() const
{
	return Base::end();
}

template<typename Edge, int N>
typename AdjacentEdges<Edge, N>::AdjacentEdgeRangeIterator AdjacentEdges<Edge, N>::adjEdges()
{
	return Base::rangeIterator();
}

template<typename Edge, int N>
typename AdjacentEdges<Edge, N>::ConstAdjacentEdgeRangeIterator
AdjacentEdges<Edge, N>::adjEdges() const
{
	return Base::rangeIterator();
}

template<typename Edge, int N>
void AdjacentEdges<Edge, N>::updateEdgeReferences(const Edge* oldBase, const Edge* newBase)
{
	Base::updateElementReferences(oldBase, newBase);
}

template<typename Edge, int N>
void AdjacentEdges<Edge, N>::updateEdgeReferencesAfterCompact(
	const Edge*             base,
	const std::vector<int>& newIndices)
{
	Base::updateElementReferencesAfterCompact(base, newIndices);
}

} // namespace vcl::comp
