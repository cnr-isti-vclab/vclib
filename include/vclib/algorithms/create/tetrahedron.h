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

#ifndef VCL_ALGORITHMS_CREATE_TETRAHEDRON_H
#define VCL_ALGORITHMS_CREATE_TETRAHEDRON_H

#include <vclib/mesh/requirements.h>

namespace vcl {

template<FaceMeshConcept MeshType>
MeshType createTetrahedron();

template<FaceMeshConcept MeshType, typename CoordType>
MeshType createTetrahedron(
	const CoordType& p0,
	const CoordType& p1,
	const CoordType& p2,
	const CoordType& p3);

} // namespace vcl

#include "tetrahedron.cpp"

#endif // VCL_ALGORITHMS_CREATE_TETRAHEDRON_H
