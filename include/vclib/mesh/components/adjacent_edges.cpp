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

#include "adjacent_edges.h"

namespace vcl::comp {

/**
 * @private
 * @brief Initializes the container of adjacent edges to nullptr.
 *
 * It is made in the init function since the component could be not available
 * during construction (e.g. if the component is optional and not enabled).
 *
 * This member function is hidden by the element that inherits this class.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::init()
{
	Base::init(this);
}

/**
 * @private
 * @brief Returns `true` if the component is enabled, `false` otherwise.
 * This member function can return `false` only if the component is optional.
 *
 * This member function is hidden by the element that inherits this class.
 *
 * @return `true` if the component is enabled, `false` otherwise.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
bool AdjacentEdges<Edge, N, TT, El, O>::isEnabled() const
{
	return Base::isEnabled(this);
}

/**
 * @brief Returns `true` if the component is enabled, `false` otherwise.
 * This member function can return `false` only if the component is optional.
 *
 * @return `true` if the component is enabled, `false` otherwise.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
bool AdjacentEdges<Edge, N, TT, El, O>::isAdjEdgesEnabled() const
{
	return isEnabled();
}

/**
 * @brief Returns the number of adjacent edges of this element.
 * @return The number of adjacent edges of this element.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
uint AdjacentEdges<Edge, N, TT, El, O>::adjEdgesNumber() const
{
	return Base::container(this).size();
}

/**
 * @brief Returns a reference of the pointer to the i-th adjacent edge of this
 * element.
 *
 * You can use this function to set the i-th adjacent edge:
 *
 * @code{.cpp}
 * e.adjEdge(2) = &m.edge(k); // the second adj edge of e will point to the k-th
 *                            // edge of the mesh.
 * @endcode
 *
 * @param[in] i: the position of the required adjacent edge in the container;
 * the value must be between 0 and the number of adj edges.
 * @return The pointer to the i-th adjacent edge of this element.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
Edge*& AdjacentEdges<Edge, N, TT, El, O>::adjEdge(uint i)
{
	return Base::container(this).at(i);
}

/**
 * @brief Returns a const pointer to the i-th adjacent edge of this element.
 * @param[in] i: the position of the required adjacent edge in the container;
 * the value must be between 0 and the number of adj edges.
 * @return The pointer to the i-th adjacent edge of this element.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
const Edge* AdjacentEdges<Edge, N, TT, El, O>::adjEdge(uint i) const
{
	return Base::container(this).at(i);
}

/**
 * @brief Returns a reference of the pointer to the i-th adjacent edge of this
 * element but using as index the module between i and the number of adjacent
 * edges. You can use this function if you need to get the "next adjacent edge
 * after position k", without check if it is less than the number of adj edges.
 * Works also for negative numbers:
 *
 * @code{.cpp}
 * k = pos; // some position of an adjacent edge
 * auto* next = e.adjEdgeMod(k+1); // the adj edge next to k, that may also be
 *                                 // at pos 0
 * auto* last = e.adjEdgeMod(-1); // the adj edge in position
 *                                // adjEdgeNumber()-1
 * @endcode
 *
 * @param[in] i: the position of the required adjacent edge in the container,
 * w.r.t. the position 0; value is modularized on adjEdgeNumber().
 * @return The pointer to the required adjacent edge of this element.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
Edge*& AdjacentEdges<Edge, N, TT, El, O>::adjEdgeMod(int i)
{
	return Base::container(this).atMod(i);
}

/**
 * @brief Same of adjEdgeMod, but returns a const Pointer to the adjacent edge.
 * @param[in] i: the position of the required adjacent edge in the container,
 * w.r.t. the position 0; value is modularized on adjEdgeNumber().
 * @return The pointer to the required adjacent edge of this element.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
const Edge* AdjacentEdges<Edge, N, TT, El, O>::adjEdgeMod(int i) const
{
	return Base::container(this).atMod(i);
}

/**
 * @brief Sets the i-th adjacent edge of this element.
 * @param[in] e: The pointer to the adjacent edge to set to this element.
 * @param[in] i: the position in the container on which set the adj edge; the
 * value must be between 0 and the number of adj edges.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::setAdjEdge(Edge* e, uint i)
{
	Base::container(this).set(e, i);
}

/**
 * @brief Sets all the adjacent edges of this element.
 *
 * If the size of the container is static, the size of the input vector must be
 * the same one of the container.
 *
 * @param[in] list: vector of adjacent edges to set.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::setAdjEdges(
	const std::vector<Edge*>& list)
{
	Base::container(this).set(list);
}

/**
 * @brief Returns `true` if the container of adjacent edges contains the given
 * edge, `false` otherwise.
 *
 * @param[in] e: the pointer to the edge to search.
 * @return `true` if the container of adjacent edges contains the given edge,
 * `false` otherwise.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
bool AdjacentEdges<Edge, N, TT, El, O>::containsAdjEdge(const Edge* e) const
{
	return Base::container(this).contains(e);
}

/**
 * @brief Returns an iterator to the first adjacent edge in the container of
 * this component that is equal to the given edge. If no such adjacent edge is
 * found, past-the-end iterator is returned.
 *
 * @param[in] e: the pointer to the edge to search.
 * @return an iterator pointing to the first adjacent edge equal to the given
 * edge, or end if no such adjacent edge is found.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
typename AdjacentEdges<Edge, N, TT, El, O>::AdjacentEdgeIterator
AdjacentEdges<Edge, N, TT, El, O>::findAdjEdge(const Edge* e)
{
	return Base::container(this).find(e);
}

/**
 * @brief Returns a const iterator to the first adjacent edge in the container
 * of this component that is equal to the given edge. If no such adjacent edge
 * is found, past-the-end iterator is returned.
 *
 * @param[in] e: the pointer to the edge to search.
 * @return a const iterator pointing to the first adjacent edge equal to the
 * given edge, or end if no such adjacent edge is found.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
typename AdjacentEdges<Edge, N, TT, El, O>::ConstAdjacentEdgeIterator
AdjacentEdges<Edge, N, TT, El, O>::findAdjEdge(const Edge* e) const
{
	return Base::container(this).find(e);
}

/**
 * @brief Returns the index of the given adjacent edge in the container of this
 * element. If the given adjacent edge is not in the container, returns
 * UINT_NULL.
 *
 * @param[in] e: the pointer to the adjacent edge to search.
 * @return the index of the given adjacent edge, or UINT_NULL if it is not
 * found.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
uint AdjacentEdges<Edge, N, TT, El, O>::indexOfAdjEdge(const Edge* e) const
{
	return Base::container(this).indexOf(e);
}

/**
 * @brief Resize the container of the adjacent edges to the given size.
 * @note This function is available only if the container of the Adjacent Edges
 * component has dynamic size.
 * @param[in] n: The new size of the adjacent edges container.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::resizeAdjEdges(uint n)
	requires(N < 0 && !TT)
{
	Base::container(this).resize(n);
}

/**
 * @brief Pushes in the back of the container the given adjacent edge.
 * @note This function is available only if the container of the Adjacent Edges
 * component has dynamic size.
 * @param[in] e: The pointer to the adjacent edge to push in the back of the
 * container.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::pushAdjEdge(Edge* e)
	requires(N < 0 && !TT)
{
	Base::container(this).pushBack(e);
}

/**
 * @brief Inserts the given adjacent edge in the container at the given
 * position.
 * @note This function is available only if the container of the Adjacent Edges
 * component has dynamic size.
 * @param[in] i: The position in the container where to insert the adjacent
 * edge.
 * @param[in] e: The pointer to the adjacent edge to insert in the container.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::insertAdjEdge(uint i, Edge* e)
	requires(N < 0 && !TT)
{
	Base::container(this).insert(i, e);
}

/**
 * @brief Removes the adjacent edge at the given position from the container.
 * @note This function is available only if the container of the Adjacent Edges
 * component has dynamic size.
 * @param[in] i: The position of the adjacent edge to remove from the
 * container.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::eraseAdjEdge(uint i)
	requires(N < 0 && !TT)
{
	Base::container(this).erase(i);
}

/**
 * @brief Clears the container of adjacent edges, making it empty.
 * @note This function is available only if the container of the Adjacent Edges
 * component has dynamic size.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::clearAdjEdges()
	requires(N < 0 && !TT)
{
	Base::container(this).clear();
}

/**
 * @brief Returns an iterator to the first adjacent edge in the container of
 * this component.
 *
 * @return an iterator pointing to the begin of this container.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
typename AdjacentEdges<Edge, N, TT, El, O>::AdjacentEdgeIterator
AdjacentEdges<Edge, N, TT, El, O>::adjEdgeBegin()
{
	return Base::container(this).begin();
}

/**
 * @brief Returns an iterator to the end of the container of this component.
 *
 * @return an iterator pointing to the end of this container.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
typename AdjacentEdges<Edge, N, TT, El, O>::AdjacentEdgeIterator
AdjacentEdges<Edge, N, TT, El, O>::adjEdgeEnd()
{
	return Base::container(this).end();
}

/**
 * @brief Returns a const iterator to the first adjacent edge in the container
 * of this component.
 *
 * @return an iterator pointing to the begin of this container.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
typename AdjacentEdges<Edge, N, TT, El, O>::ConstAdjacentEdgeIterator
AdjacentEdges<Edge, N, TT, El, O>::adjEdgeBegin() const
{
	return Base::container(this).begin();
}

/**
 * @brief Returns a const iterator to the end of the container of this
 * component.
 *
 * @return an iterator pointing to the end of this container.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
typename AdjacentEdges<Edge, N, TT, El, O>::ConstAdjacentEdgeIterator
AdjacentEdges<Edge, N, TT, El, O>::adjEdgeEnd() const
{
	return Base::container(this).end();
}

/**
 * @brief Returns a lightweight view object that stores the begin and end
 * iterators of the container of adjacent edges of the element. The view object
 * exposes the iterators trough the `begin()` and `end()` member functions, and
 * therefore the returned object can be used in range-based for loops:
 *
 * @code{.cpp}
 * for (auto* adjEdge : el.adjEdges()) {
 *     // Do something with adjEdge
 * }
 * @endcode
 *
 * @return a lightweight view object that can be used in range-based for loops
 * to iterate over adjacent edges.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
auto AdjacentEdges<Edge, N, TT, El, O>::adjEdges()
{
	return View(adjEdgeBegin(), adjEdgeEnd());
}

/**
 * @brief Returns a lightweight const view object that stores the begin and end
 * iterators of the container of adjacent edges of the element. The view object
 * exposes the iterators trough the `begin()` and `end()` member functions, and
 * therefore the returned object can be used in range-based for loops:
 *
 * @code{.cpp}
 * for (const auto* adjEdge : el.adjEdges()) {
 *     // Do something read-only with adjEdge
 * }
 * @endcode
 *
 * @return a lightweight view object that can be used in range-based for loops
 * to iterate over adjacent edges.
 */
template<typename Edge, int N, bool TT, typename El, bool O>
auto AdjacentEdges<Edge, N, TT, El, O>::adjEdges() const
{
	return View(adjEdgeBegin(), adjEdgeEnd());
}

template<typename Edge, int N, bool TT, typename El, bool O>
template<typename Element>
void AdjacentEdges<Edge, N, TT, El, O>::importFrom(const Element&)
{
}

template<typename Edge, int N, bool TT, typename El, bool O>
template<typename Element, typename ElEType>
void AdjacentEdges<Edge, N, TT, El, O>::importPointersFrom(
	const Element& e,
	Edge*          base,
	const ElEType* ebase)
{
	if constexpr (HasAdjacentEdges<Element>) {
		if (isAdjacentEdgesEnabledOn(e)) {
			if constexpr (N > 0) {
				// same static size
				if constexpr (N == Element::ADJ_EDGE_NUMBER) {
					importPtrsFrom(e, base, ebase);
				}
				// from dynamic to static, but dynamic size == static size
				else if constexpr (Element::ADJ_EDGE_NUMBER < 0) {
					if (e.adjEdgesNumber() == N) {
						importPtrsFrom(e, base, ebase);
					}
				}
				else {
					// do not import in this case: cannot import from dynamic
					// size != static size
				}
			}
			else {
				// from static/dynamic to dynamic size: need to resize first,
				// then import
				resize(e.adjEdgesNumber());
				importPtrsFrom(e, base, ebase);
			}
		}
	}
}

template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::updatePointers(
	const Edge* oldBase,
	const Edge* newBase)
{
	Base::updateElementPointers(oldBase, newBase, this);
}

template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::updatePointers(
	const Edge*             base,
	const std::vector<uint>& newIndices)
{
	Base::updateElementPointers(base, newIndices, this);
}

template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::resize(uint n) requires (N < 0)
{
	Base::container(this).resize(n);
}

template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::pushBack(Edge* e) requires (N < 0)
{
	Base::container(this).pushBack(e);
}

template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::insert(uint i, Edge* e) requires (N < 0)
{
	Base::container(this).insert(i, e);
}

template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::erase(uint i) requires (N < 0)
{
	Base::container(this).erase(i);
}

template<typename Edge, int N, bool TT, typename El, bool O>
void AdjacentEdges<Edge, N, TT, El, O>::clear() requires (N < 0)
{
	Base::container(this).clear();
}

template<typename Edge, int N, bool TT, typename El, bool O>
template<typename Element, typename ElEType>
void AdjacentEdges<Edge, N, TT, El, O>::importPtrsFrom(
	const Element& e,
	Edge*          base,
	const ElEType* ebase)
{
	if (ebase != nullptr && base != nullptr) {
		for (uint i = 0; i < e.adjEdgesNumber(); ++i) {
			if (e.adjEdge(i) != nullptr) {
				adjEdge(i) = base + (e.adjEdge(i) - ebase);
			}
		}
	}
}

template <typename T>
bool isAdjacentEdgesEnabledOn(const T& element)
{
	return isComponentEnabledOn<ADJACENT_EDGES>(element);
}

} // namespace vcl::comp
