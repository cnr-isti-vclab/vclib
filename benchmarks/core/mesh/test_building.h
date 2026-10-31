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

#include <vclib/algorithms.h>

#include <Eigen/Core>

namespace bench {

/**
 * @brief Benchmarks the conversions between a mesh and the V/F matrices.
 *
 * The matrices are obtained from the given (already loaded) mesh, which is
 * never modified: the meshes built from the matrices are discarded.
 */
inline void benchmarkBuilding(
    BenchmarkRunner& runner,
    const Mesh&      m,
    int              iterations = -1)
{
    Eigen::MatrixX3d V;
    Eigen::MatrixX3i F;

    runner.run(
        "export_matrices",
        [&]() {
            V = vcl::vertexPositionsMatrix<Eigen::MatrixX3d>(m);
            F = vcl::faceVertexIndicesMatrix<Eigen::MatrixX3i>(m);
        },
        iterations);

    runner.run(
        "import_matrices",
        [&]() {
            Mesh newM;
            vcl::meshFromMatrices(newM, V, F);
            doNotOptimize(newM);
        },
        iterations);
}

} // namespace bench

#endif // TEST_BUILDING_H
