/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#ifndef VCL_MESH_COMPONENTS_BOUNDING_BOX_H
#define VCL_MESH_COMPONENTS_BOUNDING_BOX_H

#include <vclib/space/box.h>

#include "concepts/bounding_box.h"
#include "internal/get_vertical_component_data.h"

namespace vcl::comp {

namespace internal {

template<PointConcept P, bool>
struct BoundingBoxData { Box<P> box; };

template<PointConcept P>
struct BoundingBoxData<P, false> { };

} // vcl::comp::internal

/**
 * @brief The BoundingBox component class represents an axis aligned bounding box. This class is
 * usually used as a component of a Mesh.
 *
 * The member functions of this class will be available in the instance of any Element or Mesh that
 * will contain this component.
 *
 * For example, if you have a Mesh `m` with the BoundingBox component, you'll be able to
 * access to this component member functions from `m`:
 *
 * @code{.cpp}
 * m.boundingBox();
 * @endcode
 */
template<PointConcept PointType, typename ElementType, bool horizontal>
class BoundingBox
{
public:
	using BoundingBoxType = Box<PointType>;

	const BoundingBoxType& boundingBox() const;
	BoundingBoxType&       boundingBox();

protected:
	template<typename Element>
	void importFrom(const Element& e);

private:
	Box<PointType>& box();
	const Box<PointType>& box() const;

	// contians the actual box, if the component is horizontal
	internal::BoundingBoxData<PointType, horizontal> data;
};

template <typename S, typename ElementType, bool horizontal>
using BoundingBox3  = BoundingBox<Point3<S>, ElementType, horizontal>;

template<typename ElementType, bool horizontal>
using BoundingBox3f = BoundingBox<Point3f, ElementType, horizontal>;

template<typename ElementType, bool horizontal>
using BoundingBox3d = BoundingBox<Point3d, ElementType, horizontal>;

} // namespace vcl::comp

#include "bounding_box.cpp"

#endif // VCL_MESH_COMPONENTS_BOUNDING_BOX_H
