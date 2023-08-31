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
 * @brief Returns a const reference to the bounding box of this object.
 * @return A const reference to the bounding box of this object.
 */
template<PointConcept P, typename El, bool O>
const Box<P>& BoundingBox<P, El, O>::boundingBox() const
{
	return Base::data();
}

/**
 * @brief Returns a reference to the bounding box of this object.
 * @return A reference to the bounding box of this object.
 */
template<PointConcept P, typename El, bool O>
Box<P>& BoundingBox<P, El, O>::boundingBox()
{
	return Base::data();
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

/**
 * @brief Checks if the given Element/Mesh has BoundingBox component available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element/mesh to check. Must be of a type that
 * satisfies the ElementOrMeshConcept.
 * @return `true` if the element/mesh has BoundingBox component available,
 * `false` otherwise.
 */
bool isBoundingBoxAvailableOn(const ElementOrMeshConcept auto& element)
{
	return isComponentAvailableOn<BOUNDING_BOX>(element);
}

} // namespace vcl::comp
