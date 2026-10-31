// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_TOPOLOGY_H
#define TEST_TOPOLOGY_H

#include "types.h"

#include <benchmark_runner.h>

#include <vclib/algorithms.h>

namespace bench {

/**
 * @brief Benchmarks the topology update algorithms.
 *
 * The benchmarks run on a copy of the given mesh, so that the adjacency
 * components enabled here do not affect the other benchmarks. The update
 * functions clear the adjacencies before computing them, therefore each
 * iteration starts from the same state.
 */
inline void benchmarkTopology(
    BenchmarkRunner& runner,
    const Mesh&      m,
    int              iterations = -1)
{
    Mesh mesh = m;

    mesh.enablePerVertexAdjacentFaces();
    mesh.enablePerVertexAdjacentVertices();
    mesh.enablePerFaceAdjacentFaces();

    runner.run(
        "update_vertex_adj_faces",
        [&]() { vcl::updatePerVertexAdjacentFaces(mesh); },
        iterations);

    runner.run(
        "update_vertex_adj_vertices",
        [&]() { vcl::updatePerVertexAdjacentVertices(mesh); },
        iterations);

    runner.run(
        "update_face_adj_faces",
        [&]() { vcl::updatePerFaceAdjacentFaces(mesh); },
        iterations);

    runner.run(
        "clear_vertex_adj_faces",
        [&]() { vcl::clearPerVertexAdjacentFaces(mesh); },
        iterations);

    runner.run(
        "clear_vertex_adj_vertices",
        [&]() { vcl::clearPerVertexAdjacentVertices(mesh); },
        iterations);

    runner.run(
        "clear_face_adj_faces",
        [&]() { vcl::clearPerFaceAdjacentFaces(mesh); },
        iterations);
}

} // namespace bench

#endif // TEST_TOPOLOGY_H
