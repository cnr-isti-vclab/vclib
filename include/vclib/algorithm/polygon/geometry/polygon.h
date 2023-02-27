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

#ifndef VCL_ALGORITHM_POLYGON_GEOMETRY_POLYGON_H
#define VCL_ALGORITHM_POLYGON_GEOMETRY_POLYGON_H

#include <vclib/mesh/requirements.h>
#include <vclib/space/point.h>

namespace vcl {

/* Normal */

template<typename Iterator>
auto polygonNormal(Iterator begin, Iterator end)
	requires PointConcept<typename Iterator::value_type>;

template<typename Polygon>
typename Polygon::VertexType::CoordType polygonNormal(const Polygon& p);

/* Barycenter */

template<typename Iterator>
auto polygonBarycenter(Iterator begin, Iterator end)
	requires PointConcept<typename Iterator::value_type>;

template<typename Polygon>
typename Polygon::VertexType::CoordType polygonBarycenter(const Polygon& p);

template<typename Iterator, typename WIterator>
auto polygonWeighedBarycenter(Iterator begin, Iterator end, WIterator wbegin)
	requires PointConcept<typename Iterator::value_type>;

/* Area */

template<typename Iterator>
auto polygonArea(Iterator begin, Iterator end) requires PointConcept<typename Iterator::value_type>;

template<typename Polygon, typename ScalarType = double>
ScalarType polygonArea(const Polygon& p);

/* Perimeter */

template<typename Iterator>
auto polygonPerimeter(Iterator begin, Iterator end)
	requires PointConcept<typename Iterator::value_type>;

template<typename Polygon, typename ScalarType = double>
ScalarType polygonPerimeter(const Polygon& p);

} // namespace vcl

#include "polygon.cpp"

#endif // VCL_ALGORITHM_POLYGON_GEOMETRY_POLYGON_H
