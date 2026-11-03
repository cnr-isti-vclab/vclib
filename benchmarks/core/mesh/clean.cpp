// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "benchmark.h"

#include <vclib/algorithms/mesh/clean.h>

BENCH(
    "clean/remove_unreferenced_vertices",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& base = s.mesh();
        Mesh        m    = s.copyMesh();

        s.run(
            [&]() { m = base; },
            [&]() { return vcl::removeUnreferencedVertices(m); });
    });

BENCH(
    "clean/remove_duplicate_vertices",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& base = s.mesh();
        Mesh        m    = s.copyMesh();

        s.run(
            [&]() { m = base; },
            [&]() { return vcl::removeDuplicateVertices(m); });
    });

BENCH(
    "clean/remove_duplicate_faces",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            const Mesh& base = s.mesh();
            Mesh        m    = s.copyMesh();

            s.run(
                [&]() { m = base; },
                [&]() { return vcl::removeDuplicateFaces(m); });
        }
    });

BENCH(
    "clean/remove_degenerate_vertices",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& base = s.mesh();
        Mesh        m    = s.copyMesh();

        s.run(
            [&]() { m = base; },
            [&]() { return vcl::removeDegenerateVertices(m, false); });
    });

BENCH(
    "clean/remove_degenerate_vertices_delete_incident_elements",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& base = s.mesh();
        Mesh        m    = s.copyMesh();

        s.run(
            [&]() { m = base; },
            [&]() { return vcl::removeDegenerateVertices(m, true); });
    });

BENCH(
    "clean/remove_degenerate_faces",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            const Mesh& base = s.mesh();
            Mesh        m    = s.copyMesh();

            s.run(
                [&]() { m = base; },
                [&]() { return vcl::removeDegenerateFaces(m); });
        }
    });
