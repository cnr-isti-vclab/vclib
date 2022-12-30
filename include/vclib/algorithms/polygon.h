/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#ifndef VCL_ALGORITHMS_POLYGON_H
#define VCL_ALGORITHMS_POLYGON_H

#include <vclib/mesh/requirements.h>
#include <vclib/space/point.h>

namespace vcl {

template <typename PointType>
PointType triangleNormal(const PointType& p0, const PointType& p1, const PointType& p2);

template<typename Triangle>
typename Triangle::VertexType::CoordType triangleNormal(const Triangle& t);

template <typename PointType>
PointType triangleBarycenter(const PointType& p0, const PointType& p1, const PointType& p2);

template<typename Triangle, typename PointType = Point3f>
PointType triangleBarycenter(const Triangle& t);

template <typename PointType>
PointType triangleWeightedBarycenter(
	const PointType& p0, typename PointType::ScalarType w0,
	const PointType& p1, typename PointType::ScalarType w1,
	const PointType& p2, typename PointType::ScalarType w2);

template <typename PointType, typename ScalarType>
PointType triangleBarycentricCoordinatePoint(
	const PointType& p0,
	const PointType& p1,
	const PointType& p2,
	const Point3<ScalarType> &barCoords);

template <typename Triangle, typename ScalarType>
typename Triangle::CoordType triangleBarycentricCoordinatePoint(
	const Triangle& t,
	const Point3<ScalarType>& barCoords);

template<typename PointType>
typename PointType::ScalarType
triangleArea(const PointType& p0, const PointType& p1, const PointType& p2);

template<typename Triangle, typename ScalarType = double>
ScalarType triangleArea(const Triangle& t);

template<typename PointType>
PointType polygonNormal(const std::vector<PointType>& p);

template<typename Polygon>
typename Polygon::VertexType::CoordType polygonNormal(const Polygon& p);

template<typename PointType>
PointType polygonBarycenter(const std::vector<PointType>& p);

template<typename Polygon>
typename Polygon::VertexType::CoordType polygonBarycenter(const Polygon& p);

template<typename PointType>
PointType polygonWeighedBarycenter(
	const std::vector<PointType>&                      p,
	const std::vector<typename PointType::ScalarType>& w);

template<typename PointType>
typename PointType::ScalarType polygonArea(const std::vector<PointType>& p);

template<typename Polygon, typename ScalarType = double>
ScalarType polygonArea(const Polygon& p);

template<typename Scalar>
std::vector<uint> earCut(const std::vector<Point2<Scalar>>& polygon);

template<typename Scalar>
std::vector<uint> earCut(const std::vector<Point3<Scalar>>& polygon);

template<typename Polygon>
std::vector<uint> earCut(const Polygon& polygon);

template<FaceMeshConcept MeshType, typename FaceType>
void addTriangleFacesFromPolygon(MeshType& m, FaceType& f, const std::vector<uint>& polygon);

template<FaceMeshConcept MeshType>
uint addTriangleFacesFromPolygon(MeshType& m, const std::vector<uint>& polygon);

} // namespace vcl

#include "polygon.cpp"

#endif // VCL_ALGORITHMS_POLYGON_H
