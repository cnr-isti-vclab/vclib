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

#ifndef VCL_MESH_ELEMENTS_VERTEX_COMPONENTS_H
#define VCL_MESH_ELEMENTS_VERTEX_COMPONENTS_H

#include "../components/adjacent_edges.h"
#include "../components/adjacent_faces.h"
#include "../components/adjacent_vertices.h"
#include "../components/bit_flags.h"
#include "../components/color.h"
#include "../components/coordinate.h"
#include "../components/mark.h"
#include "../components/normal.h"
#include "../components/parent_mesh_pointer.h"
#include "../components/principal_curvature.h"
#include "../components/scalar.h"
#include "../components/vertex_half_edge_reference.h"
#include "../components/tex_coord.h"

namespace vcl::vert {

/* Port AdjacentEdges class into vert namespace */
template<typename Edge>
using AdjacentEdges = comp::AdjacentEdges<Edge, -1>;

/* Port AdjacentFaces class into vert namespace */
template<typename FaceType>
using AdjacentFaces = comp::AdjacentFaces<FaceType, -1>;

/* Port AdjacentVertices class into vert namespace */
template<typename VertexType>
using AdjacentVertices = comp::AdjacentVertices<VertexType>;

/* Port BitFlags class into vert namespace */
using BitFlags = comp::BitFlags<void*, true>;

/* Port Color class into vert namespace */
using Color = comp::Color<void*, true>;

/* Port Coordinate class into vert namespace */
template<typename ScalarType, int N>
using Coordinate = comp::Coordinate<ScalarType, N, void*, true>;

template<typename ScalarType>
using Coordinate3 = comp::Coordinate3<ScalarType, void*, true>;

using Coordinate3f = comp::Coordinate3f<void*, true>;
using Coordinate3d = comp::Coordinate3d<void*, true>;

/* Port Half Edge reference into vert namespace*/
template<typename HalfEdge>
using HalfEdgeReference = comp::VertexHalfEdgeReference<HalfEdge>;

/* Port Mark class into vert namespace */
using Mark = comp::Mark<void*, true>;

/* Port Normal classes into vert namespace */
template<typename ScalarType, int N>
using Normal = comp::Normal<ScalarType, N, void*, true>;

template<typename ScalarType>
using Normal3 = comp::Normal3<ScalarType, void*, true>;

using Normal3f = comp::Normal3f<void*, true>;
using Normal3d = comp::Normal3d<void*, true>;

/* Port ParentMeshPointer class into vert namespace */
template<typename MeshType>
using ParentMeshPointer = comp::ParentMeshPointer<MeshType>;

/* Port PrincipalCurvature class into vert namespace */
template<typename ScalarType>
using PrincipalCurvature = comp::PrincipalCurvature<ScalarType>;

using PrincipalCurvaturef = comp::PrincipalCurvaturef;
using PrincipalCurvatured = comp::PrincipalCurvatured;

/* Port Scalar class into vert namespace */
template<typename ScalarType>
using Scalar = comp::Scalar<ScalarType>;

using Scalarf = comp::Scalarf;
using Scalard = comp::Scalard;

/* Port TexCoord class into vert namespace */
template<typename ScalarType>
using TexCoord = comp::TexCoord<ScalarType>;

using TexCoordf = comp::TexCoordf;
using TexCoordd = comp::TexCoordd;

} // namespace vcl::vert

#endif // VCL_MESH_ELEMENTS_VERTEX_COMPONENTS_H
