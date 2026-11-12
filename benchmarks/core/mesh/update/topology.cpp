// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../benchmark.h"

#include <vclib/algorithms/mesh.h>

// The update functions clear the adjacencies before computing them, therefore
// each iteration starts from the same state. The clear functions are measured
// on meshes with the adjacencies updated (restored before each execution).

namespace {

template<typename Mesh>
Mesh meshWithTopologyComponents(const bench::State<Mesh>& s)
{
    Mesh m = s.copyMesh();
    bench::enableTopologyComponents(m);
    return m;
}

} // namespace

BENCH(
    "update/topology/update_vertex_adjacent_faces",
    []<typename Mesh>(bench::State<Mesh>& s) {
        Mesh m = meshWithTopologyComponents(s);
        s.run([&]() { vcl::updateVertexAdjacentFaces(m); });
    });

BENCH(
    "update/topology/update_vertex_adjacent_vertices",
    []<typename Mesh>(bench::State<Mesh>& s) {
        Mesh m = meshWithTopologyComponents(s);
        s.run([&]() { vcl::updateVertexAdjacentVertices(m); });
    });

BENCH(
    "update/topology/update_face_adjacent_faces",
    []<typename Mesh>(bench::State<Mesh>& s) {
        Mesh m = meshWithTopologyComponents(s);
        s.run([&]() { vcl::updateFaceAdjacentFaces(m); });
    });

BENCH(
    "update/topology/clear_vertex_adjacent_faces",
    []<typename Mesh>(bench::State<Mesh>& s) {
        Mesh m = meshWithTopologyComponents(s);
        s.run(
            [&]() { vcl::updateVertexAdjacentFaces(m); },
            [&]() { vcl::clearVertexAdjacentFaces(m); });
    });

BENCH(
    "update/topology/clear_vertex_adjacent_vertices",
    []<typename Mesh>(bench::State<Mesh>& s) {
        Mesh m = meshWithTopologyComponents(s);
        s.run(
            [&]() { vcl::updateVertexAdjacentVertices(m); },
            [&]() { vcl::clearVertexAdjacentVertices(m); });
    });

BENCH(
    "update/topology/clear_face_adjacent_faces",
    []<typename Mesh>(bench::State<Mesh>& s) {
        Mesh m = meshWithTopologyComponents(s);
        s.run(
            [&]() { vcl::updateFaceAdjacentFaces(m); },
            [&]() { vcl::clearFaceAdjacentFaces(m); });
    });
