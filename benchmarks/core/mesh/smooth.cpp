// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "benchmark.h"

#include <vclib/algorithms/mesh.h>

BENCH("smooth/laplacian_smoothing", []<typename Mesh>(bench::State<Mesh>& s) {
    if constexpr (!vcl::FaceMeshConcept<Mesh>) {
        return s.skip("the mesh has no faces");
    }
    else {
        if (s.mesh().vertexCount() == 0) {
            return s.skip("the mesh has no vertices");
        }
        if (s.mesh().faceCount() == 0) {
            return s.skip("the mesh has no faces");
        }

        const Mesh& base = s.mesh();
        Mesh        m    = s.copyMesh();

        s.run([&]() { m = base; }, [&]() { vcl::laplacianSmoothing(m, 5); });
    }
});

BENCH(
    "smooth/laplacian_smoothing_selected",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            if (s.mesh().vertexCount() == 0) {
                return s.skip("the mesh has no vertices");
            }
            if (s.mesh().faceCount() == 0) {
                return s.skip("the mesh has no faces");
            }

            const Mesh& base = s.meshWithSelection();
            Mesh        m    = s.copyMeshWithSelection();

            s.run(
                [&]() { m = base; },
                [&]() { vcl::laplacianSmoothing(m, 5, true); });
        }
    });

BENCH(
    "smooth/laplacian_smoothing_cotangent",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            if (s.mesh().vertexCount() == 0) {
                return s.skip("the mesh has no vertices");
            }
            if (s.mesh().faceCount() == 0) {
                return s.skip("the mesh has no faces");
            }

            const Mesh& base = s.mesh();
            Mesh        m    = s.copyMesh();

            s.run(
                [&]() { m = base; },
                [&]() { vcl::laplacianSmoothing(m, 5, false, true); });
        }
    });

BENCH("smooth/taubin_smoothing", []<typename Mesh>(bench::State<Mesh>& s) {
    if constexpr (!vcl::FaceMeshConcept<Mesh>) {
        return s.skip("the mesh has no faces");
    }
    else {
        if (s.mesh().vertexCount() == 0) {
            return s.skip("the mesh has no vertices");
        }
        if (s.mesh().faceCount() == 0) {
            return s.skip("the mesh has no faces");
        }

        const Mesh& base = s.mesh();
        Mesh        m    = s.copyMesh();

        s.run(
            [&]() { m = base; },
            [&]() { vcl::taubinSmoothing(m, 10, 0.5f, -0.53f); });
    }
});

BENCH(
    "smooth/smooth_per_vertex_normals_point_cloud",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if (s.mesh().vertexCount() < 10) {
            return s.skip("not enough vertices (< 10)");
        }

        Mesh base = s.copyMesh();
        if constexpr (vcl::FaceMeshConcept<Mesh>) {
            if (base.faceCount() > 0) {
                vcl::updateVertexNormals(base);
            }
        }
        Mesh m = base;

        s.run(
            [&]() { m = base; },
            [&]() { vcl::smoothPerVertexNormalsPointCloud(m, 10, 3); });
    });
