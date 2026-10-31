// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../benchmark.h"

#include <vclib/algorithms/mesh/update/normal.h>

BENCH("update/normal/update_per_face_normals", [](bench::State& s) {
    bench::Mesh m = s.copyMesh();
    s.run([&]() { vcl::updatePerFaceNormals(m); });
});

BENCH("update/normal/update_per_vertex_normals", [](bench::State& s) {
    bench::Mesh m = s.copyMesh();
    s.run([&]() { vcl::updatePerVertexNormals(m); });
});

BENCH(
    "update/normal/update_per_vertex_normals_from_face_normals",
    [](bench::State& s) {
        bench::Mesh m = s.copyMesh();
        vcl::updatePerFaceNormals(m);
        s.run([&]() { vcl::updatePerVertexNormalsFromFaceNormals(m); });
    });

BENCH(
    "update/normal/update_per_vertex_normals_angle_weighted",
    [](bench::State& s) {
        bench::Mesh m = s.copyMesh();
        s.run([&]() { vcl::updatePerVertexNormalsAngleWeighted(m); });
    });

BENCH(
    "update/normal/update_per_vertex_normals_nelson_max_weighted",
    [](bench::State& s) {
        bench::Mesh m = s.copyMesh();
        s.run([&]() { vcl::updatePerVertexNormalsNelsonMaxWeighted(m); });
    });
