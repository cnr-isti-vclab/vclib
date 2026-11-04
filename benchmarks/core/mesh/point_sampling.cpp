// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "benchmark.h"

#include <vclib/algorithms/mesh/point_sampling.h>

#include <algorithm>
#include <vector>

namespace {

constexpr vcl::uint DEFAULT_SAMPLES = 1000;

template<typename Mesh>
vcl::uint sampleCountForVertices(const Mesh& m, bool onlySelected = false)
{
    vcl::uint count =
        onlySelected ? vcl::vertexSelectionCount(m) : m.vertexCount();
    if (count <= 1) {
        return 0;
    }
    return std::max<vcl::uint>(1, std::min(DEFAULT_SAMPLES, count / 2));
}

template<typename Mesh>
vcl::uint sampleCountForFaces(const Mesh& m, bool onlySelected = false)
{
    vcl::uint count = onlySelected ? vcl::faceSelectionCount(m) : m.faceCount();
    if (count <= 1) {
        return 0;
    }
    return std::max<vcl::uint>(1, std::min(DEFAULT_SAMPLES, count / 2));
}

} // namespace

BENCH(
    "point_sampling/all_vertices_point_sampling",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& m = s.mesh();
        s.run([&]() { return vcl::allVerticesPointSampling(m); });
    });

BENCH(
    "point_sampling/all_vertices_point_sampling_selected",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& m = s.meshWithSelection();
        s.run([&]() { return vcl::allVerticesPointSampling(m, true); });
    });

BENCH(
    "point_sampling/all_faces_point_sampling",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            const Mesh& m = s.mesh();
            s.run([&]() { return vcl::allFacesPointSampling(m); });
        }
    });

BENCH(
    "point_sampling/all_faces_point_sampling_selected",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            const Mesh& m = s.meshWithSelection();
            s.run([&]() { return vcl::allFacesPointSampling(m, true); });
        }
    });

BENCH(
    "point_sampling/vertex_uniform_point_sampling",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh&     m        = s.mesh();
        const vcl::uint nSamples = sampleCountForVertices(m);
        if (nSamples == 0) {
            return s.skip("not enough vertices to sample");
        }
        s.run([&]() { return vcl::vertexUniformPointSampling(m, nSamples); });
    });

BENCH(
    "point_sampling/vertex_uniform_point_sampling_selected",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh&     m        = s.meshWithSelection();
        const vcl::uint nSamples = sampleCountForVertices(m, true);
        if (nSamples == 0) {
            return s.skip("not enough selected vertices to sample");
        }
        s.run([&]() {
            return vcl::vertexUniformPointSampling(m, nSamples, true);
        });
    });

BENCH(
    "point_sampling/face_uniform_point_sampling",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            const Mesh&     m        = s.mesh();
            const vcl::uint nSamples = sampleCountForFaces(m);
            if (nSamples == 0) {
                return s.skip("not enough faces to sample");
            }
            s.run([&]() { return vcl::faceUniformPointSampling(m, nSamples); });
        }
    });

BENCH(
    "point_sampling/face_uniform_point_sampling_selected",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            const Mesh&     m        = s.meshWithSelection();
            const vcl::uint nSamples = sampleCountForFaces(m, true);
            if (nSamples == 0) {
                return s.skip("not enough selected faces to sample");
            }
            s.run([&]() {
                return vcl::faceUniformPointSampling(m, nSamples, true);
            });
        }
    });

BENCH(
    "point_sampling/vertex_weighted_point_sampling",
    []<typename Mesh>(bench::State<Mesh>& s) {
        using Scalar             = typename Mesh::ScalarType;
        const Mesh&     m        = s.mesh();
        const vcl::uint nSamples = sampleCountForVertices(m);
        if (nSamples == 0) {
            return s.skip("not enough vertices to sample");
        }
        std::vector<Scalar> weights(m.vertexContainerSize());
        for (vcl::uint i = 0; i < weights.size(); ++i) {
            weights[i] = static_cast<Scalar>(i + 1);
        }
        s.run([&]() {
            return vcl::vertexWeightedPointSampling(m, weights, nSamples);
        });
    });

BENCH(
    "point_sampling/face_weighted_point_sampling",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            using Scalar             = typename Mesh::ScalarType;
            const Mesh&     m        = s.mesh();
            const vcl::uint nSamples = sampleCountForFaces(m);
            if (nSamples == 0) {
                return s.skip("not enough faces to sample");
            }
            std::vector<Scalar> weights(m.faceContainerSize());
            for (vcl::uint i = 0; i < weights.size(); ++i) {
                weights[i] = static_cast<Scalar>(i + 1);
            }
            s.run([&]() {
                return vcl::faceWeightedPointSampling(m, weights, nSamples);
            });
        }
    });

