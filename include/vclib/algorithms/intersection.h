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

#ifndef VCL_ALGORITHMS_INTERSECTION_H
#define VCL_ALGORITHMS_INTERSECTION_H

#include <vclib/algorithms/polygon.h>
#include <vclib/mesh/requirements.h>
#include <vclib/space/plane.h>

namespace vcl {

template<typename PlaneType, typename BoxType>
bool planeBoxIntersect(const PlaneType& p, const BoxType& b);

template<PointConcept PointType>
bool triangleBoxIntersect(
	const PointType&      p0,
	const PointType&      p1,
	const PointType&      p2,
	const Box<PointType>& b) requires (PointType::DIM == 3);

template<FaceConcept FaceType, PointConcept PointType>
bool faceBoxIntersect(
	const FaceType& f,
	const Box<PointType>& b);

template<EdgeMeshConcept EdgeMesh, FaceMeshConcept MeshType, typename PlaneType>
void meshPlaneIntersection(EdgeMesh& em, const MeshType& m, const PlaneType& pl);

}

#include "intersection.cpp"

#endif // VCL_ALGORITHMS_INTERSECTION_H
