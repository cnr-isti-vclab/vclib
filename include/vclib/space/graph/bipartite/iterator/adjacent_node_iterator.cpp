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

#include "adjacent_node_iterator.h"

namespace vcl::internal {

template<typename Graph, typename Iterator>
AdjacentNodeIterator<Graph, Iterator>::AdjacentNodeIterator()
{
}

template<typename Graph, typename Iterator>
AdjacentNodeIterator<Graph, Iterator>::AdjacentNodeIterator(const Graph& g, Iterator it) :
		g(&g), it(it)
{
}

template<typename Graph, typename Iterator>
bool AdjacentNodeIterator<Graph, Iterator>::operator==(const AdjacentNodeIterator& otherIterator) const
{
	return (g == otherIterator.g && it == otherIterator.it);
}

template<typename Graph, typename Iterator>
bool AdjacentNodeIterator<Graph, Iterator>::operator!=(const AdjacentNodeIterator& otherIterator) const
{
	return !(*this == otherIterator);
}

template<typename Graph, typename Iterator>
AdjacentNodeIterator<Graph, Iterator> AdjacentNodeIterator<Graph, Iterator>::operator++()
{
	++it;
	return *this;
}

template<typename Graph, typename Iterator>
AdjacentNodeIterator<Graph, Iterator> AdjacentNodeIterator<Graph, Iterator>::operator++(int)
{
	AdjacentNodeIterator tmp;
	++it;
	return tmp;
}

} // namespace vcl::internal


