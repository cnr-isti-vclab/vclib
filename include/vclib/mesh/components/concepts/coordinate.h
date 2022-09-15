/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_COORDINATE_H
#define VCL_MESH_COMPONENTS_CONCEPTS_COORDINATE_H

#include <vclib/misc/types.h>
#include <vclib/space/point.h>

namespace vcl::comp {

/**
 * @brief HasCoordinate concept
 *
 * This concept is satisfied only if a class has a member function 'coord()'.
 * No check is made on the return type.
 */
template<typename T>
concept HasCoordinate = requires(T o)
{
	typename T::CoordType;
	o.coord();
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_COORDINATE_H
