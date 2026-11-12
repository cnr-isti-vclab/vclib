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

namespace bench {

/**
 * @brief Enables the per-vertex adjacent faces, the per-vertex adjacent
 * vertices and the per-face adjacent faces of the given mesh.
 */
template<typename Mesh>
void enableTopologyComponents(Mesh& m)
{
    m.enablePerVertexAdjacentFaces();
    m.enablePerVertexAdjacentVertices();
    m.enablePerFaceAdjacentFaces();
}

template<typename Mesh>
void enableQualityComponents(Mesh& m)
{
    if constexpr (vcl::HasPerVertexQuality<Mesh>) {
        if constexpr (requires { m.enablePerVertexQuality(); }) {
            m.enablePerVertexQuality();
        }
    }
    if constexpr (vcl::FaceMeshConcept<Mesh> && vcl::HasPerFaceQuality<Mesh>) {
        if constexpr (requires { m.enablePerFaceQuality(); }) {
            m.enablePerFaceQuality();
        }
    }
}

template<typename Mesh>
void enableCurvatureComponents(Mesh& m)
{
    if constexpr (requires { m.enablePerVertexPrincipalCurvature(); }) {
        m.enablePerVertexPrincipalCurvature();
    }
    else if constexpr (requires { m.enableVertexPrincipalCurvature(); }) {
        m.enableVertexPrincipalCurvature();
    }
}

template<typename Mesh>
void initQuality(Mesh& m)
{
    if constexpr (vcl::HasPerVertexQuality<Mesh>) {
        for (auto& v : m.vertices()) {
            v.quality() = static_cast<typename Mesh::VertexType::QualityType>(
                m.index(v) + 1);
        }
    }
    if constexpr (vcl::FaceMeshConcept<Mesh> && vcl::HasPerFaceQuality<Mesh>) {
        for (auto& f : m.faces()) {
            f.quality() = static_cast<typename Mesh::FaceType::QualityType>(
                m.index(f) + 1);
        }
    }
}

template<typename Mesh>
bool hasWedgeTexCoords(const Mesh& m)
{
    return m.isPerFaceWedgeTexCoordsEnabled();
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
