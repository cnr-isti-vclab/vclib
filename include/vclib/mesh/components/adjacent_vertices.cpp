/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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
 * @brief Creates a Container of Adjacent Vertices, stored as Vertex Pointers.
 * If the size of the container is static, all the values will be set to nullptr.
 * If the size is dynamic, the number of values in the container is zero.
 */
template<typename Vertex, typename El, bool h>
AdjacentVertices<Vertex, El, h>::AdjacentVertices() : Base()
{
}

/**
 * @brief Returns the number of adjacent vertices of the element.
 * @return The number of adjacent vertices of the element.
 */
template<typename Vertex, typename El, bool h>
uint AdjacentVertices<Vertex, El, h>::adjVerticesNumber() const
{
	return Base::container.size();
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
template<typename Vertex, typename El, bool h>
Vertex*& AdjacentVertices<Vertex, El, h>::adjVertex(uint i)
{
	return Base::container.at(i);
}

template<typename Vertex, typename El, bool h>
const Vertex* AdjacentVertices<Vertex, El, h>::adjVertex(uint i) const
{
	return Base::container.at(i);
}

template<typename Vertex, typename El, bool h>
Vertex*& AdjacentVertices<Vertex, El, h>::adjVertexMod(int i)
{
	return Base::container.atMod(i);
}

template<typename Vertex, typename El, bool h>
const Vertex* AdjacentVertices<Vertex, El, h>::adjVertexMod(int i) const
{
	return Base::container.atMod(i);
}

template<typename Vertex, typename El, bool h>
void AdjacentVertices<Vertex, El, h>::setAdjVertex(Vertex* v, uint i)
{
	Base::container.set(v, i);
}

template<typename Vertex, typename El, bool h>
void AdjacentVertices<Vertex, El, h>::setAdjVertices(const std::vector<Vertex*>& list)
{
	Base::container.set(list);
}

template<typename Vertex, typename El, bool h>
bool AdjacentVertices<Vertex, El, h>::containsAdjVertex(const Vertex* v) const
{
	return Base::container.contains(v);
}

template<typename Vertex, typename El, bool h>
typename AdjacentVertices<Vertex, El, h>::AdjacentVertexIterator
AdjacentVertices<Vertex, El, h>::findAdjVertex(const Vertex* v)
{
	return Base::container.find(v);
}

template<typename Vertex, typename El, bool h>
typename AdjacentVertices<Vertex, El, h>::ConstAdjacentVertexIterator
AdjacentVertices<Vertex, El, h>::findAdjVertex(const Vertex* v) const
{
	return Base::container.find(v);
}

template<typename Vertex, typename El, bool h>
int AdjacentVertices<Vertex, El, h>::indexOfAdjVertex(const Vertex* v) const
{
	return Base::container.indexOf(v);
}

/**
 * @brief Resize the container of the adjacent vertices to the given size.
 * @note This function is available only if the container of the Adjacent Vertices is has dynamic
 * size.
 * @param n
 */
template<typename Vertex, typename El, bool h>
void AdjacentVertices<Vertex, El, h>::resizeAdjVertices(uint n)
{
	Base::container.resize(n);
}

template<typename Vertex, typename El, bool h>
void AdjacentVertices<Vertex, El, h>::pushAdjVertex(Vertex* v)
{
	Base::container.pushBack(v);
}

template<typename Vertex, typename El, bool h>
void AdjacentVertices<Vertex, El, h>::insertAdjVertex(uint i, Vertex* v)
{
	Base::container.insert(i, v);
}

template<typename Vertex, typename El, bool h>
void AdjacentVertices<Vertex, El, h>::eraseAdjVertex(uint i)
{
	Base::container.erase(i);
}

template<typename Vertex, typename El, bool h>
void AdjacentVertices<Vertex, El, h>::clearAdjVertices()
{
	Base::container.clear();
}

template<typename Vertex, typename El, bool h>
typename AdjacentVertices<Vertex, El, h>::AdjacentVertexIterator AdjacentVertices<Vertex, El, h>::adjVertexBegin()
{
	return Base::container.begin();
}

template<typename Vertex, typename El, bool h>
typename AdjacentVertices<Vertex, El, h>::AdjacentVertexIterator AdjacentVertices<Vertex, El, h>::adjVertexEnd()
{
	return Base::container.end();
}

template<typename Vertex, typename El, bool h>
typename AdjacentVertices<Vertex, El, h>::ConstAdjacentVertexIterator
AdjacentVertices<Vertex, El, h>::adjVertexBegin() const
{
	return Base::container.begin();
}

template<typename Vertex, typename El, bool h>
typename AdjacentVertices<Vertex, El, h>::ConstAdjacentVertexIterator
AdjacentVertices<Vertex, El, h>::adjVertexEnd() const
{
	return Base::container.end();
}

template<typename Vertex, typename El, bool h>
typename AdjacentVertices<Vertex, El, h>::AdjacentVertexRangeIterator
AdjacentVertices<Vertex, El, h>::adjVertices()
{
	return Base::container.rangeIterator();
}

template<typename Vertex, typename El, bool h>
typename AdjacentVertices<Vertex, El, h>::ConstAdjacentVertexRangeIterator
AdjacentVertices<Vertex, El, h>::adjVertices() const
{
	return Base::container.rangeIterator();
}

template<typename Vertex, typename El, bool h>
void AdjacentVertices<Vertex, El, h>::updateVertexReferences(const Vertex* oldBase, const Vertex* newBase)
{
	Base::updateElementReferences(oldBase, newBase);
}

template<typename Vertex, typename El, bool h>
void AdjacentVertices<Vertex, El, h>::updateVertexReferencesAfterCompact(
	const Vertex*           base,
	const std::vector<int>& newIndices)
{
	Base::updateElementReferencesAfterCompact(base, newIndices);
}

template<typename Vertex, typename El, bool h>
template<typename Element>
void AdjacentVertices<Vertex, El, h>::importFrom(const Element&)
{
}

template<typename Vertex, typename El, bool h>
template<typename Element, typename ElVType>
void AdjacentVertices<Vertex, El, h>::importVertexReferencesFrom(
	const Element& e,
	Vertex*        base,
	const ElVType* ebase)
{
	if constexpr (HasAdjacentVertices<Element>) {
		if (isAdjacentVerticesEnabledOn(e)) {
			// from static/dynamic to dynamic size: need to resize first, then import
			resizeAdjVertices(e.adjVerticesNumber());
			importReferencesFrom(e, base, ebase);
		}
	}
}

template<typename Vertex, typename El, bool h>
template<typename Element, typename ElVType>
void AdjacentVertices<Vertex, El, h>::importReferencesFrom(
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

} // namespace vcl::comp
