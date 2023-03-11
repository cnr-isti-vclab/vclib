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

template<FaceConcept Polygon>
typename Polygon::VertexType::CoordType polygonNormal(const Polygon& p);

/* Barycenter */

template<FaceConcept Polygon>
typename Polygon::VertexType::CoordType polygonBarycenter(const Polygon& p);

/* Area */

template<FaceConcept Polygon>
auto polygonArea(const Polygon& p);

/* Perimeter */

template<FaceConcept Polygon>
auto polygonPerimeter(const Polygon& p);

} // namespace vcl

#include "polygon.cpp"

#endif // VCL_ALGORITHM_POLYGON_GEOMETRY_POLYGON_H