BENCH(
    "point_sampling/vertex_quality_weighted_point_sampling",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::HasPerVertexQuality<Mesh>) {
            return s.skip("the mesh has no vertex quality component");
        }
        else {
            const Mesh&     m        = s.meshWithQuality();
            const vcl::uint nSamples = sampleCountForVertices(m);
            if (nSamples == 0) {
                return s.skip("not enough vertices to sample");
            }
            s.run([&]() {
                return vcl::vertexQualityWeightedPointSampling(m, nSamples);
            });
        }
    });

BENCH(
    "point_sampling/face_quality_weighted_point_sampling",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else if constexpr (!vcl::HasPerFaceQuality<Mesh>) {
            return s.skip("the mesh has no face quality component");
        }
        else {
            const Mesh&     m        = s.meshWithQuality();
            const vcl::uint nSamples = sampleCountForFaces(m);
            if (nSamples == 0) {
                return s.skip("not enough faces to sample");
            }
            s.run([&]() {
                return vcl::faceQualityWeightedPointSampling(m, nSamples);
            });
        }
    });

BENCH(
    "point_sampling/vertex_area_weighted_point_sampling",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            const Mesh&     m        = s.mesh();
            const vcl::uint nSamples = sampleCountForVertices(m);
            if (nSamples == 0) {
                return s.skip("not enough vertices to sample");
            }
            s.run([&]() {
                return vcl::vertexAreaWeightedPointSampling(m, nSamples);
            });
        }
    });

BENCH(
    "point_sampling/face_area_weighted_point_sampling",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            const Mesh&     m        = s.mesh();
            const vcl::uint nSamples = sampleCountForFaces(m);
            if (nSamples == 0) {
                return s.skip("not enough faces to sample");
            }
            s.run([&]() {
                return vcl::faceAreaWeightedPointSampling(m, nSamples);
            });
        }
    });

BENCH(
    "point_sampling/montecarlo_point_sampling",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            const Mesh&     m        = s.mesh();
            const vcl::uint nSamples = sampleCountForFaces(m);
            if (nSamples == 0) {
                return s.skip("not enough faces to sample");
            }
            s.run([&]() { return vcl::montecarloPointSampling(m, nSamples); });
        }
    });

BENCH(
    "point_sampling/stratified_montecarlo_point_sampling",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            const Mesh&     m        = s.mesh();
            const vcl::uint nSamples = sampleCountForFaces(m);
            if (nSamples == 0) {
                return s.skip("not enough faces to sample");
            }
            s.run([&]() {
                return vcl::stratifiedMontecarloPointSampling(m, nSamples);
            });
        }
    });

BENCH(
    "point_sampling/montecarlo_poisson_point_sampling",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            const Mesh&     m        = s.mesh();
            const vcl::uint nSamples = sampleCountForFaces(m);
            if (nSamples == 0) {
                return s.skip("not enough faces to sample");
            }
            s.run([&]() {
                return vcl::montecarloPoissonPointSampling(m, nSamples);
            });
        }
    });

BENCH(
    "point_sampling/vertex_weighted_montecarlo_point_sampling",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            using Scalar             = typename Mesh::ScalarType;
            const Mesh&     m        = s.mesh();
            const vcl::uint nSamples = sampleCountForVertices(m);
            if (nSamples == 0) {
                return s.skip("not enough vertices to sample");
            }
            std::vector<Scalar> weights(m.vertexContainerSize());
            for (vcl::uint i = 0; i < weights.size(); ++i) {
                weights[i] = static_cast<Scalar>(i + 1);
            }
            s.run([&]() {
                return vcl::vertexWeightedMontecarloPointSampling(
                    m, weights, nSamples, 2.0);
            });
        }
    });

BENCH(
    "point_sampling/vertex_quality_weighted_montecarlo_point_sampling",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else if constexpr (!vcl::HasPerVertexQuality<Mesh>) {
            return s.skip("the mesh has no vertex quality component");
        }
        else {
            const Mesh&     m        = s.meshWithQuality();
            const vcl::uint nSamples = sampleCountForVertices(m);
            if (nSamples == 0) {
                return s.skip("not enough vertices to sample");
            }
            s.run([&]() {
                return vcl::vertexQualityWeightedMontecarloPointSampling(
                    m, nSamples, 2.0);
            });
        }
    });
