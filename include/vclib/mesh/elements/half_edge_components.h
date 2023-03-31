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

#ifndef VCL_MESH_ELEMENTS_HALF_EDGE_COMPONENTS_H
#define VCL_MESH_ELEMENTS_HALF_EDGE_COMPONENTS_H

#include "../components/bit_flags.h"
#include "../components/color.h"
#include "../components/custom_components.h"
#include "../components/half_edge_references.h"
#include "../components/mark.h"
#include "../components/normal.h"
#include "../components/parent_mesh_pointer.h"
#include "../components/scalar.h"
#include "../components/tex_coord.h"

namespace vcl::hedge {

/* Port BitFlags class into hedge namespace */
using BitFlags = comp::BitFlags<>;

template<typename HEdgeType>
using VerticalBitFlags = comp::BitFlags<HEdgeType>;

/* Port Color class into hedge namespace */
using Color = comp::Color<>;

template<typename HEdgeType>
using VerticalColor = comp::Color<HEdgeType>; 

template<typename HEdgeType>
using OptionalColor = comp::Color<HEdgeType, true>;

/* Port CustomComponents class into hedge namespace */
template<typename HEdgeType>
using CustomComponents = comp::CustomComponents<HEdgeType>;

/* Port HalfEdgeReferences class into hedge namespace */
template<typename HalfEdgeType, typename VertexType, typename FaceType>
using HalfEdgeReferences = comp::HalfEdgeReferences<HalfEdgeType, VertexType, FaceType>;

/* Port Mark class into hedge namespace */
using Mark = comp::Mark<>;

template<typename HEdgeType>
using VerticalMark = comp::Mark<HEdgeType>;

template<typename HEdgeType>
using OptionalMark = comp::Mark<HEdgeType, true>;

/* Port ParentMeshPointer class into vert namespace */
template<typename MeshType>
using ParentMeshPointer = comp::ParentMeshPointer<MeshType>;

/* Port Scalar class into hedge namespace */
template<typename ScalarType>
using Scalar = comp::Scalar<ScalarType>;

using Scalarf = comp::Scalarf<>;
using Scalard = comp::Scalard<>;

template<typename ScalarType, typename HEdgeType>
using VerticalScalar = comp::Scalar<ScalarType, HEdgeType>;

template<typename HEdgeType>
using VerticalScalarf = comp::Scalar<float, HEdgeType>;

template<typename HEdgeType>
using VerticalScalard = comp::Scalar<double, HEdgeType>;

template<typename ScalarType, typename HEdgeType>
using OptionalScalar = comp::Scalar<ScalarType, HEdgeType, true>;

template<typename HEdgeType>
using OptionalScalarf = comp::Scalar<float, HEdgeType, true>;

template<typename HEdgeType>
using OptionalScalard = comp::Scalar<double, HEdgeType, true>;

/* Port TexCoord class into hedge namespace */
template<typename ScalarType>
using TexCoord = comp::TexCoord<ScalarType>;

using TexCoordf = comp::TexCoordf<>;
using TexCoordd = comp::TexCoordd<>;

template<typename ScalarType, typename HEdgeType>
using VerticalTexCoord = comp::TexCoord<ScalarType, HEdgeType>;

template<typename HEdgeType>
using VerticalTexCoordf = comp::TexCoord<float, HEdgeType>;

template<typename HEdgeType>
using VerticalTexCoordd = comp::TexCoord<double, HEdgeType>;

template<typename ScalarType, typename HEdgeType>
using OptionalTexCoord = comp::TexCoord<ScalarType, HEdgeType, true>;

template<typename HEdgeType>
using OptionalTexCoordf = comp::TexCoord<float, HEdgeType, true>;

template<typename HEdgeType>
using OptionalTexCoordd = comp::TexCoord<double, HEdgeType, true>;

} // namespace vcl::hedge

#endif // VCL_MESH_ELEMENTS_HALF_EDGE_COMPONENTS_H
