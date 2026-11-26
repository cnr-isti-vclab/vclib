// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/algorithms/mesh/stat.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh.h>

#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <span>

namespace vcl::bind {

namespace detail {

template<MeshConcept MeshType>
void defWeightedBarycenter(pybind11::module& m)
{
    using namespace pybind11::literals;

    m.def(
        "weighted_barycenter",
        [](const MeshType& m,
           const pybind11::array_t<double, pybind11::array::c_style>& weights) {
            if (weights.ndim() != 1 ||
                static_cast<std::size_t>(weights.shape(0)) != m.vertexCount()) {
                throw WrongSizeException(
                    "Weights size must match vertex count (" +
                    std::to_string(m.vertexCount()) + ").");
            }
            return vcl::weightedBarycenter(
                m, std::span<const double>(weights.data(), weights.size()));
        },
        "mesh"_a,
        "weights"_a);

    m.def(
        "weighted_barycenter",
        [](const MeshType& m, const std::vector<double>& weights) {
            if (weights.size() != m.vertexCount()) {
                throw WrongSizeException(
                    "Weights size must match vertex count (" +
                    std::to_string(m.vertexCount()) + ").");
            }
            return vcl::weightedBarycenter(m, weights);
        },
        "mesh"_a,
        "weights"_a);
}

} // namespace detail

void initStatAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;

    auto fAllMeshes = []<MeshConcept MeshType>(
                          pybind11::module& m, MeshType = MeshType()) {
        // barycenter.h

        m.def(
            "barycenter", [](const MeshType& m) { return vcl::barycenter(m); });

        detail::defWeightedBarycenter<MeshType>(m);

        m.def("quality_weighted_barycenter", [](const MeshType& m) {
            return vcl::qualityWeightedBarycenter(m);
        });

        // bounding_box.h

        m.def("bounding_box", [](const MeshType& m) {
            return vcl::boundingBox(m);
        });

        // geometry.h

        m.def("covariance_matrix_of_point_cloud", [](const MeshType& m) {
            return vcl::covarianceMatrixOfPointCloud(m);
        });

        m.def(
            "vertex_radius_from_weights",
            [](const MeshType&            m,
               const std::vector<double>& weights,
               double                     diskRadius,
               double                     radiusVariance,
               bool                       invert) {
                return vcl::vertexRadiusFromWeights(
                    m, weights, diskRadius, radiusVariance, invert);
            },
            py::arg("mesh"),
            py::arg("weights"),
            py::arg("disk_radius"),
            py::arg("radius_variance"),
            py::arg("invert") = false);

        // quality.h

        m.def("vertex_quality_min_max", [](const MeshType& m) {
            return vcl::vertexQualityMinMax(m);
        });

        m.def("vertex_quality_average", [](const MeshType& m) {
            return vcl::vertexQualityAverage(m);
        });

        m.def(
            "vertex_quality_histogram",
            [](const MeshType& m, bool selectionOnly, uint histSize) {
                return vcl::vertexQualityHistogram(m, selectionOnly, histSize);
            },
            py::arg("mesh"),
            py::arg("selection_only") = false,
            py::arg("hist_size")      = 10000u);

        m.def(
            "vertex_radius_from_quality",
            [](const MeshType& m,
               double          diskRadius,
               double          radiusVariance,
               bool            invert) {
                return vcl::vertexRadiusFromQuality(
                    m, diskRadius, radiusVariance, invert);
            },
            py::arg("mesh"),
            py::arg("disk_radius"),
            py::arg("radius_variance"),
            py::arg("invert") = false);

        // selection.h

        m.def("vertex_selection_count", [](const MeshType& m) {
            return vcl::vertexSelectionCount(m);
        });

        // topology.h

        m.def(
            "referenced_vertices",
            [](const MeshType& m, bool onlyFaces) {
                uint nUnref = 0;
                return vcl::referencedVertices<std::vector<bool>>(
                    m, nUnref, onlyFaces);
            },
            py::arg("mesh"),
            py::arg("only_faces") = false);

        m.def(
            "unreferenced_vertex_count",
            [](const MeshType& m, bool onlyFaces) {
                return vcl::unreferencedVertexCount(m, onlyFaces);
            },
            py::arg("mesh"),
            py::arg("only_faces") = false);
    };

    defForAllMeshTypes(m, fAllMeshes);

    auto fFaceMeshes = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        // barycenter.h

        m.def("shell_barycenter", [](const MeshType& m) {
            return vcl::shellBarycenter(m);
        });

        // geometry.h

        m.def("volume", [](const MeshType& m) { return vcl::volume(m); });

        m.def("surface_area", [](const MeshType& m) {
            return vcl::surfaceArea(m);
        });

        m.def("border_length", [](const MeshType& m) {
            return vcl::borderLength(m);
        });

        m.def("covariance_matrix_of_mesh", [](const MeshType& m) {
            return vcl::covarianceMatrixOfMesh(m);
        });

        m.def(
            "crease_face_edges",
            [](const MeshType& m,
               double          angleRadNeg,
               double          angleRadPos,
               bool            alsoBorderEdges) {
                return vcl::creaseFaceEdges(
                    m, angleRadNeg, angleRadPos, alsoBorderEdges);
            },
            py::arg("mesh"),
            py::arg("angle_rad_neg"),
            py::arg("angle_rad_pos"),
            py::arg("also_border_edges") = false);

        // quality.h

        m.def("face_quality_min_max", [](const MeshType& m) {
            return vcl::faceQualityMinMax(m);
        });

        m.def("face_quality_average", [](const MeshType& m) {
            return vcl::faceQualityAverage(m);
        });

        m.def(
            "face_quality_histogram",
            [](const MeshType& m, bool selectionOnly, uint histSize) {
                return vcl::faceQualityHistogram(m, selectionOnly, histSize);
            },
            py::arg("mesh"),
            py::arg("selection_only") = false,
            py::arg("hist_size")      = 10000u);

        // selection.h

        m.def("face_selection_count", [](const MeshType& m) {
            return vcl::faceSelectionCount(m);
        });

        m.def("face_edges_selection_count", [](const MeshType& m) {
            return vcl::faceEdgesSelectionCount(m);
        });

        // topology.h

        m.def("face_vertex_references_count", [](const MeshType& m) {
            return vcl::faceVertexReferencesCount(m);
        });

        m.def("largest_face_size", [](const MeshType& m) {
            return vcl::largestFaceSize(m);
        });

        m.def("triangulated_face_count", [](const MeshType& m) {
            return vcl::triangulatedFaceCount(m);
        });

        m.def("non_manifold_vertex_count", [](const MeshType& m) {
            return vcl::nonManifoldVertexCount(m);
        });

        m.def("is_water_tight", [](const MeshType& m) {
            return vcl::isWaterTight(m);
        });

        m.def(
            "hole_count", [](const MeshType& m) { return vcl::holeCount(m); });

        m.def("connected_components", [](const MeshType& m) {
            return vcl::connectedComponents(m);
        });

        m.def("connected_component_count", [](const MeshType& m) {
            return vcl::connectedComponentCount(m);
        });
    };

    defForAllMeshTypes(m, fFaceMeshes);

    auto fEdgeMeshes = []<EdgeMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        // selection.h

        m.def(
            "edge_selection_count",
            [](const MeshType& m) { return vcl::edgeSelectionCount(m); },
            py::arg("mesh"));
    };

    defForAllMeshTypes(m, fEdgeMeshes);
}

} // namespace vcl::bind
