// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/algorithms/mesh/point_sampling.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh.h>

#include <pybind11/eigen.h>
#include <pybind11/stl.h>

namespace vcl::bind {

namespace detail {

template<typename WeightsType, MeshConcept MeshType>
void checkVertexWeightsSize(const MeshType& m, const WeightsType& weights)
{
    if (static_cast<std::size_t>(weights.size()) != m.vertexContainerSize()) {
        throw WrongSizeException(
            "Weights size must match vertex container size (" +
            std::to_string(m.vertexContainerSize()) + ").");
    }
}

template<typename WeightsType, FaceMeshConcept MeshType>
void checkFaceWeightsSize(const MeshType& m, const WeightsType& weights)
{
    if (static_cast<std::size_t>(weights.size()) != m.faceContainerSize()) {
        throw WrongSizeException(
            "Weights size must match face container size (" +
            std::to_string(m.faceContainerSize()) + ").");
    }
}

template<typename WeightsType, MeshConcept MeshType>
void defVertexWeightedSampling(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;
    using IndexType = IndexType<MeshType>;

    m.def(
        "vertex_weighted_point_sampling",
        [](const MeshType&     m,
           const WeightsType&  weights,
           uint                nSamples,
           std::optional<uint> seed) {
            checkVertexWeightsSize(m, weights);
            return vcl::vertexWeightedPointSampling(
                m, weights, nSamples, toRConfig(seed));
        },
        "mesh"_a,
        "weights"_a,
        "n_samples"_a,
        "seed"_a = py::none());

    m.def(
        "vertex_weighted_point_sampling_with_birth_vertices",
        [](const MeshType&     m,
           const WeightsType&  weights,
           uint                nSamples,
           std::optional<uint> seed) {
            checkVertexWeightsSize(m, weights);
            std::vector<IndexType> birthVertices;
            auto ps = vcl::vertexWeightedPointSampling(
                m, weights, nSamples, birthVertices, toRConfig(seed));
            return std::make_pair(std::move(ps), std::move(birthVertices));
        },
        "mesh"_a,
        "weights"_a,
        "n_samples"_a,
        "seed"_a = py::none());
}

template<typename WeightsType, FaceMeshConcept MeshType>
void defFaceWeightedSampling(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;
    using IndexType = IndexType<MeshType>;

    m.def(
        "face_weighted_point_sampling",
        [](const MeshType&     m,
           const WeightsType&  weights,
           uint                nSamples,
           std::optional<uint> seed) {
            checkFaceWeightsSize(m, weights);
            return vcl::faceWeightedPointSampling(
                m, weights, nSamples, toRConfig(seed));
        },
        "mesh"_a,
        "weights"_a,
        "n_samples"_a,
        "seed"_a = py::none());

    m.def(
        "face_weighted_point_sampling_with_birth_faces",
        [](const MeshType&     m,
           const WeightsType&  weights,
           uint                nSamples,
           std::optional<uint> seed) {
            checkFaceWeightsSize(m, weights);
            std::vector<IndexType> birthFaces;
            auto ps = vcl::faceWeightedPointSampling(
                m, weights, nSamples, birthFaces, toRConfig(seed));
            return std::make_pair(std::move(ps), std::move(birthFaces));
        },
        "mesh"_a,
        "weights"_a,
        "n_samples"_a,
        "seed"_a = py::none());

    m.def(
        "vertex_weighted_montecarlo_point_sampling",
        [](const MeshType&     m,
           const WeightsType&  weights,
           uint                nSamples,
           double              variance,
           std::optional<uint> seed) {
            checkVertexWeightsSize(m, weights);
            return vcl::vertexWeightedMontecarloPointSampling(
                m, weights, nSamples, variance, toRConfig(seed));
        },
        "mesh"_a,
        "weights"_a,
        "n_samples"_a,
        "variance"_a,
        "seed"_a = py::none());
}

} // namespace detail

void initPointSamplingAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    auto fAllMeshes = []<MeshConcept MeshType>(
                          pybind11::module& m, MeshType = MeshType()) {
        using IndexType = IndexType<MeshType>;

        m.def(
            "all_vertices_point_sampling",
            [](const MeshType& m, bool onlySelected) {
                return vcl::allVerticesPointSampling(m, onlySelected);
            },
            "mesh"_a,
            "only_selected"_a = false);

        m.def(
            "all_vertices_point_sampling_with_birth_vertices",
            [](const MeshType& m, bool onlySelected) {
                std::vector<IndexType> birthVertices;
                auto                   ps = vcl::allVerticesPointSampling(
                    m, birthVertices, onlySelected);
                return std::make_pair(std::move(ps), std::move(birthVertices));
            },
            "mesh"_a,
            "only_selected"_a = false);

        m.def(
            "vertex_uniform_point_sampling",
            [](const MeshType&     m,
               uint                nSamples,
               bool                onlySelected,
               std::optional<uint> seed) {
                return vcl::vertexUniformPointSampling(
                    m, nSamples, onlySelected, toRConfig(seed));
            },
            "mesh"_a,
            "n_samples"_a,
            "only_selected"_a = false,
            "seed"_a          = py::none());

        m.def(
            "vertex_uniform_point_sampling_with_birth_vertices",
            [](const MeshType&     m,
               uint                nSamples,
               bool                onlySelected,
               std::optional<uint> seed) {
                std::vector<IndexType> birthVertices;
                auto                   ps = vcl::vertexUniformPointSampling(
                    m, nSamples, birthVertices, onlySelected, toRConfig(seed));
                return std::make_pair(std::move(ps), std::move(birthVertices));
            },
            "mesh"_a,
            "n_samples"_a,
            "only_selected"_a = false,
            "seed"_a          = py::none());

        detail::defVertexWeightedSampling<Eigen::VectorXd, MeshType>(m);
        detail::defVertexWeightedSampling<std::vector<double>, MeshType>(m);

        m.def(
            "vertex_quality_weighted_point_sampling",
            [](const MeshType& m, uint nSamples, std::optional<uint> seed) {
                return vcl::vertexQualityWeightedPointSampling(
                    m, nSamples, toRConfig(seed));
            },
            "mesh"_a,
            "n_samples"_a,
            "seed"_a = py::none());
    };

