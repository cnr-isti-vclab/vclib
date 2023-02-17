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

#ifndef VCL_ALGORITHMS_INTERSECTION_ELEMENT_H
#define VCL_ALGORITHMS_INTERSECTION_ELEMENT_H

#include <vclib/algorithms/polygon.h>
#include <vclib/mesh/requirements.h>
#include <vclib/space/plane.h>
#include <vclib/space/sphere.h>

namespace vcl {

template<PointConcept PointType>
bool triangleBoxIntersect(
	const PointType&      p0,
	const PointType&      p1,
	const PointType&      p2,
	const Box<PointType>& box) requires (PointType::DIM == 3);

template<FaceConcept FaceType, PointConcept PointType>
bool faceBoxIntersect(
	const FaceType& f,
	const Box<PointType>& box);

template<PointConcept PointType, typename SScalar>
bool triangleSphereItersect(
	PointType      p0,
	PointType      p1,
	PointType      p2,
	const vcl::Sphere<SScalar>& sphere,
	PointType& witness,
	std::pair<SScalar, SScalar>& res);

template<PointConcept PointType, typename SScalar>
bool triangleSphereItersect(
	const PointType&       p0,
	const PointType&       p1,
	const PointType&       p2,
	const Sphere<SScalar>& sphere);

template<FaceConcept FaceType, typename SScalar>
bool faceSphereItersect(
	const FaceType& f,
	const Sphere<SScalar>& sphere);

} // namespace vcl

#include "element.cpp"

#endif // VCL_ALGORITHMS_INTERSECTION_ELEMENT_H
