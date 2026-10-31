// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../benchmark.h"

#include <vclib/algorithms/mesh/import_export/export_matrix.h>
#include <vclib/algorithms/mesh/import_export/import_matrix.h>

#include <Eigen/Core>

BENCH("import_export/import_matrix/mesh_from_matrices", [](bench::State& s) {
    const Eigen::MatrixX3d V =
        vcl::vertexPositionsMatrix<Eigen::MatrixX3d>(s.mesh());
    const Eigen::MatrixX3i F =
        vcl::faceVertexIndicesMatrix<Eigen::MatrixX3i>(s.mesh());

    s.run([&]() { return vcl::meshFromMatrices<bench::Mesh>(V, F); });
});
