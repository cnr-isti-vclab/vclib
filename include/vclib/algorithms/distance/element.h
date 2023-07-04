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

#ifndef VCL_ALGORITHMS_DISTANCE_ELEMENT_H
#define VCL_ALGORITHMS_DISTANCE_ELEMENT_H

#include <vclib/mesh/requirements.h>

namespace vcl {

template<Point3Concept PointType, FaceConcept FaceType, typename ScalarType>
auto pointFaceDistance(
	const PointType& p,
	const FaceType&  f,
	ScalarType       maxDist,
	PointType&       closest,
	bool             signedDist = false);

template<Point3Concept PointType, FaceConcept FaceType, typename ScalarType>
auto pointFaceDistance(
	const PointType& p,
	const FaceType&  f,
	ScalarType       maxDist,
	bool             signedDist = false);

template<Point3Concept PointType, FaceConcept FaceType>
auto pointFaceDistance(
	const PointType& p,
	const FaceType&  f,
	PointType&       closest,
	bool             signedDist = false);

template<Point3Concept PointType, FaceConcept FaceType>
auto pointFaceDistance(
	const PointType& p,
	const FaceType&  f,
	bool             signedDist = false);

} // namespace vcl

#include "element.cpp"

#endif // VCL_ALGORITHMS_DISTANCE_ELEMENT_H
