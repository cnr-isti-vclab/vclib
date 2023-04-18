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

#ifndef VCL_ALGORITHMS_CREATE_SPHERE_H
#define VCL_ALGORITHMS_CREATE_SPHERE_H

#include <vclib/mesh/requirements.h>
#include <vclib/space/sphere.h>

namespace vcl {

/**
 * @brief The CreateSphereArgs structs contains a series of parameters to generate a sphere.
 *
 * The main parameter is `mode`, an enumeration that indicates the method used to generate the
 * sphere mesh. Its values can be: `UV` (default), `NORMALIZED_CUBE`, `SPHERIFIED_CUBE` and
 * `ICOSAHEDRON`.
 * Please refer to https://github.com/caosdoar/spheres for more details about the available modes.
 *
 * Other arguments are:
 * - parallels (default 10): used when mode == `UV`;
 * - meridians (default 20): used when mode == `UV`;
 * - divisions (default 20): used when mode == `NORMALIZED_CUBE` or `SPHERIFIED_CUBE`
 */
struct CreateSphereArgs {
	typedef enum { UV = 0, NORMALIZED_CUBE, SPHERIFIED_CUBE, ICOSAHEDRON } CreateSphereMode;

	CreateSphereMode mode      = UV;
	uint             parallels = 10; // used for mode = UV
	uint             meridians = 20; // used for mode = UV
	uint             divisions = 20; // used for mode = NORMALIZED_CUBE, SPHERIFIED_CUBE, ICOSAHEDRON
};

template<FaceMeshConcept MeshType>
MeshType createSphereUV(const SphereConcept auto& sp, uint parallels = 10, uint meridians = 20);

template<FaceMeshConcept MeshType>
MeshType createSphereNormalizedCube(const SphereConcept auto& sp, uint divisions);

template<FaceMeshConcept MeshType>
MeshType createSphereSpherifiedCube(const SphereConcept auto& sp, uint divisions);

template<FaceMeshConcept MeshType>
MeshType createSphereIcosahedron(const SphereConcept auto& sp, uint divisions);

template<FaceMeshConcept MeshType>
MeshType createSphere(
	const SphereConcept auto& sp,
	const CreateSphereArgs& args = CreateSphereArgs());

template<FaceMeshConcept MeshType>
MeshType createSphere();

} // namespace vcl

#include "sphere.cpp"

#endif // VCL_ALGORITHMS_CREATE_SPHERE_H
