// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_ELEMENTS_EDGE_COMPONENTS_H
#define VCL_MESH_ELEMENTS_EDGE_COMPONENTS_H

#include "../components/adjacent_edges.h"
#include "../components/adjacent_faces.h"
#include "../components/bit_flags.h"
#include "../components/color.h"
#include "../components/custom_components.h"
#include "../components/normal.h"
#include "../components/quality.h"
#include "../components/vertex_references.h"

namespace vcl::edge {

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
concept HasNormal = comp::HasNormal<T>;
template<typename T>
concept HasOptionalNormal = comp::HasOptionalNormal<T>;
template<typename T>
concept HasQuality = comp::HasQuality<T>;
template<typename T>
concept HasOptionalQuality = comp::HasOptionalQuality<T>;
template<typename T>
concept HasVertexReferences = comp::HasVertexReferences<T>;

/* Port AdjacentEdges class into edge namespace */
template<std::unsigned_integral Index = uint>
using AdjacentEdges = comp::AdjacentEdges<-1, Index>;

template<std::unsigned_integral Index = uint>
using OptionalAdjacentEdges = comp::AdjacentEdges<-1, Index, false, true>;

/* Port AdjacentFaces class into edge namespace */
template<std::unsigned_integral Index = uint>
using AdjacentFaces = comp::AdjacentFaces<-1, Index>;

template<std::unsigned_integral Index = uint>
using OptionalAdjacentFaces = comp::AdjacentFaces<-1, Index, false, true>;

/* Port BitFlags class into edge namespace */
using BitFlags = comp::BitFlags;

/* Port Color class into edge namespace */
using Color         = comp::Color<false>;
using OptionalColor = comp::OptionalColor;

/* Port CustomComponents class into edge namespace */
using CustomComponents = comp::CustomComponents;

/* Port Normal classes into edge namespace */
template<typename ScalarType, int N, bool OPT = false>
using Normal = comp::Normal<Point<ScalarType, N>, OPT>;

template<typename ScalarType, bool OPT = false>
using Normal3 = comp::Normal3<ScalarType, OPT>;

using Normal3f         = comp::Normal3f;
using Normal3d         = comp::Normal3d;
template<typename ScalarType>
using OptionalNormal3  = comp::OptionalNormal3<ScalarType>;
using OptionalNormal3f = comp::OptionalNormal3f;
using OptionalNormal3d = comp::OptionalNormal3d;

/* Port Quality classes into edge namespace */
template<typename ScalarType, bool OPT = false>
using Quality = comp::Quality<ScalarType, OPT>;

template<typename ScalarType>
using OptionalQuality = comp::Quality<ScalarType, true>;

using Qualityf         = comp::Qualityf<false>;
using Qualityd         = comp::Qualityd<false>;
using OptionalQualityf = comp::Qualityf<true>;
using OptionalQualityd = comp::Qualityd<true>;

/* Port VertexReferences class and aliases into edge namespace */
template<std::unsigned_integral Index = uint>
using VertexReferences = comp::VertexReferences<2, Index>;

} // namespace vcl::edge

#endif // VCL_MESH_ELEMENTS_EDGE_COMPONENTS_H
