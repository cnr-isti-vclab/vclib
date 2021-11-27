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

#include "edge_container.h"

namespace vcl::mesh {

/**
 * @brief Empty constructor that creates an empty container of Edges.
 */
template<typename T>
EdgeContainer<T>::EdgeContainer()
{
}

/**
 * @brief Returns a const reference of the edge at the i-th position in the Edge Container of the
 * Mesh, which will be the edge having index = i.
 *
 * This function does not perform any sanity check: if i is less than edgeContainerSize(), this
 * function will return a valid Edge reference (note that the Edge may have been flagged as
 * deleted).
 *
 * @param[in] i: the index of the edge that will be returned.
 */
template<typename T>
const typename EdgeContainer<T>::EdgeType& EdgeContainer<T>::edge(uint i) const
{
	return Base::vec[i];
}

/**
 * @brief Returns a reference of the edge at the i-th position in the Edge Container of the Mesh,
 * which will be the edge having index = i.
 *
 * This function does not perform any sanity check: if i is less than edgeContainerSize(), this
 * function will return a valid Edge reference (note that the Edge may have been flagged as
 * deleted).
 *
 * @param[in] i: the index of the edge that will be returned.
 */
template<typename T>
typename EdgeContainer<T>::EdgeType& EdgeContainer<T>::edge(uint i)
{
	return Base::vec[i];
}

/**
 * @brief Returns the number of **non-deleted** edges contained in the Edge container of the Mesh.
 *
 * If edgeNumber() != edgeContainerSize(), it means that there are some edges that are
 * flagged as deleted.
 *
 * @return the number of non-deleted edges of the Mesh.
 */
template<typename T>
uint EdgeContainer<T>::edgeNumber() const
{
	return en;
}

/**
 * @brief Returns the number of edges (also deleted) contained in the Edge container of the Mesh.
 *
 * If edgeNumber() != edgeContainerSize(), it means that there are some edges that are
 * flagged as deleted.
 *
 * @return the number of all the edges contained in the Mesh.
 */
template<typename T>
uint EdgeContainer<T>::edgeContainerSize() const
{
	return Base::vec.size();
}

/**
 * @brief Returns the number of deleted edges in the Edge container, that is edgeContainerSize() -
 * edgeNumber().
 *
 * @return The number of deleted edges in the container.
 */
template<typename T>
uint EdgeContainer<T>::deletedEdgeNumber() const
{
	return edgeContainerSize() - edgeNumber();
}

} // namespace vcl::mesh
