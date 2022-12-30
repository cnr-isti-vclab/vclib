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

#include "../components/vertical/persistent/custom_components.h"
#include "../components/vertical/optional/optional_adjacent_edges.h"
#include "../components/vertical/optional/optional_adjacent_faces.h"
#include "../components/vertical/optional/optional_color.h"
#include "../components/vertical/optional/optional_mark.h"
#include "../components/vertical/optional/optional_normal.h"
#include "../components/vertical/optional/optional_principal_curvature.h"
#include "../components/vertical/optional/optional_scalar.h"
#include "../components/vertical/optional/optional_wedge_colors.h"
#include "../components/vertical/optional/optional_wedge_tex_coords.h"

namespace vcl::face {

/* Port OptionalAdjacentEdges class into face namespace */
template<typename EdgeType, typename FaceType>
using OptionalAdjacentPolygonEdges = comp::OptionalAdjacentEdges<EdgeType, -1, FaceType>;

template<typename EdgeType, typename FaceType>
using OptionalAdjacentTriangleEdges = comp::OptionalAdjacentEdges<EdgeType, 3, FaceType>;

/* Port OptionalAdjacentFaces class into face namespace */
template<typename FaceType>
using OptionalAdjacentPolygons = comp::OptionalAdjacentFaces<FaceType, -1, FaceType>;

template<typename FaceType>
using OptionalAdjacentTriangles = comp::OptionalAdjacentFaces<FaceType, 3, FaceType>;

/* Port OptionalColor class into face namespace */
template<typename FaceType>
using OptionalColor = comp::OptionalColor<FaceType>;

/* Port OptionalMark class into face namespace */
template<typename FaceType>
using OptionalMark = comp::OptionalMark<FaceType>;

/* Port OptionalNormal class into face namespace */
template<typename ScalarType, int N, typename FaceType>
using OptionalNormal = comp::OptionalNormal<ScalarType, N, FaceType>;

template<typename ScalarType, typename FaceType>
using OptionalNormal3 = comp::OptionalNormal3<ScalarType, FaceType>;

template<typename FaceType>
using OptionalNormal3f = comp::OptionalNormal3f<FaceType>;

template<typename FaceType>
using OptionalNormal3d = comp::OptionalNormal3d<FaceType>;

/* Port OptionalPrincipalCurvature class into face namespace */
template<typename ScalarType, typename FaceType>
using OptionalPrincipalCurvature = comp::OptionalPrincipalCurvature<ScalarType, FaceType>;

template<typename FaceType>
using OptionalPrincipalCurvaturef = comp::OptionalPrincipalCurvaturef<FaceType>;
template<typename FaceType>
using OptionalPrincipalCurvatured = comp::OptionalPrincipalCurvatured<FaceType>;

/* Port OptionalScalar class into face namespace */
template<typename ScalarType, typename FaceType>
using OptionalScalar = comp::OptionalScalar<ScalarType, FaceType>;

template<typename FaceType>
using OptionalScalarf = comp::OptionalScalar<float, FaceType>;

template<typename FaceType>
using OptionalScalard = comp::OptionalScalar<double, FaceType>;

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

/* Port CustomComponents class into face namespace */
template<typename FaceType>
using CustomComponents = comp::CustomComponents<FaceType>;

/* Port VerticalComponent class into face namespace */
template<typename FaceType>
using VerticalComponent = comp::VerticalComponent<FaceType>;

} // namespace vcl::face

#endif // VCL_MESH_ELEMENTS_FACE_COMPONENTS_OPTIONAL_H
