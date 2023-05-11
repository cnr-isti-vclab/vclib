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

template<typename Vertex, typename El, bool O>
void AdjacentVertices<Vertex, El, O>::init()
{
	Base::init(this);
}

template<typename Vertex, typename El, bool O>
bool AdjacentVertices<Vertex, El, O>::isEnabled() const
{
	return Base::isEnabled(this);
}

template<typename Vertex, typename El, bool O>
bool AdjacentVertices<Vertex, El, O>::isAdjVerticesEnabled() const
{
	return isEnabled();
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
 * @brief Returns a reference of the pointer to the i-th adjacent vertex of an element.
 *
 * You can use this function to set the i-th adjacent vertex:
 *
 * @code{.cpp}
 * e.adjVertex(2) = &m.vertex(k); // the second adj vertex of e will point to the k-th vertex of the
 * mesh.
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

template<typename Vertex, typename El, bool O>
const Vertex* AdjacentVertices<Vertex, El, O>::adjVertex(uint i) const
{
	return Base::container(this).at(i);
}

/**
 * @brief Returns a reference of the pointer to the i-th adjacent vertex of this element but using
 * as index the module between i and the number of adjacent vertices.
 *
 * You can use this function if you need to get the "next adjacent vertex after position k", without
 * check if it is less than the number of adj faces. Works also for negative numbers:
 *
 * @code{.cpp}
 * k = pos; // some position of an adjacent vertex
 * auto* next = e.adjVertexMod(k+1); // the adj vertex next to k, that may also be at pos 0
 * auto* last = e.adjVertexMod(-1); // the adj vertex in position adjVertexNumber()-1
 * @endcode
 *
 * @param[in] i: the position of the required adjacent vertex in the container, w.r.t. the position
 * 0; value is modularized on adjVertexNumber().
 * @return The pointer to the required adjacent vertex of this element.
 */
template<typename Vertex, typename El, bool O>
Vertex*& AdjacentVertices<Vertex, El, O>::adjVertexMod(int i)
{
	return Base::container(this).atMod(i);
}

template<typename Vertex, typename El, bool O>
const Vertex* AdjacentVertices<Vertex, El, O>::adjVertexMod(int i) const
{
	return Base::container(this).atMod(i);
}

template<typename Vertex, typename El, bool O>
void AdjacentVertices<Vertex, El, O>::setAdjVertex(Vertex* v, uint i)
{
	Base::container(this).set(v, i);
}

template<typename Vertex, typename El, bool O>
void AdjacentVertices<Vertex, El, O>::setAdjVertices(const std::vector<Vertex*>& list)
{
	Base::container(this).set(list);
}

template<typename Vertex, typename El, bool O>
bool AdjacentVertices<Vertex, El, O>::containsAdjVertex(const Vertex* v) const
{
	return Base::container(this).contains(v);
}

template<typename Vertex, typename El, bool O>
typename AdjacentVertices<Vertex, El, O>::AdjacentVertexIterator
AdjacentVertices<Vertex, El, O>::findAdjVertex(const Vertex* v)
{
	return Base::container(this).find(v);
}

template<typename Vertex, typename El, bool O>
typename AdjacentVertices<Vertex, El, O>::ConstAdjacentVertexIterator
AdjacentVertices<Vertex, El, O>::findAdjVertex(const Vertex* v) const
{
	return Base::container(this).find(v);
}

template<typename Vertex, typename El, bool O>
int AdjacentVertices<Vertex, El, O>::indexOfAdjVertex(const Vertex* v) const
{
	return Base::container(this).indexOf(v);
}

/**
 * @brief Resize the container of the adjacent vertices to the given size.
 * @note This function is available only if the container of the Adjacent Vertices is has dynamic
 * size.
 * @param n
 */
template<typename Vertex, typename El, bool O>
void AdjacentVertices<Vertex, El, O>::resizeAdjVertices(uint n)
{
	Base::container(this).resize(n);
}

template<typename Vertex, typename El, bool O>
void AdjacentVertices<Vertex, El, O>::pushAdjVertex(Vertex* v)
{
	Base::container(this).pushBack(v);
}

template<typename Vertex, typename El, bool O>
void AdjacentVertices<Vertex, El, O>::insertAdjVertex(uint i, Vertex* v)
{
	Base::container(this).insert(i, v);
}

template<typename Vertex, typename El, bool O>
void AdjacentVertices<Vertex, El, O>::eraseAdjVertex(uint i)
{
	Base::container(this).erase(i);
}

template<typename Vertex, typename El, bool O>
void AdjacentVertices<Vertex, El, O>::clearAdjVertices()
{
	Base::container(this).clear();
}

template<typename Vertex, typename El, bool O>
typename AdjacentVertices<Vertex, El, O>::AdjacentVertexIterator AdjacentVertices<Vertex, El, O>::adjVertexBegin()
{
	return Base::container(this).begin();
}

template<typename Vertex, typename El, bool O>
typename AdjacentVertices<Vertex, El, O>::AdjacentVertexIterator AdjacentVertices<Vertex, El, O>::adjVertexEnd()
{
	return Base::container(this).end();
}

template<typename Vertex, typename El, bool O>
typename AdjacentVertices<Vertex, El, O>::ConstAdjacentVertexIterator
AdjacentVertices<Vertex, El, O>::adjVertexBegin() const
{
	return Base::container(this).begin();
}

template<typename Vertex, typename El, bool O>
typename AdjacentVertices<Vertex, El, O>::ConstAdjacentVertexIterator
AdjacentVertices<Vertex, El, O>::adjVertexEnd() const
{
	return Base::container(this).end();
}

template<typename Vertex, typename El, bool O>
auto AdjacentVertices<Vertex, El, O>::adjVertices()
{
	return View(adjVertexBegin(), adjVertexEnd());
}

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
			// from static/dynamic to dynamic size: need to resize first, then import
			resizeAdjVertices(e.adjVerticesNumber());
			importPtrsFrom(e, base, ebase);
		}
	}
}

template<typename Vertex, typename El, bool O>
void AdjacentVertices<Vertex, El, O>::updatePointers(const Vertex* oldBase, const Vertex* newBase)
{
	Base::updateElementPointers(oldBase, newBase, this);
}

template<typename Vertex, typename El, bool O>
void AdjacentVertices<Vertex, El, O>::updatePointersAfterCompact(
	const Vertex*           base,
	const std::vector<int>& newIndices)
{
	Base::updateElementPointersAfterCompact(base, newIndices, this);
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
	if constexpr (HasOptionalAdjacentVertices<T>) {
		return element.isAdjVerticesEnabled();
	}
	else {
		return HasAdjacentVertices<T>;
	}
}

} // namespace vcl::comp