    defForAllMeshTypes(m, fAllMeshes);

    auto fFaceMeshes = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        using IndexType = IndexType<MeshType>;

        m.def(
            "all_faces_point_sampling",
            [](const MeshType& m, bool onlySelected) {
                return vcl::allFacesPointSampling(m, onlySelected);
            },
            "mesh"_a,
            "only_selected"_a = false);

        m.def(
            "all_faces_point_sampling_with_birth_faces",
            [](const MeshType& m, bool onlySelected) {
                std::vector<IndexType> birthFaces;
                auto                   ps =
                    vcl::allFacesPointSampling(m, birthFaces, onlySelected);
                return std::make_pair(std::move(ps), std::move(birthFaces));
            },
            "mesh"_a,
            "only_selected"_a = false);

        m.def(
            "face_uniform_point_sampling",
            [](const MeshType&     m,
               uint                nSamples,
               bool                onlySelected,
               std::optional<uint> seed) {
                return vcl::faceUniformPointSampling(
                    m, nSamples, onlySelected, toRConfig(seed));
            },
            "mesh"_a,
            "n_samples"_a,
            "only_selected"_a = false,
            "seed"_a          = py::none());

        m.def(
            "face_uniform_point_sampling_with_birth_faces",
            [](const MeshType&     m,
               uint                nSamples,
               bool                onlySelected,
               std::optional<uint> seed) {
                std::vector<IndexType> birthFaces;
                auto                   ps = vcl::faceUniformPointSampling(
                    m, nSamples, birthFaces, onlySelected, toRConfig(seed));
                return std::make_pair(std::move(ps), std::move(birthFaces));
            },
            "mesh"_a,
            "n_samples"_a,
            "only_selected"_a = false,
            "seed"_a          = py::none());

        detail::defFaceWeightedSampling<Eigen::VectorXd, MeshType>(m);
        detail::defFaceWeightedSampling<std::vector<double>, MeshType>(m);

        m.def(
            "face_quality_weighted_point_sampling",
            [](const MeshType& m, uint nSamples, std::optional<uint> seed) {
                return vcl::faceQualityWeightedPointSampling(
                    m, nSamples, toRConfig(seed));
            },
            "mesh"_a,
            "n_samples"_a,
            "seed"_a = py::none());

        m.def(
            "vertex_area_weighted_point_sampling",
            [](const MeshType& m, uint nSamples, std::optional<uint> seed) {
                return vcl::vertexAreaWeightedPointSampling(
                    m, nSamples, toRConfig(seed));
            },
            "mesh"_a,
            "n_samples"_a,
            "seed"_a = py::none());

        m.def(
            "face_area_weighted_point_sampling",
            [](const MeshType& m, uint nSamples, std::optional<uint> seed) {
                return vcl::faceAreaWeightedPointSampling(
                    m, nSamples, toRConfig(seed));
            },
            "mesh"_a,
            "n_samples"_a,
            "seed"_a = py::none());

        m.def(
            "montecarlo_point_sampling",
            [](const MeshType& m, uint nSamples, std::optional<uint> seed) {
                return vcl::montecarloPointSampling(
                    m, nSamples, toRConfig(seed));
            },
            "mesh"_a,
            "n_samples"_a,
            "seed"_a = py::none());

        m.def(
            "montecarlo_point_sampling_with_birth_faces",
            [](const MeshType& m, uint nSamples, std::optional<uint> seed) {
                std::vector<IndexType> birthFaces;
                auto                   ps = vcl::montecarloPointSampling(
                    m, nSamples, birthFaces, toRConfig(seed));
                return std::make_pair(std::move(ps), std::move(birthFaces));
            },
            "mesh"_a,
            "n_samples"_a,
            "seed"_a = py::none());

        m.def(
            "stratified_montecarlo_point_sampling",
            [](const MeshType& m, uint nSamples, std::optional<uint> seed) {
                return vcl::stratifiedMontecarloPointSampling(
                    m, nSamples, toRConfig(seed));
            },
            "mesh"_a,
            "n_samples"_a,
            "seed"_a = py::none());

        m.def(
            "montecarlo_poisson_point_sampling",
            [](const MeshType& m, uint nSamples, std::optional<uint> seed) {
                return vcl::montecarloPoissonPointSampling(
                    m, nSamples, toRConfig(seed));
            },
            "mesh"_a,
            "n_samples"_a,
            "seed"_a = py::none());

        m.def(
            "vertex_quality_weighted_montecarlo_point_sampling",
            [](const MeshType&     m,
               uint                nSamples,
               double              variance,
               std::optional<uint> seed) {
                return vcl::vertexQualityWeightedMontecarloPointSampling(
                    m, nSamples, variance, toRConfig(seed));
            },
            "mesh"_a,
            "n_samples"_a,
            "variance"_a,
            "seed"_a = py::none());
    };

    defForAllMeshTypes(m, fFaceMeshes);
}

} // namespace vcl::bind
