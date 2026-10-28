// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_BUILDING_H
#define TEST_BUILDING_H

#include "types.h"

#include <benchmark_runner.h>

#include <Eigen/Core>

namespace bench {

inline void buildMesh(
    Mesh&                   m,
    const Eigen::MatrixX3d& V,
    const Eigen::MatrixX3i& F)
{
    m.clear();
    const auto numV = V.rows();
    m.resizeVertices(numV);
    for (int i = 0; i < numV; ++i) {
        m.vertex(i).position() = {V(i, 0), V(i, 1), V(i, 2)};
    }

    const auto numF = F.rows();
    m.resizeFaces(numF);
    for (int i = 0; i < numF; ++i) {
        m.face(i).setVertex(0, F(i, 0));
        m.face(i).setVertex(1, F(i, 1));
        m.face(i).setVertex(2, F(i, 2));
    }
}

inline void benchmarkBuilding(
    BenchmarkRunner&        runner,
    const Eigen::MatrixX3d& V,
    const Eigen::MatrixX3i& F,
    Mesh&                   outM,
    int                     iterations = -1)
{
    runner.run(
        "mesh_building",
        [&]() {
            Mesh m;
            buildMesh(m, V, F);
        },
        iterations);

    buildMesh(outM, V, F);
}

} // namespace bench

#endif // TEST_BUILDING_H
