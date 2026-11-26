// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/algorithms/mesh/update.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh/update/normal.h>

namespace vcl::bind::detail {

void initUpdateNormalAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    auto fAllMeshes =
        []<MeshConcept MeshType>(pybind11::module& m, MeshType = MeshType()) {
            m.def(
                "clear_vertex_normals",
                [](MeshType& m, AbstractLogger& log) {
                    return clearVertexNormals(m, log);
                },
                "mesh"_a,
                "log"_a = py::cast(&vcl::nullLogger));

            m.def(
                "clear_referenced_vertex_normals",
                [](MeshType& m, AbstractLogger& log) {
                    return clearReferencedVertexNormals(m, log);
                },
                "mesh"_a,
                "log"_a = py::cast(&vcl::nullLogger));

            m.def(
                "normalize_vertex_normals",
                [](MeshType& m, AbstractLogger& log) {
                    return normalizeVertexNormals(m, log);
                },
                "mesh"_a,
                "log"_a = py::cast(&vcl::nullLogger));

            m.def(
                "normalize_referenced_vertex_normals",
                [](MeshType& m, AbstractLogger& log) {
                    return normalizeReferencedVertexNormals(m, log);
                },
                "mesh"_a,
                "log"_a = py::cast(&vcl::nullLogger));

            m.def(
                "multiply_vertex_normals_by_matrix",
                [](MeshType&             m,
                   const vcl::Matrix33d& mat,
                   bool                  removeScalingFromMatrix = true,
                   AbstractLogger& log = vcl::nullLogger) {
                    return multiplyVertexNormalsByMatrix(
                        m, mat, removeScalingFromMatrix, log);
                },
                "mesh"_a,
                "matrix"_a,
                "remove_scaling_from_matrix"_a = true,
                "log"_a                        = py::cast(&vcl::nullLogger));

            m.def(
                "multiply_vertex_normals_by_matrix",
                [](MeshType&             m,
                   const vcl::Matrix44d& mat,
                   bool                  removeScalingFromMatrix = true,
                   AbstractLogger& log = vcl::nullLogger) {
                    return multiplyVertexNormalsByMatrix(
                        m, mat, removeScalingFromMatrix, log);
                },
                "mesh"_a,
                "matrix"_a,
                "remove_scaling_from_matrix"_a = true,
                "log"_a                        = py::cast(&vcl::nullLogger));
        };

    defForAllMeshTypes(m, fAllMeshes);

    auto fFaceMeshes = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        m.def(
            "clear_face_normals",
            [](MeshType& m, AbstractLogger& log) {
                return vcl::clearFaceNormals(m, log);
            },
            "mesh"_a,
            "log"_a = py::cast(&vcl::nullLogger));

        m.def(
            "normalize_face_normals",
            [](MeshType& m, AbstractLogger& log) {
                return vcl::normalizeFaceNormals(m, log);
            },
            "mesh"_a,
            "log"_a = py::cast(&vcl::nullLogger));

        m.def(
            "multiply_face_normals_by_matrix",
            [](MeshType&             m,
               const vcl::Matrix33d& mat,
               bool                  removeScalingFromMatrix = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::multiplyFaceNormalsByMatrix(
                    m, mat, removeScalingFromMatrix, log);
            },
            "mesh"_a,
            "matrix"_a,
            "remove_scaling_from_matrix"_a = true,
            "log"_a                        = py::cast(&vcl::nullLogger));

        m.def(
            "multiply_face_normals_by_matrix",
            [](MeshType&             m,
               const vcl::Matrix44d& mat,
               bool                  removeScalingFromMatrix = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::multiplyFaceNormalsByMatrix(
                    m, mat, removeScalingFromMatrix, log);
            },
            "mesh"_a,
            "matrix"_a,
            "remove_scaling_from_matrix"_a = true,
            "log"_a                        = py::cast(&vcl::nullLogger));

        m.def(
            "update_face_normals",
            [](MeshType& m,
               bool      normalize = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::updateFaceNormals(m, normalize, log);
            },
            "mesh"_a,
            "normalize"_a = true,
            "log"_a       = py::cast(&vcl::nullLogger));

        m.def(
            "update_vertex_normals",
            [](MeshType& m,
               bool      normalize = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::updateVertexNormals(m, normalize, log);
            },
            "mesh"_a,
            "normalize"_a = true,
            "log"_a       = py::cast(&vcl::nullLogger));

        m.def(
            "update_vertex_normals_from_face_normals",
            [](MeshType& m,
               bool      normalize = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::updateVertexNormalsFromFaceNormals(
                    m, normalize, log);
            },
            "mesh"_a,
            "normalize"_a = true,
            "log"_a       = py::cast(&vcl::nullLogger));

        m.def(
            "update_vertex_and_face_normals",
            [](MeshType& m,
               bool      normalize = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::updateVertexAndFaceNormals(m, normalize, log);
            },
            "mesh"_a,
            "normalize"_a = true,
            "log"_a       = py::cast(&vcl::nullLogger));

        m.def(
            "update_vertex_normals_angle_weighted",
            [](MeshType& m,
               bool      normalize = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::updateVertexNormalsAngleWeighted(m, normalize, log);
            },
            "mesh"_a,
            "normalize"_a = true,
            "log"_a       = py::cast(&vcl::nullLogger));

        m.def(
            "update_vertex_normals_nelson_max_weighted",
            [](MeshType& m,
               bool      normalize = true,
               AbstractLogger& log = vcl::nullLogger) {
                return vcl::updateVertexNormalsNelsonMaxWeighted(
                    m, normalize, log);
            },
            "mesh"_a,
            "normalize"_a = true,
            "log"_a       = py::cast(&vcl::nullLogger));
    };

    defForAllMeshTypes(m, fFaceMeshes);

    auto fEdgeMeshes = []<EdgeMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        m.def(
            "clear_edge_normals",
            [](MeshType& m, AbstractLogger& log) {
                return vcl::clearElementNormals<ElemId::EDGE>(m, log);
            },
            "mesh"_a,
            "log"_a = py::cast(&vcl::nullLogger));

        m.def(
            "normalize_edge_normals",
            [](MeshType& m, AbstractLogger& log) {
                return vcl::normalizeElementNormals<ElemId::EDGE>(m, log);
            },
            "mesh"_a,
            "log"_a = py::cast(&vcl::nullLogger));

        m.def(
            "multiply_edge_normals_by_matrix",
            [](MeshType&             m,
               const vcl::Matrix33d& mat,
               bool                  removeScalingFromMatrix,
               AbstractLogger&       log) {
                return vcl::multiplyElementNormalsByMatrix<ElemId::EDGE>(
                    m, mat, removeScalingFromMatrix, log);
            },
            "mesh"_a,
            "matrix"_a,
            "remove_scaling_from_matrix"_a = true,
            "log"_a                        = py::cast(&vcl::nullLogger));

        m.def(
            "multiply_edge_normals_by_matrix",
            [](MeshType&             m,
               const vcl::Matrix44d& mat,
               bool                  removeScalingFromMatrix,
               AbstractLogger&       log) {
                return vcl::multiplyElementNormalsByMatrix<ElemId::EDGE>(
                    m, mat, removeScalingFromMatrix, log);
            },
            "mesh"_a,
            "matrix"_a,
            "remove_scaling_from_matrix"_a = true,
            "log"_a                        = py::cast(&vcl::nullLogger));
    };

    defForAllMeshTypes(m, fEdgeMeshes);
}

} // namespace vcl::bind::detail
