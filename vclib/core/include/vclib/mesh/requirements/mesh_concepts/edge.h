// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_EDGE_H
#define VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_EDGE_H

#include "base.h"

namespace vcl {

/* =========================================================================
 * Edge Component Concepts
 * ========================================================================= */

/* Normals */
template<typename M>
concept HasEdgeNormal =
    HasElementComponent<M, ElemId::EDGE, CompId::NORMAL>;

template<typename M>
concept HasEdgeOptionalNormal =
    HasElementOptionalComponent<M, ElemId::EDGE, CompId::NORMAL>;

/* Colors */
template<typename M>
concept HasEdgeColor =
    HasElementComponent<M, ElemId::EDGE, CompId::COLOR>;

template<typename M>
concept HasEdgeOptionalColor =
    HasElementOptionalComponent<M, ElemId::EDGE, CompId::COLOR>;

/* Adjacencies */
template<typename M>
concept HasEdgeAdjacentFaces =
    HasElementAdjacentFaces<M, ElemId::EDGE>;

template<typename M>
concept HasEdgeOptionalAdjacentFaces =
    HasElementOptionalAdjacentFaces<M, ElemId::EDGE>;

template<typename M>
concept HasEdgeAdjacentEdges =
    HasElementAdjacentEdges<M, ElemId::EDGE>;

template<typename M>
concept HasEdgeOptionalAdjacentEdges =
    HasElementOptionalAdjacentEdges<M, ElemId::EDGE>;

/* Quality */
template<typename M>
concept HasEdgeQuality =
    HasElementComponent<M, ElemId::EDGE, CompId::QUALITY>;

template<typename M>
concept HasEdgeOptionalQuality =
    HasElementOptionalComponent<M, ElemId::EDGE, CompId::QUALITY>;

/* Custom Components */
template<typename M>
concept HasEdgeCustomComponents =
    HasElementCustomComponents<M, ElemId::EDGE>;

} // namespace vcl

#endif // VCL_MESH_REQUIREMENTS_MESH_CONCEPTS_EDGE_H
