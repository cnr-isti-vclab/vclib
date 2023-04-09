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

#ifndef VCL_ALGORITHMS_POLYGON_GEOMETRY_H
#define VCL_ALGORITHMS_POLYGON_GEOMETRY_H

#include <vclib/mesh/requirements.h>

namespace vcl {

template<FaceConcept FaceType>
typename FaceType::VertexType::CoordType faceNormal(const FaceType& f);

template<FaceConcept FaceType>
typename FaceType::VertexType::CoordType faceBarycenter(const FaceType& f);

template<FaceConcept FaceType>
auto faceArea(const FaceType& f);

template<FaceConcept FaceType>
auto facePerimeter(const FaceType& f);

template<FaceConcept FaceType>
auto faceAngleOnVertexRad(const FaceType& f, uint v);

template<FaceConcept FaceType>
auto faceDihedralAngleOnEdge(const FaceType& f, uint e) requires comp::HasAdjacentFaces<FaceType>;

} // namespace vcl

#include "geometry.cpp"

#endif // VCL_ALGORITHMS_POLYGON_GEOMETRY_H
