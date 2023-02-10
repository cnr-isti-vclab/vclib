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

#ifndef VCL_MESH_ELEMENTS_FACE_COMPONENTS_OPTIONAL_H
#define VCL_MESH_ELEMENTS_FACE_COMPONENTS_OPTIONAL_H

#include "../components/vertical/optional/optional_wedge_colors.h"
#include "../components/vertical/optional/optional_wedge_tex_coords.h"

namespace vcl::face {

/* Port OptionalWedgeColors class into face namespace */
template<typename FaceType>
using OptionalPolygonWedgeColors = comp::OptionalWedgeColors<-1, FaceType>;

template<typename FaceType>
using OptionalTriangleWedgeColors = comp::OptionalWedgeColors<3, FaceType>;

/* Port OptionalWedgeTexCoords class into face namespace */
template<typename ScalarType, typename FaceType>
using OptionalPolygonWedgeTexCoords = comp::OptionalWedgeTexCoords<ScalarType, -1, FaceType>;

template<typename ScalarType, typename FaceType>
using OptionalTriangleWedgeTexCoords = comp::OptionalWedgeTexCoords<ScalarType, 3, FaceType>;

template<typename FaceType>
using OptionalPolygonWedgeTexCoordsf = comp::OptionalWedgeTexCoords<float, -1, FaceType>;
template<typename FaceType>
using OptionalPolygonWedgeTexCoordsd = comp::OptionalWedgeTexCoords<double, -1, FaceType>;
template<typename FaceType>
using OptionalTriangleWedgeTexCoordsf = comp::OptionalWedgeTexCoords<float, 3, FaceType>;
template<typename FaceType>
using OptionalTriangleWedgeTexCoordsd = comp::OptionalWedgeTexCoords<double, 3, FaceType>;

/* Port VerticalComponent class into face namespace */
template<typename FaceType>
using VerticalComponent = comp::VerticalComponent<FaceType>;

} // namespace vcl::face

#endif // VCL_MESH_ELEMENTS_FACE_COMPONENTS_OPTIONAL_H
