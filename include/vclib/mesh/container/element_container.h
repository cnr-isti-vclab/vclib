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

#ifndef VCL_MESH_CONTAINER_ELEMENT_CONTAINER_H
#define VCL_MESH_CONTAINER_ELEMENT_CONTAINER_H

#include <vector>
#include "../components/vertical/vertical_info.h"
#include "../components/vertical/vectors/vertical_components_vector.h"

namespace vcl::mesh {

template <typename T>
class ElementContainer
{
	friend class comp::VerticalInfo<T>;

public:
	ElementContainer() {};

protected:
	std::vector<T> vec;
	internal::VerticalComponentsVector<T> optionalVec;
};

}

#endif // VCL_MESH_CONTAINER_ELEMENT_CONTAINER_H
