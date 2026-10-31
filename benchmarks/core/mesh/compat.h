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

#include <vclib/algorithms/mesh/import_export/import_matrix.h>

namespace bench {

/**
 * @brief Enables the per-vertex adjacent faces, the per-vertex adjacent
 * vertices and the per-face adjacent faces of the given mesh.
 */
inline void enableTopologyComponents(Mesh& m)
{
    m.enablePerVertexAdjacentFaces();
    m.enablePerVertexAdjacentVertices();
    m.enablePerFaceAdjacentFaces();
}

inline bool hasWedgeTexCoords(const Mesh& m)
{
    return m.isPerFaceWedgeTexCoordsEnabled();
}

/**
 * @brief Fills the given mesh with a regular triangulated grid.
 */
inline void buildGridMesh(Mesh& m, vcl::uint rows, vcl::uint cols)
{
    auto grid = makeGridMatrices(rows, cols);
    vcl::meshFromMatrices(m, grid.vertices, grid.faces);
}

} // namespace bench

#endif // BENCH_COMPAT_H
