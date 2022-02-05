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

#include "half_edge_container.h"

namespace vcl::mesh {

/**
 * @brief Empty constructor that creates an empty container of HalfEdges.
 */
template<typename T>
HalfEdgeContainer<T>::HalfEdgeContainer()
{
}

/**
 * @brief Returns a const reference of the halfedge at the i-th position in the halfEdge Container
 * of the Mesh, which will be the halfedge having index = i.
 *
 * This function does not perform any sanity check: if i is less than halfedgeContainerSize(), this
 * function will return a valid halfEdge reference (note that the halfEdge may have been flagged as
 * deleted).
 *
 * @param[in] i: the index of the halfedge that will be returned.
 */
template<typename T>
const typename HalfEdgeContainer<T>::HalfEdgeType& HalfEdgeContainer<T>::halfEdge(uint i) const
{
	return Base::element(i);
}

/**
 * @brief Returns a reference of the halfedge at the i-th position in the halfEdge Container of the
 * Mesh, which will be the halfedge having index = i.
 *
 * This function does not perform any sanity check: if i is less than halfedgeContainerSize(), this
 * function will return a valid halfEdge reference (note that the halfEdge may have been flagged as
 * deleted).
 *
 * @param[in] i: the index of the halfedge that will be returned.
 */
template<typename T>
typename HalfEdgeContainer<T>::HalfEdgeType& HalfEdgeContainer<T>::halfEdge(uint i)
{
	return Base::element(i);
}

/**
 * @brief Returns the number of **non-deleted** halfedges contained in the halfEdge container of the
 * Mesh.
 *
 * If halfEdgeNumber() != halfEdgeContainerSize(), it means that there are some halfedges that are
 * flagged as deleted.
 *
 * @return the number of non-deleted halfedges of the Mesh.
 */
template<typename T>
uint HalfEdgeContainer<T>::halfEdgeNumber() const
{
	return Base::elementNumber();
}

/**
 * @brief Returns the number of halfedges (also deleted) contained in the halfEdge container of the
 * Mesh.
 *
 * If halfEdgeNumber() != halfEdgeContainerSize(), it means that there are some halfedges that are
 * flagged as deleted.
 *
 * @return the number of all the halfedges contained in the Mesh.
 */
template<typename T>
uint HalfEdgeContainer<T>::halfEdgeContainerSize() const
{
	return Base::elementContainerSize();
}

/**
 * @brief Returns the number of deleted halfedges in the halfEdge container, that is
 * halfEdgeContainerSize() - halfEdgeNumber().
 *
 * @return The number of deleted halfedges in the container.
 */
template<typename T>
uint HalfEdgeContainer<T>::deletedHalfEdgeNumber() const
{
	return Base::deletedElementNumber();
}

/**
 * @brief Marks as deleted the halfedge with the given id.
 *
 * This member function does not perform any reallocation of the halfedges: the deleted halfedges
 * will stay in the halfEdge Container, but will be marked as deleted.
 *
 * Deleted halfedges are automatically jumped by the iterators provided by the halfEdge Container.
 *
 * @param[in] i: the id of the halfedge that will be marked as deleted.
 */
template<typename T>
void HalfEdgeContainer<T>::deleteHalfEdge(uint i)
{
	Base::deleteElement(i);
}

/**
 * @brief Marks as deleted the given halfedge, before asserting that the halfedge belongs to this
 * container.
 *
 * This member function does not perform any reallocation of the halfedges: the deleted halfedges
 * will stay in the halfEdge Container, but will be marked as deleted.
 *
 * Deleted halfedges are automatically jumped by the iterators provided by the halfEdge Container.
 *
 * @param[in] e: the pointer of the halfedge that will be marked as deleted.
 */
template<typename T>
void HalfEdgeContainer<T>::deleteHalfEdge(const HalfEdgeType* e)
{
	Base::deleteElement(e);
}

} // namespace vcl::mesh
