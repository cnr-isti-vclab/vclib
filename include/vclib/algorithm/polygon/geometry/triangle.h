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

#ifndef VCL_ALGORITHM_POLYGON_GEOMETRY_TRIANGLE_H
#define VCL_ALGORITHM_POLYGON_GEOMETRY_TRIANGLE_H

#include <vclib/mesh/requirements.h>
#include <vclib/space/point.h>

namespace vcl {

/* Normal */

template <PointConcept PointType>
PointType triangleNormal(const PointType& p0, const PointType& p1, const PointType& p2);

template<FaceConcept Triangle>
typename Triangle::VertexType::CoordType triangleNormal(const Triangle& t);

/* Barycenter */

template <PointConcept PointType>
PointType triangleBarycenter(const PointType& p0, const PointType& p1, const PointType& p2);

template<FaceConcept Triangle>
typename Triangle::VertexType::CoordType triangleBarycenter(const Triangle& t);

template <PointConcept PointType>
PointType triangleWeightedBarycenter(
	const PointType& p0, typename PointType::ScalarType w0,
	const PointType& p1, typename PointType::ScalarType w1,
	const PointType& p2, typename PointType::ScalarType w2);

/* Baricentric Coordinates */

template <PointConcept PointType, typename ScalarType>
PointType triangleBarycentricCoordinatePoint(
	const PointType& p0,
	const PointType& p1,
	const PointType& p2,
	const Point3<ScalarType> &barCoords);

template <FaceConcept Triangle, typename ScalarType>
typename Triangle::CoordType triangleBarycentricCoordinatePoint(
	const Triangle& t,
	const Point3<ScalarType>& barCoords);

/* Area */

template<PointConcept PointType>
typename PointType::ScalarType
triangleArea(const PointType& p0, const PointType& p1, const PointType& p2);

template<FaceConcept Triangle>
auto triangleArea(const Triangle& t);

/* Perimeter */

template<PointConcept PointType>
typename PointType::ScalarType
trianglePerimeter(const PointType& p0, const PointType& p1, const PointType& p2);

template<FaceConcept Triangle>
auto trianglePerimeter(const Triangle& t);

/* Quality */

template <PointConcept PointType>
double triangleQuality(const PointType& p0, const PointType& p1, const PointType& p2);

template<FaceConcept Triangle>
double triangleQuality(const Triangle& t);

template <PointConcept PointType>
double triangleQualityRadii(const PointType& p0, const PointType& p1, const PointType& p2);

template<FaceConcept Triangle>
double triangleQualityRadii(const Triangle& t);

template <PointConcept PointType>
double triangleQualityMeanRatio(const PointType& p0, const PointType& p1, const PointType& p2);

template<FaceConcept Triangle>
double triangleQualityMeanRatio(const Triangle& t);

/* Circumcenter */

template <PointConcept PointType>
PointType triangleCircumcenter(const PointType& p0, const PointType& p1, const PointType& p2);

template<FaceConcept Triangle>
typename Triangle::VertexType::CoordType triangleCircumcenter(const Triangle& t);

} // namespace vcl

#include "triangle.cpp"

#endif // VCL_ALGORITHM_POLYGON_GEOMETRY_TRIANGLE_H
