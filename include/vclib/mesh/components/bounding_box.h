/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

#ifndef VCL_MESH_COMPONENTS_BOUNDING_BOX_H
#define VCL_MESH_COMPONENTS_BOUNDING_BOX_H

#include <vclib/space/box.h>

namespace vcl::comp {

class BoundingBoxTriggerer
{
};

/**
 * @brief The BoundingBox component class represent an axis aligned bounding box. This class is
 * usually used as a component of a Mesh.
 */
template<typename  PointType>
class BoundingBox : public BoundingBoxTriggerer
{
public:
	using BoundingBoxType = Box<PointType>;

	const BoundingBoxType boundingBox() const { return box; }
	BoundingBoxType&      boundingBox() { return box; }

private:
	Box<PointType> box;
};

using BoundingBox3f = BoundingBox<Point3f>;
using BoundingBox3d = BoundingBox<Point3d>;

/**
 * Detector to check if a class has (inherits) BoundingBox
 */

template<typename T>
using hasBoundingBoxT = std::is_base_of<BoundingBoxTriggerer, T>;

template<typename U, typename T>
using ReturnIfHasBoundingBox = typename std::enable_if<hasBoundingBoxT<U>::value, T>::type;

template<typename T>
bool constexpr hasBoundingBox()
{
	return hasBoundingBoxT<T>::value;
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_BOUNDING_BOX_H
