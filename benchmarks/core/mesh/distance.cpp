// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "benchmark.h"

#include <vclib/algorithms/mesh.h>

#include <algorithm>

namespace {

constexpr vcl::uint DEFAULT_SAMPLES = 1000;

template<typename Mesh>
vcl::uint sampleCountForVertices(const Mesh& m)
{
    if (m.vertexCount() <= 1) {
        return 0;
    }
    return std::max<vcl::uint>(
        1, std::min<vcl::uint>(DEFAULT_SAMPLES, m.vertexCount() / 2));
}

} // namespace

BENCH(
    "distance/hausdorff_distance_vertex_uniform",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            const Mesh& m1 = s.mesh();
            if (m1.vertexCount() == 0) {
                return s.skip("the mesh has no vertices");
            }
            if (m1.faceCount() == 0) {
                return s.skip("the mesh has no faces");
            }

            const vcl::uint nSamples = sampleCountForVertices(m1);
            if (nSamples == 0) {
                return s.skip("not enough vertices to sample");
            }

            Mesh m2 = s.copyMesh();
            vcl::laplacianSmoothing(m2, 2);

            s.run([&]() {
                return vcl::hausdorffDistance(
                    m1,
                    m2,
                    vcl::nullLogger,
                    vcl::HAUSDORFF_VERTEX_UNIFORM,
                    nSamples,
                    s.seed());
            });
        }
    });

BENCH(
    "distance/hausdorff_distance_montecarlo",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            const Mesh& m1 = s.mesh();
            if (m1.vertexCount() == 0) {
                return s.skip("the mesh has no vertices");
            }
            if (m1.faceCount() == 0) {
                return s.skip("the mesh has no faces");
            }

            const vcl::uint nSamples = sampleCountForVertices(m1);
            if (nSamples == 0) {
                return s.skip("not enough vertices to sample");
            }

            Mesh m2 = s.copyMesh();
            vcl::laplacianSmoothing(m2, 2);

            s.run([&]() {
                return vcl::hausdorffDistance(
                    m1,
                    m2,
                    vcl::nullLogger,
                    vcl::HAUSDORFF_MONTECARLO,
                    nSamples,
                    s.seed());
            });
        }
    });
