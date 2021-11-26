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

template<typename Vertex, int N>
AdjacentVertices<Vertex, N>::AdjacentVertices() : Base()
{
}

template<typename Vertex, int N>
uint AdjacentVertices<Vertex, N>::adjVerticesNumber() const
{
	return Base::size();
}

template<typename Vertex, int N>
Vertex*& AdjacentVertices<Vertex, N>::adjVertex(uint i)
{
	return Base::at(i);
}

template<typename Vertex, int N>
const Vertex* AdjacentVertices<Vertex, N>::adjVertex(uint i) const
{
	return Base::at(i);
}

template<typename Vertex, int N>
Vertex*& AdjacentVertices<Vertex, N>::adjVertexMod(int i)
{
	return Base::atMod(i);
}

template<typename Vertex, int N>
const Vertex* AdjacentVertices<Vertex, N>::adjVertexMod(int i) const
{
	return Base::atMod(i);
}

template<typename Vertex, int N>
void AdjacentVertices<Vertex, N>::setAdjVertex(Vertex* v, uint i)
{
	Base::set(v, i);
}

template<typename Vertex, int N>
void AdjacentVertices<Vertex, N>::setAdjVertices(const std::vector<Vertex*>& list)
{
	Base::set(list);
}

template<typename Vertex, int N>
bool AdjacentVertices<Vertex, N>::containsAdjVertex(const Vertex* v) const
{
	return Base::contains(v);
}

template<typename Vertex, int N>
typename AdjacentVertices<Vertex, N>::AdjacentVertexIterator
AdjacentVertices<Vertex, N>::findAdjVertex(const Vertex* v)
{
	return Base::find(v);
}

template<typename Vertex, int N>
typename AdjacentVertices<Vertex, N>::ConstAdjacentVertexIterator
AdjacentVertices<Vertex, N>::findAdjVertex(const Vertex* v) const
{
	return Base::find(v);
}

template<typename Vertex, int N>
int AdjacentVertices<Vertex, N>::indexOfAdjVertex(const Vertex* v) const
{
	return Base::indexOf(v);
}

/**
 * @brief Resize the container of the adjacent vertices to the given size.
 * @note This function is available only if the container of the Adjacent Vertices is has dynamic
 * size.
 * @param n
 */
template<typename Vertex, int N>
template<int M>
VCL_ENABLE_IF(M < 0, void) AdjacentVertices<Vertex, N>::resizeAdjVertices(uint n)
{
	Base::resize(n);
}

template<typename Vertex, int N>
template<int M>
VCL_ENABLE_IF(M < 0, void)
AdjacentVertices<Vertex, N>::pushAdjVertex(Vertex* v)
{
	Base::pushBack(v);
}

template<typename Vertex, int N>
template<int M>
VCL_ENABLE_IF(M < 0, void)
AdjacentVertices<Vertex, N>::insertAdjVertex(uint i, Vertex* v)
{
	Base::insert(i, v);
}

template<typename Vertex, int N>
template<int M>
VCL_ENABLE_IF(M < 0, void)
AdjacentVertices<Vertex, N>::eraseAdjVertex(uint i)
{
	Base::erase(i);
}

template<typename Vertex, int N>
template<int M>
VCL_ENABLE_IF(M < 0, void)
AdjacentVertices<Vertex, N>::clearAdjVertices()
{
	Base::clear();
}

template<typename Vertex, int N>
typename AdjacentVertices<Vertex, N>::AdjacentVertexIterator
AdjacentVertices<Vertex, N>::adjVertexBegin()
{
	return Base::begin();
}

template<typename Vertex, int N>
typename AdjacentVertices<Vertex, N>::AdjacentVertexIterator
AdjacentVertices<Vertex, N>::adjVertexEnd()
{
	return Base::end();
}

template<typename Vertex, int N>
typename AdjacentVertices<Vertex, N>::ConstAdjacentVertexIterator
AdjacentVertices<Vertex, N>::adjVertexBegin() const
{
	return Base::begin();
}

template<typename Vertex, int N>
typename AdjacentVertices<Vertex, N>::ConstAdjacentVertexIterator
AdjacentVertices<Vertex, N>::adjVertexEnd() const
{
	return Base::end();
}

template<typename Vertex, int N>
typename AdjacentVertices<Vertex, N>::AdjacentVertexRangeIterator
AdjacentVertices<Vertex, N>::adjVertices()
{
	return Base::rangeIterator();
}

template<typename Vertex, int N>
typename AdjacentVertices<Vertex, N>::ConstAdjacentVertexRangeIterator
AdjacentVertices<Vertex, N>::adjVertices() const
{
	return Base::rangeIterator();
}

template<typename Vertex, int N>
void AdjacentVertices<Vertex, N>::updateVertexReferences(
	const Vertex* oldBase,
	const Vertex* newBase)
{
	Base::updateElementReferences(oldBase, newBase);
}

template<typename Vertex, int N>
void AdjacentVertices<Vertex, N>::updateVertexReferencesAfterCompact(
	const Vertex*           base,
	const std::vector<int>& newIndices)
{
	Base::updateElementReferencesAfterCompact(base, newIndices);
}

} // namespace vcl::comp
