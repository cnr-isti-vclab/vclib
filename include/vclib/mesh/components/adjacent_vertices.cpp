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

#include "adjacent_vertices.h"

namespace vcl::comp {

/**
 * @brief Creates a Container of Adjacent Vertices, stored as Vertex Pointers.
 * If the size of the container is static, all the values will be set to nullptr.
 * If the size is dynamic, the number of values in the container is zero.
 */
template<typename Vertex>
AdjacentVertices<Vertex>::AdjacentVertices() : Base()
{
}

/**
 * @brief Returns the number of adjacent vertices of the element.
 * @return The number of adjacent vertices of the element.
 */
template<typename Vertex>
uint AdjacentVertices<Vertex>::adjVerticesNumber() const
{
	return Base::size();
}

/**
 * @brief Returns a reference of the pointer to the i-th adjacent vertex of an element.
 *
 * You can use this function to set the i-th adjacent vertex:
 *
 * @code{.cpp}
 * e.adjVertex(2) = &m.vertex(k); // the second adj vertex of e will point to the k-th vertex of the mesh.
 * @endcode
 *
 * @param i: the position of the required adjacent list in the container.
 * @return The pointer i-th adjacent vertex of the element.
 */
template<typename Vertex>
Vertex*& AdjacentVertices<Vertex>::adjVertex(uint i)
{
	return Base::at(i);
}

template<typename Vertex>
const Vertex* AdjacentVertices<Vertex>::adjVertex(uint i) const
{
	return Base::at(i);
}

template<typename Vertex>
Vertex*& AdjacentVertices<Vertex>::adjVertexMod(int i)
{
	return Base::atMod(i);
}

template<typename Vertex>
const Vertex* AdjacentVertices<Vertex>::adjVertexMod(int i) const
{
	return Base::atMod(i);
}

template<typename Vertex>
void AdjacentVertices<Vertex>::setAdjVertex(Vertex* v, uint i)
{
	Base::set(v, i);
}

template<typename Vertex>
void AdjacentVertices<Vertex>::setAdjVertices(const std::vector<Vertex*>& list)
{
	Base::set(list);
}

template<typename Vertex>
bool AdjacentVertices<Vertex>::containsAdjVertex(const Vertex* v) const
{
	return Base::contains(v);
}

template<typename Vertex>
typename AdjacentVertices<Vertex>::AdjacentVertexIterator
AdjacentVertices<Vertex>::findAdjVertex(const Vertex* v)
{
	return Base::find(v);
}

template<typename Vertex>
typename AdjacentVertices<Vertex>::ConstAdjacentVertexIterator
AdjacentVertices<Vertex>::findAdjVertex(const Vertex* v) const
{
	return Base::find(v);
}

template<typename Vertex>
int AdjacentVertices<Vertex>::indexOfAdjVertex(const Vertex* v) const
{
	return Base::indexOf(v);
}

/**
 * @brief Resize the container of the adjacent vertices to the given size.
 * @note This function is available only if the container of the Adjacent Vertices is has dynamic
 * size.
 * @param n
 */
template<typename Vertex>
void AdjacentVertices<Vertex>::resizeAdjVertices(uint n)
{
	Base::resize(n);
}

template<typename Vertex>
void AdjacentVertices<Vertex>::pushAdjVertex(Vertex* v)
{
	Base::pushBack(v);
}

template<typename Vertex>
void AdjacentVertices<Vertex>::insertAdjVertex(uint i, Vertex* v)
{
	Base::insert(i, v);
}

template<typename Vertex>
void AdjacentVertices<Vertex>::eraseAdjVertex(uint i)
{
	Base::erase(i);
}

template<typename Vertex>
void AdjacentVertices<Vertex>::clearAdjVertices()
{
	Base::clear();
}

template<typename Vertex>
typename AdjacentVertices<Vertex>::AdjacentVertexIterator
AdjacentVertices<Vertex>::adjVertexBegin()
{
	return Base::begin();
}

template<typename Vertex>
typename AdjacentVertices<Vertex>::AdjacentVertexIterator
AdjacentVertices<Vertex>::adjVertexEnd()
{
	return Base::end();
}

template<typename Vertex>
typename AdjacentVertices<Vertex>::ConstAdjacentVertexIterator
AdjacentVertices<Vertex>::adjVertexBegin() const
{
	return Base::begin();
}

template<typename Vertex>
typename AdjacentVertices<Vertex>::ConstAdjacentVertexIterator
AdjacentVertices<Vertex>::adjVertexEnd() const
{
	return Base::end();
}

template<typename Vertex>
typename AdjacentVertices<Vertex>::AdjacentVertexRangeIterator
AdjacentVertices<Vertex>::adjVertices()
{
	return Base::rangeIterator();
}

template<typename Vertex>
typename AdjacentVertices<Vertex>::ConstAdjacentVertexRangeIterator
AdjacentVertices<Vertex>::adjVertices() const
{
	return Base::rangeIterator();
}

template<typename Vertex>
void AdjacentVertices<Vertex>::updateVertexReferences(
	const Vertex* oldBase,
	const Vertex* newBase)
{
	Base::updateElementReferences(oldBase, newBase);
}

template<typename Vertex>
void AdjacentVertices<Vertex>::updateVertexReferencesAfterCompact(
	const Vertex*           base,
	const std::vector<int>& newIndices)
{
	Base::updateElementReferencesAfterCompact(base, newIndices);
}

template<typename Vertex>
template<typename Element>
void AdjacentVertices<Vertex>::importFrom(const Element&)
{
}

template<typename Vertex>
template<typename Element, typename ElVType>
void AdjacentVertices<Vertex>::importVertexReferencesFrom(
	const Element& e,
	Vertex* base,
	const ElVType* ebase)
{
	if constexpr (hasAdjacentVertices<Element>()) {
		if (isAdjacentVerticesEnabledOn(e)){
			// from static/dynamic to dynamic size: need to resize first, then import
			resizeAdjVertices(e.adjVerticesNumber());
			importReferencesFrom(e, base, ebase);
		}
	}
}

template<typename Vertex>
template<typename Element, typename ElVType>
void AdjacentVertices<Vertex>::importReferencesFrom(
	const Element& e,
	Vertex* base,
	const ElVType* ebase)
{
	if (ebase != nullptr && base != nullptr) {
		for (uint i = 0; i < e.adjVerticesNumber(); ++i){
			if (e.adjVertex(i) != nullptr){
				adjVertex(i) = base + (e.adjVertex(i) - ebase);
			}
		}
	}
}

} // namespace vcl::comp
