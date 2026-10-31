// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../benchmark.h"

#include <vclib/algorithms/mesh/update/topology.h>

// The update functions clear the adjacencies before computing them, therefore
// each iteration starts from the same state. The clear functions are measured
// on meshes with the adjacencies updated (restored before each execution).

namespace {

bench::Mesh meshWithTopologyComponents(const bench::State& s)
{
    bench::Mesh m = s.copyMesh();
    bench::enableTopologyComponents(m);
    return m;
}

} // namespace

BENCH("update/topology/update_per_vertex_adjacent_faces", [](bench::State& s) {
    bench::Mesh m = meshWithTopologyComponents(s);
    s.run([&]() { vcl::updatePerVertexAdjacentFaces(m); });
});

BENCH(
    "update/topology/update_per_vertex_adjacent_vertices",
    [](bench::State& s) {
        bench::Mesh m = meshWithTopologyComponents(s);
        s.run([&]() { vcl::updatePerVertexAdjacentVertices(m); });
    });

BENCH("update/topology/update_per_face_adjacent_faces", [](bench::State& s) {
    bench::Mesh m = meshWithTopologyComponents(s);
    s.run([&]() { vcl::updatePerFaceAdjacentFaces(m); });
});

BENCH("update/topology/clear_per_vertex_adjacent_faces", [](bench::State& s) {
    bench::Mesh m = meshWithTopologyComponents(s);
    s.run(
        [&]() { vcl::updatePerVertexAdjacentFaces(m); },
        [&]() { vcl::clearPerVertexAdjacentFaces(m); });
});

BENCH(
    "update/topology/clear_per_vertex_adjacent_vertices",
    [](bench::State& s) {
        bench::Mesh m = meshWithTopologyComponents(s);
        s.run(
            [&]() { vcl::updatePerVertexAdjacentVertices(m); },
            [&]() { vcl::clearPerVertexAdjacentVertices(m); });
    });

BENCH("update/topology/clear_per_face_adjacent_faces", [](bench::State& s) {
    bench::Mesh m = meshWithTopologyComponents(s);
    s.run(
        [&]() { vcl::updatePerFaceAdjacentFaces(m); },
        [&]() { vcl::clearPerFaceAdjacentFaces(m); });
});
