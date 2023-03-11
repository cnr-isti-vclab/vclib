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
#include <vclib/space/triangle.h>

namespace vcl {

/* Normal */

template<FaceConcept Triangle>
typename Triangle::VertexType::CoordType triangleNormal(const Triangle& t);

/* Barycenter */

template<FaceConcept Triangle>
typename Triangle::VertexType::CoordType triangleBarycenter(const Triangle& t);

/* Baricentric Coordinates */

template <FaceConcept Triangle, typename ScalarType>
typename Triangle::CoordType triangleBarycentricCoordinatePoint(
	const Triangle& t,
	const Point3<ScalarType>& barCoords);

/* Circumcenter */

template<FaceConcept Triangle>
typename Triangle::VertexType::CoordType triangleCircumcenter(const Triangle& t);

/* Perimeter */

template<FaceConcept Triangle>
auto trianglePerimeter(const Triangle& t);

/* Area */

template<FaceConcept Triangle>
auto triangleArea(const Triangle& t);

/* Quality */

template<FaceConcept Triangle>
double triangleQuality(const Triangle& t);

template<FaceConcept Triangle>
double triangleQualityRadii(const Triangle& t);

template<FaceConcept Triangle>
double triangleQualityMeanRatio(const Triangle& t);

} // namespace vcl

#include "triangle.cpp"

#endif // VCL_ALGORITHM_POLYGON_GEOMETRY_TRIANGLE_H
