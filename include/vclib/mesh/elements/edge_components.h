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
#include "../components/vertex_indices.h"
#include "../components/vertex_references.h"
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
template<typename ParentEdgeType>
using AdjacentEdgePointers =
    comp::AdjacentEdgePointers<ParentEdgeType, -1, false, ParentEdgeType>;

template<typename ParentEdgeType>
using VerticalAdjacentEdgePointers =
    comp::AdjacentEdgePointers<ParentEdgeType, -1, false, ParentEdgeType, true>;

template<typename ParentEdgeType>
using OptionalAdjacentEdgePointers = comp::
    AdjacentEdgePointers<ParentEdgeType, -1, false, ParentEdgeType, true, true>;

/* Port AdjacentFaces class into edge namespace */
template<typename FaceType, typename ParentEdgeType>
using AdjacentFacePointers =
    comp::AdjacentFacePointers<FaceType, -1, false, ParentEdgeType>;

template<typename FaceType, typename ParentEdgeType>
using VerticalAdjacentFacePointers =
    comp::AdjacentFacePointers<FaceType, -1, false, ParentEdgeType, true>;

template<typename FaceType, typename ParentEdgeType>
using OptionalAdjacentFacePointers =
    comp::AdjacentFacePointers<FaceType, -1, false, ParentEdgeType, true, true>;

/* Port BitFlags class into edge namespace */
using BitFlags = comp::BitFlags<>;

/* Port Color class into edge namespace */
using Color = comp::Color<>;

template<typename VertexType>
using VerticalColor = comp::Color<VertexType>;

template<typename ParentEdgeType>
using OptionalColor = comp::Color<ParentEdgeType, true>;

/* Port CustomComponents class into edge namespace */
template<typename ParentEdgeType>
using CustomComponents = comp::CustomComponents<ParentEdgeType>;

/* Port Mark class into edge namespace */
using Mark = comp::Mark<>;

template<typename ParentEdgeType>
using VerticalMark = comp::Mark<ParentEdgeType>;

template<typename ParentEdgeType>
using OptionalMark = comp::Mark<ParentEdgeType, true>;

/* Port ParentMeshPointer class into vert namespace */
template<typename MeshType>
using ParentMeshPointer = comp::ParentMeshPointer<MeshType>;

/* Port Quality class into edge namespace */
template<typename QualityType>
using Quality = comp::Quality<QualityType>;

using Qualityf = comp::Qualityf<>;
using Qualityd = comp::Qualityd<>;

template<typename QualityType, typename ParentEdgeType>
using VerticalQuality = comp::Quality<QualityType, ParentEdgeType>;

template<typename ParentEdgeType>
using VerticalQualityf = comp::Quality<float, ParentEdgeType>;

template<typename ParentEdgeType>
using VerticalQualityd = comp::Quality<double, ParentEdgeType>;

template<typename QualityType, typename ParentEdgeType>
using OptionalQuality = comp::Quality<QualityType, ParentEdgeType, true>;

template<typename ParentEdgeType>
using OptionalQualityf = comp::Quality<float, ParentEdgeType, true>;

template<typename ParentEdgeType>
using OptionalQualityd = comp::Quality<double, ParentEdgeType, true>;

/* Port VertexIndices class into edge namespace */
template<typename VertexType, typename EdgeType>
using VertexIndices = comp::VertexIndices<VertexType, 2, EdgeType>;

/* Port VertexReferences alias into edge namespace */
template<bool INDEXED, typename VertexType, typename EdgeType>
using VertexReferences =
    comp::VertexReferences<INDEXED, VertexType, 2, EdgeType>;

/* Port VertexPointers class into edge namespace */
template<typename VertexType, typename EdgeType>
using VertexPointers = comp::VertexPointers<VertexType, 2, EdgeType>;

/** @} */ // end of edge_components group

} // namespace vcl::edge

#endif // VCL_MESH_ELEMENTS_EDGE_COMPONENTS_H
