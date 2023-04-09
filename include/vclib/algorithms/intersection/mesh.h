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

#ifndef VCL_ALGORITHMS_INTERSECTION_MESH_H
#define VCL_ALGORITHMS_INTERSECTION_MESH_H

#include "element.h"

namespace vcl {

template<EdgeMeshConcept EdgeMesh, FaceMeshConcept MeshType, PlaneConcept PlaneType>
EdgeMesh meshPlaneIntersection(const MeshType& m, const PlaneType& pl);

template<FaceMeshConcept MeshType, typename SScalar>
MeshType meshSphereIntersection(const MeshType& m, const vcl::Sphere<SScalar>& sphere);

template<FaceMeshConcept MeshType, typename SScalar>
MeshType meshSphereIntersection(const MeshType& m, const vcl::Sphere<SScalar>& sphere, double tol);

} // namespace vcl

#include "mesh.cpp"

#endif // VCL_ALGORITHMS_INTERSECTION_MESH_H
