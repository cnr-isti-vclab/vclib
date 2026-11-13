// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../benchmark.h"

#include <vclib/algorithms/mesh.h>

BENCH(
    "update/normal/update_face_normals",
    []<typename Mesh>(bench::State<Mesh>& s) {
        Mesh m = s.copyMesh();
        s.run([&]() { vcl::updateFaceNormals(m); });
    });

BENCH(
    "update/normal/update_vertex_normals",
    []<typename Mesh>(bench::State<Mesh>& s) {
        Mesh m = s.copyMesh();
        s.run([&]() { vcl::updateVertexNormals(m); });
    });

BENCH(
    "update/normal/update_vertex_normals_from_face_normals",
    []<typename Mesh>(bench::State<Mesh>& s) {
        Mesh m = s.copyMesh();
        vcl::updateFaceNormals(m);
        s.run([&]() { vcl::updateVertexNormalsFromFaceNormals(m); });
    });

BENCH(
    "update/normal/update_vertex_normals_angle_weighted",
    []<typename Mesh>(bench::State<Mesh>& s) {
        Mesh m = s.copyMesh();
        s.run([&]() { vcl::updateVertexNormalsAngleWeighted(m); });
    });

BENCH(
    "update/normal/update_vertex_normals_nelson_max_weighted",
    []<typename Mesh>(bench::State<Mesh>& s) {
        Mesh m = s.copyMesh();
        s.run([&]() { vcl::updateVertexNormalsNelsonMaxWeighted(m); });
    });
