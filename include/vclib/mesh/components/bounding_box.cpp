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

#include "bounding_box.h"

namespace vcl::comp {

/**
 * @brief Returns the bounding box of this object.
 * @return The bounding box of this object.
 */
template<PointConcept P, typename El, bool O>
const Box<P>& BoundingBox<P, El, O>::boundingBox() const
{
	return Base::data(this);
}

/**
 * @brief Returns the bounding box of this object.
 * @return The bounding box of this object.
 */
template<PointConcept P, typename El, bool O>
Box<P>& BoundingBox<P, El, O>::boundingBox()
{
	return Base::data(this);
}

template<PointConcept P, typename El, bool O>
template<typename Element>
void BoundingBox<P, El, O>::importFrom(const Element& e)
{
	if constexpr(HasBoundingBox<Element>) {
		using ScalarType = typename P::ScalarType;
		boundingBox() = e.boundingBox().template cast<ScalarType>();
	}
}

} // namespace vcl::comp
