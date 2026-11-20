// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef BENCH_COMPAT_H
#define BENCH_COMPAT_H

// This file contains the only parts of the benchmarks that depend on the
// differences of the Mesh API between the versions of the library. Keeping them
// here allows to share the benchmark sources among the different branches.

#include "grid.h"
#include "types.h"

#include <vclib/algorithms/mesh.h>
#include <vclib/mesh.h>

namespace bench {

/**
 * @brief Enables the per-vertex adjacent faces, the per-vertex adjacent
 * vertices and the per-face adjacent faces of the given mesh.
 */
template<typename Mesh>
void enableTopologyComponents(Mesh& m)
{
    if constexpr (vcl::HasVertexOptionalAdjacentFaces<Mesh>) {
        m.enableVertexAdjacentFaces();
    }
    if constexpr (vcl::HasVertexOptionalAdjacentVertices<Mesh>) {
        m.enableVertexAdjacentVertices();
    }
    if constexpr (vcl::HasFaceOptionalAdjacentFaces<Mesh>) {
        m.enableFaceAdjacentFaces();
    }
}

template<typename Mesh>
void enableQualityComponents(Mesh& m)
{
    if constexpr (vcl::HasVertexOptionalQuality<Mesh>) {
        m.enableVertexQuality();
    }
    if constexpr (
        vcl::FaceMeshConcept<Mesh> && vcl::HasFaceOptionalQuality<Mesh>) {
        m.enableFaceQuality();
    }
}

template<typename Mesh>
void enableCurvatureComponents(Mesh& m)
{
    if constexpr (vcl::HasVertexOptionalPrincipalCurvature<Mesh>) {
        m.enableVertexPrincipalCurvature();
    }
}

template<typename Mesh>
void initQuality(Mesh& m)
{
    if constexpr (vcl::HasVertexQuality<Mesh>) {
        using QType = vcl::VertexType<Mesh>::QualityType;
        for (auto&& v : m.vertices()) {
            v.quality() = static_cast<QType>(v.index() + 1);
        }
    }
    if constexpr (vcl::FaceMeshConcept<Mesh> && vcl::HasFaceQuality<Mesh>) {
        using QType = vcl::FaceType<Mesh>::QualityType;
        for (auto&& f : m.faces()) {
            f.quality() = static_cast<QType>(f.index() + 1);
        }
    }
}

template<typename Mesh>
bool hasWedgeTexCoords(const Mesh& m)
{
    if constexpr (vcl::HasFaceWedgeTexCoords<Mesh>) {
        return m
            .template isFaceComponentAvailable<vcl::CompId::WEDGE_TEX_COORDS>();
    }
    return false;
}

/**
 * @brief Fills the given mesh with a regular triangulated grid.
 */
template<typename Mesh>
void buildGridMesh(Mesh& m, vcl::uint rows, vcl::uint cols)
{
    auto grid = makeGridMatrices(rows, cols);

    vcl::meshFromMatrices(m, grid.vertices, grid.faces);
}

} // namespace bench

#endif // BENCH_COMPAT_H
