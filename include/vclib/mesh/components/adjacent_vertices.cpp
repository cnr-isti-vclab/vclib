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

template<typename Vertex, typename El, bool o>
void AdjacentVertices<Vertex, El, o>::init()
{
	Base::init(this);
}

template<typename Vertex, typename El, bool o>
bool AdjacentVertices<Vertex, El, o>::isEnabled() const
{
	return Base::isEnabled(this);
}

template<typename Vertex, typename El, bool o>
bool AdjacentVertices<Vertex, El, o>::isAdjVerticesEnabled() const
{
	return isEnabled();
}

/**
 * @brief Returns the number of adjacent vertices of the element.
 * @return The number of adjacent vertices of the element.
 */
template<typename Vertex, typename El, bool o>
uint AdjacentVertices<Vertex, El, o>::adjVerticesNumber() const
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
template<typename Vertex, typename El, bool o>
Vertex*& AdjacentVertices<Vertex, El, o>::adjVertex(uint i)
{
	return Base::container(this).at(i);
}

template<typename Vertex, typename El, bool o>
const Vertex* AdjacentVertices<Vertex, El, o>::adjVertex(uint i) const
{
	return Base::container(this).at(i);
}

template<typename Vertex, typename El, bool o>
Vertex*& AdjacentVertices<Vertex, El, o>::adjVertexMod(int i)
{
	return Base::container(this).atMod(i);
}

template<typename Vertex, typename El, bool o>
const Vertex* AdjacentVertices<Vertex, El, o>::adjVertexMod(int i) const
{
	return Base::container(this).atMod(i);
}

template<typename Vertex, typename El, bool o>
void AdjacentVertices<Vertex, El, o>::setAdjVertex(Vertex* v, uint i)
{
	Base::container(this).set(v, i);
}

template<typename Vertex, typename El, bool o>
void AdjacentVertices<Vertex, El, o>::setAdjVertices(const std::vector<Vertex*>& list)
{
	Base::container(this).set(list);
}

template<typename Vertex, typename El, bool o>
bool AdjacentVertices<Vertex, El, o>::containsAdjVertex(const Vertex* v) const
{
	return Base::container(this).contains(v);
}

template<typename Vertex, typename El, bool o>
typename AdjacentVertices<Vertex, El, o>::AdjacentVertexIterator
AdjacentVertices<Vertex, El, o>::findAdjVertex(const Vertex* v)
{
	return Base::container(this).find(v);
}

template<typename Vertex, typename El, bool o>
typename AdjacentVertices<Vertex, El, o>::ConstAdjacentVertexIterator
AdjacentVertices<Vertex, El, o>::findAdjVertex(const Vertex* v) const
{
	return Base::container(this).find(v);
}

template<typename Vertex, typename El, bool o>
int AdjacentVertices<Vertex, El, o>::indexOfAdjVertex(const Vertex* v) const
{
	return Base::container(this).indexOf(v);
}

/**
 * @brief Resize the container of the adjacent vertices to the given size.
 * @note This function is available only if the container of the Adjacent Vertices is has dynamic
 * size.
 * @param n
 */
template<typename Vertex, typename El, bool o>
void AdjacentVertices<Vertex, El, o>::resizeAdjVertices(uint n)
{
	Base::container(this).resize(n);
}

template<typename Vertex, typename El, bool o>
void AdjacentVertices<Vertex, El, o>::pushAdjVertex(Vertex* v)
{
	Base::container(this).pushBack(v);
}

template<typename Vertex, typename El, bool o>
void AdjacentVertices<Vertex, El, o>::insertAdjVertex(uint i, Vertex* v)
{
	Base::container(this).insert(i, v);
}

template<typename Vertex, typename El, bool o>
void AdjacentVertices<Vertex, El, o>::eraseAdjVertex(uint i)
{
	Base::container(this).erase(i);
}

template<typename Vertex, typename El, bool o>
void AdjacentVertices<Vertex, El, o>::clearAdjVertices()
{
	Base::container(this).clear();
}

template<typename Vertex, typename El, bool o>
typename AdjacentVertices<Vertex, El, o>::AdjacentVertexIterator AdjacentVertices<Vertex, El, o>::adjVertexBegin()
{
	return Base::container(this).begin();
}

template<typename Vertex, typename El, bool o>
typename AdjacentVertices<Vertex, El, o>::AdjacentVertexIterator AdjacentVertices<Vertex, El, o>::adjVertexEnd()
{
	return Base::container(this).end();
}

template<typename Vertex, typename El, bool o>
typename AdjacentVertices<Vertex, El, o>::ConstAdjacentVertexIterator
AdjacentVertices<Vertex, El, o>::adjVertexBegin() const
{
	return Base::container(this).begin();
}

template<typename Vertex, typename El, bool o>
typename AdjacentVertices<Vertex, El, o>::ConstAdjacentVertexIterator
AdjacentVertices<Vertex, El, o>::adjVertexEnd() const
{
	return Base::container(this).end();
}

template<typename Vertex, typename El, bool o>
typename AdjacentVertices<Vertex, El, o>::AdjacentVertexRange
AdjacentVertices<Vertex, El, o>::adjVertices()
{
	return AdjacentVertexRange(adjVertexBegin(), adjVertexEnd());
}

template<typename Vertex, typename El, bool o>
typename AdjacentVertices<Vertex, El, o>::ConstAdjacentVertexRange
AdjacentVertices<Vertex, El, o>::adjVertices() const
{
	return ConstAdjacentVertexRange(adjVertexBegin(), adjVertexEnd());
}

template<typename Vertex, typename El, bool o>
void AdjacentVertices<Vertex, El, o>::updateReferences(const Vertex* oldBase, const Vertex* newBase)
{
	Base::updateElementReferences(oldBase, newBase, this);
}

template<typename Vertex, typename El, bool o>
void AdjacentVertices<Vertex, El, o>::updateReferencesAfterCompact(
	const Vertex*           base,
	const std::vector<int>& newIndices)
{
	Base::updateElementReferencesAfterCompact(base, newIndices, this);
}

template<typename Vertex, typename El, bool o>
template<typename Element>
void AdjacentVertices<Vertex, El, o>::importFrom(const Element&)
{
}

template<typename Vertex, typename El, bool o>
template<typename Element, typename ElVType>
void AdjacentVertices<Vertex, El, o>::importReferencesFrom(
	const Element& e,
	Vertex*        base,
	const ElVType* ebase)
{
	if constexpr (HasAdjacentVertices<Element>) {
		if (isAdjacentVerticesEnabledOn(e)) {
			// from static/dynamic to dynamic size: need to resize first, then import
			resizeAdjVertices(e.adjVerticesNumber());
			importRefsFrom(e, base, ebase);
		}
	}
}

template<typename Vertex, typename El, bool o>
template<typename Element, typename ElVType>
void AdjacentVertices<Vertex, El, o>::importRefsFrom(
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
