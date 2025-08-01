/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#include "../components/adjacent_edges.h"
#include "../components/adjacent_faces.h"
#include "../components/bit_flags.h"
#include "../components/color.h"
#include "../components/custom_components.h"
#include "../components/mark.h"
#include "../components/normal.h"
#include "../components/parent_mesh_pointer.h"
#include "../components/quality.h"
#include "../components/vertex_references.h"

/**
 * @defgroup edge_components Edge Components
 * @ingroup components
 *
 * @brief This group lists all the Components that may be used by the Edge
 * element.
 *
 * All these components are listed inside the vcl::edge namespace.
 */

/**
 * @defgroup edge_concepts Edge Concepts
 * @ingroup element_concepts
 *
 * @brief List of concepts for types related to the Edge Element.
 */

namespace vcl::edge {

/**
 * @ingroup edge_concepts edge_components
 * @{
 */

/* Port concepts into the edge namespace */
template<typename T>
concept HasAdjacentEdges = comp::HasAdjacentEdges<T>;
template<typename T>
concept HasOptionalAdjacentEdges = comp::HasOptionalAdjacentEdges<T>;
template<typename T>
concept HasAdjacentFaces = comp::HasAdjacentFaces<T>;
template<typename T>
concept HasOptionalAdjacentFaces = comp::HasOptionalAdjacentFaces<T>;
template<typename T>
concept HasBitFlags = comp::HasBitFlags<T>;
template<typename T>
concept HasColor = comp::HasColor<T>;
template<typename T>
concept HasOptionalColor = comp::HasOptionalColor<T>;
template<typename T>
concept HasCustomComponents = comp::HasCustomComponents<T>;
template<typename T>
concept HasMark = comp::HasMark<T>;
template<typename T>
concept HasOptionalMark = comp::HasOptionalMark<T>;
template<typename T>
concept HasNormal = comp::HasNormal<T>;
template<typename T>
concept HasOptionalNormal = comp::HasOptionalNormal<T>;
template<typename T>
concept HasQuality = comp::HasQuality<T>;
template<typename T>
concept HasOptionalQuality = comp::HasOptionalQuality<T>;
template<typename T>
concept HasVertexReferences = comp::HasVertexReferences<T>;

/** @} */ // end of group

/**
 * @addtogroup edge_components
 * @{
 */

/* Port AdjacentEdges class into edge namespace */
template<bool INDEXED, typename ParentEdgeType>
using AdjacentEdges =
    comp::AdjacentEdges<INDEXED, ParentEdgeType, -1, false, ParentEdgeType>;

template<bool INDEXED, typename ParentEdgeType>
using VerticalAdjacentEdges = comp::
    AdjacentEdges<INDEXED, ParentEdgeType, -1, false, ParentEdgeType, true>;

template<bool INDEXED, typename ParentEdgeType>
using OptionalAdjacentEdges = comp::AdjacentEdges<
    INDEXED,
    ParentEdgeType,
    -1,
    false,
    ParentEdgeType,
    true,
    true>;

template<typename ParentEdgeType>
using AdjacentEdgeIndices =
    comp::AdjacentEdges<true, ParentEdgeType, -1, false, ParentEdgeType>;

template<typename ParentEdgeType>
using VerticalAdjacentEdgeIndices =
    comp::AdjacentEdges<true, ParentEdgeType, -1, false, ParentEdgeType, true>;

template<typename ParentEdgeType>
using OptionalAdjacentEdgeIndices = comp::
    AdjacentEdges<true, ParentEdgeType, -1, false, ParentEdgeType, true, true>;

template<typename ParentEdgeType>
using AdjacentEdgePointers =
    comp::AdjacentEdges<false, ParentEdgeType, -1, false, ParentEdgeType>;

template<typename ParentEdgeType>
using VerticalAdjacentEdgePointers =
    comp::AdjacentEdges<false, ParentEdgeType, -1, false, ParentEdgeType, true>;

template<typename ParentEdgeType>
using OptionalAdjacentEdgePointers = comp::
    AdjacentEdges<false, ParentEdgeType, -1, false, ParentEdgeType, true, true>;

/* Port AdjacentFaces class into edge namespace */
template<bool INDEXED, typename FaceType, typename ParentEdgeType>
using AdjacentFaces =
    comp::AdjacentFaces<INDEXED, FaceType, -1, false, ParentEdgeType>;

template<bool INDEXED, typename FaceType, typename ParentEdgeType>
using VerticalAdjacentFaces =
    comp::AdjacentFaces<INDEXED, FaceType, -1, false, ParentEdgeType, true>;

template<bool INDEXED, typename FaceType, typename ParentEdgeType>
using OptionalAdjacentFaces = comp::
    AdjacentFaces<INDEXED, FaceType, -1, false, ParentEdgeType, true, true>;

template<typename FaceType, typename ParentEdgeType>
using AdjacentFaceIndices =
    comp::AdjacentFaces<true, FaceType, -1, false, ParentEdgeType>;

template<typename FaceType, typename ParentEdgeType>
using VerticalAdjacentFaceIndices =
    comp::AdjacentFaces<true, FaceType, -1, false, ParentEdgeType, true>;

template<typename FaceType, typename ParentEdgeType>
using OptionalAdjacentFaceIndices =
    comp::AdjacentFaces<true, FaceType, -1, false, ParentEdgeType, true, true>;

template<typename FaceType, typename ParentEdgeType>
using AdjacentFacePointers =
    comp::AdjacentFaces<false, FaceType, -1, false, ParentEdgeType>;

template<typename FaceType, typename ParentEdgeType>
using VerticalAdjacentFacePointers =
    comp::AdjacentFaces<false, FaceType, -1, false, ParentEdgeType, true>;

template<typename FaceType, typename ParentEdgeType>
using OptionalAdjacentFacePointers =
    comp::AdjacentFaces<false, FaceType, -1, false, ParentEdgeType, true, true>;

/* Port BitFlags class into edge namespace */
using BitFlags = comp::BitFlags<>;

/* Port Color class into edge namespace */
using Color = comp::Color<>;

template<typename ParentEdgeType>
using VerticalColor = comp::Color<ParentEdgeType>;

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

/* Port Normal classes into edge namespace */
template<typename ScalarType, int N>
using Normal = comp::Normal<Point<ScalarType, N>>;

template<typename ScalarType>
using Normal3 = comp::Normal3<ScalarType>;

using Normal3f = comp::Normal3f<>;
using Normal3d = comp::Normal3d<>;

template<typename ScalarType, int N, typename ParentEdgeType>
using VerticalNormal = comp::Normal<Point<ScalarType, N>, ParentEdgeType>;

template<typename ScalarType, typename ParentEdgeType>
using VerticalNormal3 = comp::Normal3<ScalarType, ParentEdgeType>;

template<typename ParentEdgeType>
using VerticalNormal3f = comp::Normal3f<ParentEdgeType>;

template<typename ParentEdgeType>
using VerticalNormal3d = comp::Normal3d<ParentEdgeType>;

template<typename ScalarType, int N, typename ParentEdgeType>
using OptionalNormal = comp::Normal<Point<ScalarType, N>, ParentEdgeType, true>;

template<typename ScalarType, typename ParentEdgeType>
using OptionalNormal3 = comp::Normal3<ScalarType, ParentEdgeType, true>;

template<typename ParentEdgeType>
using OptionalNormal3f = comp::Normal3f<ParentEdgeType, true>;

template<typename ParentEdgeType>
using OptionalNormal3d = comp::Normal3d<ParentEdgeType, true>;

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

/* Port VertexReferences alias into edge namespace */
template<bool INDEXED, typename VertexType, typename EdgeType>
using VertexReferences =
    comp::VertexReferences<INDEXED, VertexType, 2, EdgeType>;

template<typename VertexType, typename EdgeType>
using VertexIndices = comp::VertexReferences<true, VertexType, 2, EdgeType>;

template<typename VertexType, typename EdgeType>
using VertexPointers = comp::VertexReferences<false, VertexType, 2, EdgeType>;

/** @} */ // end of edge_components group

} // namespace vcl::edge

#endif // VCL_MESH_ELEMENTS_EDGE_COMPONENTS_H
