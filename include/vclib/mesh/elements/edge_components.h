/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_MESH_ELEMENTS_EDGE_COMPONENTS_H
#define VCL_MESH_ELEMENTS_EDGE_COMPONENTS_H

#include "../components/adjacent_edge_pointers.h"
#include "../components/adjacent_face_pointers.h"
#include "../components/bit_flags.h"
#include "../components/color.h"
#include "../components/custom_components.h"
#include "../components/mark.h"
#include "../components/parent_mesh_pointer.h"
#include "../components/quality.h"
#include "../components/vertex_pointers.h"

/**
 * @defgroup edge_components Edge Components
 * @ingroup components
 *
 * @brief This group lists all the Components that may be used by the Edge
 * element.
 *
 * All these components are listed inside the vcl::edge namespace.
 */

namespace vcl::edge {

/**
 * @addtogroup edge_components
 * @{
 */

/* Port AdjacentEdges class into edge namespace */
template<typename EdgeType>
using AdjacentEdgePointers =
    comp::AdjacentEdgePointers<EdgeType, -1, false, EdgeType>;

template<typename EdgeType>
using VerticalAdjacentEdgePointers =
    comp::AdjacentEdgePointers<EdgeType, -1, false, EdgeType, true>;

template<typename EdgeType>
using OptionalAdjacentEdgePointers =
    comp::AdjacentEdgePointers<EdgeType, -1, false, EdgeType, true, true>;

/* Port AdjacentFaces class into edge namespace */
template<typename FaceType, typename EdgeType>
using AdjacentFacePointers =
    comp::AdjacentFacePointers<FaceType, -1, false, EdgeType>;

template<typename FaceType, typename EdgeType>
using VerticalAdjacentFacePointers =
    comp::AdjacentFacePointers<FaceType, -1, false, EdgeType, true>;

template<typename FaceType, typename EdgeType>
using OptionalAdjacentFacePointers =
    comp::AdjacentFacePointers<FaceType, -1, false, EdgeType, true, true>;

/* Port BitFlags class into edge namespace */
using BitFlags = comp::BitFlags<>;

/* Port Color class into edge namespace */
using Color = comp::Color<>;

template<typename VertexType>
using VerticalColor = comp::Color<VertexType>;

template<typename EdgeType>
using OptionalColor = comp::Color<EdgeType, true>;

/* Port CustomComponents class into edge namespace */
template<typename EdgeType>
using CustomComponents = comp::CustomComponents<EdgeType>;

/* Port Mark class into edge namespace */
using Mark = comp::Mark<>;

template<typename EdgeType>
using VerticalMark = comp::Mark<EdgeType>;

template<typename EdgeType>
using OptionalMark = comp::Mark<EdgeType, true>;

/* Port ParentMeshPointer class into vert namespace */
template<typename MeshType>
using ParentMeshPointer = comp::ParentMeshPointer<MeshType>;

/* Port Quality class into edge namespace */
template<typename QualityType>
using Quality = comp::Quality<QualityType>;

using Qualityf = comp::Qualityf<>;
using Qualityd = comp::Qualityd<>;

template<typename QualityType, typename EdgeType>
using VerticalQuality = comp::Quality<QualityType, EdgeType>;

template<typename EdgeType>
using VerticalQualityf = comp::Quality<float, EdgeType>;

template<typename EdgeType>
using VerticalQualityd = comp::Quality<double, EdgeType>;

template<typename QualityType, typename EdgeType>
using OptionalQuality = comp::Quality<QualityType, EdgeType, true>;

template<typename EdgeType>
using OptionalQualityf = comp::Quality<float, EdgeType, true>;

template<typename EdgeType>
using OptionalQualityd = comp::Quality<double, EdgeType, true>;

/* Port VertexPointers class into edge namespace */
template<typename VertexType, typename EdgeType>
using VertexPointers = comp::VertexPointers<VertexType, 2, EdgeType>;

/** @} */ // end of edge_components group

} // namespace vcl::edge

#endif // VCL_MESH_ELEMENTS_EDGE_COMPONENTS_H
