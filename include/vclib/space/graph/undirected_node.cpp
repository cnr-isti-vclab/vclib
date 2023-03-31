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

#include "undirected_node.h"

namespace vcl {

template<class T>
UndirectedNode<T>::UndirectedNode()
{
}

template<class T>
UndirectedNode<T>::UndirectedNode(const T& info) : inf(inf)
{
}

template<class T>
const T& UndirectedNode<T>::info() const
{
	return inf;
}

template<class T>
T& UndirectedNode<T>::info()
{
	return inf;
}

template<class T>
void UndirectedNode<T>::addAdjacent(unsigned int node)
{
	adjacentNodes.insert(node);
}

template<class T>
bool UndirectedNode<T>::isAdjacent(unsigned int node) const
{
	return adjacentNodes.find(node) != adjacentNodes.end();
}

template<class T>
void UndirectedNode<T>::deleteAdjacent(unsigned int node)
{
	adjacentNodes.erase(node);
}

template<class T>
void UndirectedNode<T>::clearAdjacentNodes()
{
	adjacentNodes.clear();
}

template<class T>
std::unordered_set<unsigned int>::const_iterator UndirectedNode<T>::begin() const
{
	return adjacentNodes.begin();
}

template<class T>
std::unordered_set<unsigned int>::const_iterator UndirectedNode<T>::end() const
{
	return adjacentNodes.end();
}

template<class T>
unsigned int UndirectedNode<T>::sizeAdjacentNodes() const
{
	return (unsigned int) adjacentNodes.size();
}

} // namespace vcl
