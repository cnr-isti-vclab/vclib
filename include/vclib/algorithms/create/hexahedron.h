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

#ifndef VCL_ALGORITHMS_CREATE_HEXAHEDRON_H
#define VCL_ALGORITHMS_CREATE_HEXAHEDRON_H

namespace vcl {

template<typename MeshType>
MeshType createHexahedron();

template<typename MeshType, typename CoordType>
MeshType createHexahedron(
	const CoordType& min,
	const CoordType& max);

template<typename MeshType, typename CoordType>
MeshType createCube(
	const CoordType& min,
	double edgeLength);

} // namespace vcl

#include "hexahedron.cpp"

#endif // VCL_ALGORITHMS_CREATE_HEXAHEDRON_H
