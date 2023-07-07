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

#include "adjacent_vertices.h"

namespace vcl::comp {

/**
 * @private
 * @brief Initializes the container of adjacent vertices to nullptr.
 *
 * It is made in the init function since the component could be not available
 * during construction (e.g. if the component is optional and not enabled).
 *
 * This member function is hidden by the element that inherits this class.
 */
template<typename Vertex, typename El, bool O>
void AdjacentVertices<Vertex, El, O>::init()
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
template<typename Vertex, typename El, bool O>
bool AdjacentVertices<Vertex, El, O>::isEnabled() const
{
	return Base::isEnabled(this);
}

/**
 * @brief Returns the number of adjacent vertices of the element.
 * @return The number of adjacent vertices of the element.
 */
template<typename Vertex, typename El, bool O>
uint AdjacentVertices<Vertex, El, O>::adjVerticesNumber() const
{
	return Base::container(this).size();
}

/**
 * @brief Returns a reference of the pointer to the i-th adjacent vertex of an
 * element.
 *
 * You can use this function to set the i-th adjacent vertex:
 *
 * @code{.cpp}
 * e.adjVertex(2) = &m.vertex(k); // the second adj vertex of e will point to
 *                                // the k-th vertex of the mesh.
 * @endcode
 *
 * @param i: the position of the required adjacent list in the container.
 * @return The pointer i-th adjacent vertex of the element.
 */
template<typename Vertex, typename El, bool O>
Vertex*& AdjacentVertices<Vertex, El, O>::adjVertex(uint i)
{
	return Base::container(this).at(i);
}

/**
 * @brief Returns a const pointer to the i-th adjacent vertex of this element.
 * @param[in] i: the position of the required adjacent vertex in the container;
 * the value must be between 0 and the number of adj vertices.
 * @return The pointer to the i-th adjacent vertex of this element.
 */
template<typename Vertex, typename El, bool O>
const Vertex* AdjacentVertices<Vertex, El, O>::adjVertex(uint i) const
{
	return Base::container(this).at(i);
}

/**
 * @brief Returns a reference of the pointer to the i-th adjacent vertex of this
 * element but using as index the module between i and the number of adjacent
 * vertices. You can use this function if you need to get the "next adjacent
 * vertex after position k", without check if it is less than the number of adj
 * faces. Works also for negative numbers:
 *
 * @code{.cpp}
 * k = pos; // some position of an adjacent vertex
 * auto* next = e.adjVertexMod(k+1); // the adj vertex next to k, that may also
 *                                   // be at pos 0
 * auto* last = e.adjVertexMod(-1); // the adj vertex in position
 *                                  // adjVertexNumber()-1
 * @endcode
 *
 * @param[in] i: the position of the required adjacent vertex in the container,
 * w.r.t. the position 0; value is modularized on adjVertexNumber().
 * @return The pointer to the required adjacent vertex of this element.
 */
template<typename Vertex, typename El, bool O>
Vertex*& AdjacentVertices<Vertex, El, O>::adjVertexMod(int i)
{
	return Base::container(this).atMod(i);
}

/**
 * @brief Same of adjVertexMod, but returns a const Pointer to the adjacent
 * vertex.
 * @param[in] i: the position of the required adjacent vertex in the container,
 * w.r.t. the position 0; value is modularized on adjVertexNumber().
 * @return The pointer to the required adjacent vertex of this element.
 */
template<typename Vertex, typename El, bool O>
const Vertex* AdjacentVertices<Vertex, El, O>::adjVertexMod(int i) const
{
	return Base::container(this).atMod(i);
}

/**
 * @brief Sets the i-th adjacent vertex of this element.
 * @param[in] e: The pointer to the adjacent vertex to set to this element.
 * @param[in] i: the position in the container on which set the adj vertex; the
 * value must be between 0 and the number of adj vertices.
 */
template<typename Vertex, typename El, bool O>
void AdjacentVertices<Vertex, El, O>::setAdjVertex(Vertex* v, uint i)
{
	Base::container(this).set(v, i);
}

/**
 * @brief Sets all the adjacent vertices of this element.
 *
 * If the size of the container is static, the size of the input vector must be
 * the same one of the container.
 *
 * @param[in] list: vector of adjacent vertices to set.
 */
template<typename Vertex, typename El, bool O>
void AdjacentVertices<Vertex, El, O>::setAdjVertices(
	const std::vector<Vertex*>& list)
{
	Base::container(this).set(list);
}

/**
 * @brief Returns `true` if the container of adjacent vertices contains the
 * given vertex, `false` otherwise.
 *
 * @param[in] e: the pointer to the vertex to search.
 * @return `true` if the container of adjacent vertices contains the given
 * vertex, `false` otherwise.
 */
template<typename Vertex, typename El, bool O>
bool AdjacentVertices<Vertex, El, O>::containsAdjVertex(const Vertex* v) const
{
	return Base::container(this).contains(v);
}

/**
 * @brief Returns an iterator to the first adjacent vertex in the container of
 * this component that is equal to the given vertex. If no such adjacent vertex
 * is found, past-the-end iterator is returned.
 *
 * @param[in] e: the pointer to the vertex to search.
 * @return an iterator pointing to the first adjacent vertex equal to the given
 * edge, or end if no such adjacent vertex is found.
 */
template<typename Vertex, typename El, bool O>
typename AdjacentVertices<Vertex, El, O>::AdjacentVertexIterator
AdjacentVertices<Vertex, El, O>::findAdjVertex(const Vertex* v)
{
	return Base::container(this).find(v);
}

/**
 * @brief Returns a const iterator to the first adjacent vertex in the container
 * of this component that is equal to the given vertex. If no such adjacent
 * vertex is found, past-the-end iterator is returned.
 *
 * @param[in] e: the pointer to the vertex to search.
 * @return a const iterator pointing to the first adjacent vertex equal to the
 * given vertex, or end if no such adjacent vertex is found.
 */
template<typename Vertex, typename El, bool O>
typename AdjacentVertices<Vertex, El, O>::ConstAdjacentVertexIterator
AdjacentVertices<Vertex, El, O>::findAdjVertex(const Vertex* v) const
{
	return Base::container(this).find(v);
}

/**
 * @brief Returns the index of the given adjacent vertex in the container of
 * this element. If the given adjacent vertex is not in the container, returns
 * UINT_NULL.
 *
 * @param[in] e: the pointer to the adjacent vertex to search.
 * @return the index of the given adjacent vertex, or UINT_NULL if it is not
 * found.
 */
template<typename Vertex, typename El, bool O>
uint AdjacentVertices<Vertex, El, O>::indexOfAdjVertex(const Vertex* v) const
{
	return Base::container(this).indexOf(v);
}

/**
 * @brief Resize the container of the adjacent vertices to the given size.
 * @note This function is available only if the container of the Adjacent
 * Vertices is has dynamic size.
 * @param[in] n: The new size of the adjacent vertices container.
 */
template<typename Vertex, typename El, bool O>
void AdjacentVertices<Vertex, El, O>::resizeAdjVertices(uint n)
{
	Base::container(this).resize(n);
}

/**
 * @brief Pushes in the back of the container the given adjacent vertex.
 * @note This function is available only if the container of the Adjacent
 * Vertices component has dynamic size.
 * @param[in] e: The pointer to the adjacent vertex to push in the back of the
 * container.
 */
template<typename Vertex, typename El, bool O>
void AdjacentVertices<Vertex, El, O>::pushAdjVertex(Vertex* v)
{
	Base::container(this).pushBack(v);
}

/**
 * @brief Inserts the given adjacent vertex in the container at the given
 * position.
 * @note This function is available only if the container of the Adjacent
 * Vertices component has dynamic size.
 * @param[in] i: The position in the container where to insert the adjacent
 * vertex.
 * @param[in] e: The pointer to the adjacent vertex to insert in the container.
 */
template<typename Vertex, typename El, bool O>
void AdjacentVertices<Vertex, El, O>::insertAdjVertex(uint i, Vertex* v)
{
	Base::container(this).insert(i, v);
}

/**
 * @brief Removes the adjacent vertex at the given position from the container.
 * @note This function is available only if the container of the Adjacent
 * Vertices component has dynamic size.
 * @param[in] i: The position of the adjacent vertex to remove from the
 * container.
 */
template<typename Vertex, typename El, bool O>
void AdjacentVertices<Vertex, El, O>::eraseAdjVertex(uint i)
{
	Base::container(this).erase(i);
}

/**
 * @brief Clears the container of adjacent vertices, making it empty.
 * @note This function is available only if the container of the Adjacent
 * Vertices component has dynamic size.
 */
template<typename Vertex, typename El, bool O>
void AdjacentVertices<Vertex, El, O>::clearAdjVertices()
{
	Base::container(this).clear();
}

/**
 * @brief Returns an iterator to the first adjacent vertex in the container of
 * this component.
 *
 * @return an iterator pointing to the begin of this container.
 */
template<typename Vertex, typename El, bool O>
typename AdjacentVertices<Vertex, El, O>::AdjacentVertexIterator
AdjacentVertices<Vertex, El, O>::adjVertexBegin()
{
	return Base::container(this).begin();
}

/**
 * @brief Returns an iterator to the end of the container of this component.
 *
 * @return an iterator pointing to the end of this container.
 */
template<typename Vertex, typename El, bool O>
typename AdjacentVertices<Vertex, El, O>::AdjacentVertexIterator
AdjacentVertices<Vertex, El, O>::adjVertexEnd()
{
	return Base::container(this).end();
}

/**
 * @brief Returns a const iterator to the first adjacent vertex in the container
 * of this component.
 *
 * @return an iterator pointing to the begin of this container.
 */
template<typename Vertex, typename El, bool O>
typename AdjacentVertices<Vertex, El, O>::ConstAdjacentVertexIterator
AdjacentVertices<Vertex, El, O>::adjVertexBegin() const
{
	return Base::container(this).begin();
}

/**
 * @brief Returns a const iterator to the end of the container of this
 * component.
 *
 * @return an iterator pointing to the end of this container.
 */
template<typename Vertex, typename El, bool O>
typename AdjacentVertices<Vertex, El, O>::ConstAdjacentVertexIterator
AdjacentVertices<Vertex, El, O>::adjVertexEnd() const
{
	return Base::container(this).end();
}

/**
 * @brief Returns a lightweight view object that stores the begin and end
 * iterators of the container of adjacent vertices of the element. The view
 * object exposes the iterators trough the `begin()` and `end()` member
 * functions, and therefore the returned object can be used in range-based for
 * loops:
 *
 * @code{.cpp}
 * for (auto* adjVertex : el.adjVertices()) {
 *     // Do something with adjVertex
 * }
 * @endcode
 *
 * @return a lightweight view object that can be used in range-based for loops
 * to iterate over adjacent vertices.
 */
template<typename Vertex, typename El, bool O>
auto AdjacentVertices<Vertex, El, O>::adjVertices()
{
	return View(adjVertexBegin(), adjVertexEnd());
}

/**
 * @brief Returns a lightweight const view object that stores the begin and end
 * iterators of the container of adjacent vertices of the element. The view
 * object exposes the iterators trough the `begin()` and `end()` member
 * functions, and therefore the returned object can be used in range-based for
 * loops:
 *
 * @code{.cpp}
 * for (const auto* adjEdge : el.adjVertices()) {
 *     // Do something read-only with adjVertex
 * }
 * @endcode
 *
 * @return a lightweight view object that can be used in range-based for loops
 * to iterate over adjacent edges.
 */
template<typename Vertex, typename El, bool O>
auto AdjacentVertices<Vertex, El, O>::adjVertices() const
{
	return View(adjVertexBegin(), adjVertexEnd());
}

template<typename Vertex, typename El, bool O>
template<typename Element>
void AdjacentVertices<Vertex, El, O>::importFrom(const Element&)
{
}

template<typename Vertex, typename El, bool O>
template<typename Element, typename ElVType>
void AdjacentVertices<Vertex, El, O>::importPointersFrom(
	const Element& e,
	Vertex*        base,
	const ElVType* ebase)
{
	if constexpr (HasAdjacentVertices<Element>) {
		if (isAdjacentVerticesEnabledOn(e)) {
			// from static/dynamic to dynamic size: need to resize first, then
			// import
			resizeAdjVertices(e.adjVerticesNumber());
			importPtrsFrom(e, base, ebase);
		}
	}
}

template<typename Vertex, typename El, bool O>
void AdjacentVertices<Vertex, El, O>::updatePointers(
	const Vertex* oldBase,
	const Vertex* newBase)
{
	Base::updateElementPointers(oldBase, newBase, this);
}

template<typename Vertex, typename El, bool O>
void AdjacentVertices<Vertex, El, O>::updatePointers(
	const Vertex*           base,
	const std::vector<uint>& newIndices)
{
	Base::updateElementPointers(base, newIndices, this);
}

template<typename Vertex, typename El, bool O>
template<typename Element, typename ElVType>
void AdjacentVertices<Vertex, El, O>::importPtrsFrom(
	const Element& e,
	Vertex*        base,
	const ElVType* ebase)
{
	if (ebase != nullptr && base != nullptr) {
		for (uint i = 0; i < e.adjVerticesNumber(); ++i) {
			if (e.adjVertex(i) != nullptr) {
				adjVertex(i) = base + (e.adjVertex(i) - ebase);
			}
		}
	}
}

template <typename T>
bool isAdjacentVerticesEnabledOn(const T& element)
{
	return isComponentEnabledOn<ADJACENT_VERTICES>(element);
}

} // namespace vcl::comp
