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

#include "bounding_box.h"

namespace vcl::comp {

/**
 * @brief Returns the bounding box of this object.
 * @return The bounding box of this object.
 */
template<typename PointType>
const Box<PointType>& BoundingBox<PointType>::boundingBox() const
{
	return box;
}

/**
 * @brief Returns the bounding box of this object.
 * @return The bounding box of this object.
 */
template<typename PointType>
Box<PointType>& BoundingBox<PointType>::boundingBox()
{
	return box;
}

template<typename PointType>
template<typename Element>
void BoundingBox<PointType>::importFrom(const Element& e)
{
	if constexpr(hasBoundingBox<Element>()) {
		box = e.boundingBox();
	}
}

} // namespace vcl::comp
