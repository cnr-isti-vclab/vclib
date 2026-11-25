// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/algorithms/mesh/filter.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh.h>

#include <pybind11/eigen.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace vcl::bind {

namespace detail {

template<FaceMeshConcept InMeshType, EdgeMeshConcept OutMeshType>
void defFaceEdgeFilters(pybind11::module& m, const std::string& suffix = "")
{
    using namespace pybind11::literals;

    std::string feName  = "face_edge_mesh_filter" + suffix;
    std::string fesName = "face_edge_selection_mesh_filter" + suffix;

    m.def(
        feName.c_str(),
        [](const InMeshType&  m,
           pybind11::function faceEdgeFilter,
           bool               dontDuplicateEdges,
           bool               saveBirthIndicesInCustomComponent) {
            std::function<bool(typename InMeshType::ConstFace, uint)>
                cppFilter = [&m, &faceEdgeFilter](
                                typename InMeshType::ConstFace f,
                                uint                           ei) -> bool {
                return faceEdgeFilter(
                           const_cast<InMeshType&>(m).face(f.index()), ei)
                    .template cast<bool>();
            };
            return vcl::faceEdgeMeshFilter<OutMeshType>(
                m,
                cppFilter,
                dontDuplicateEdges,
                saveBirthIndicesInCustomComponent);
        },
        "mesh"_a,
        "face_edge_filter"_a,
        "dont_duplicate_edges"_a                  = true,
        "save_birth_indices_in_custom_component"_a = true);

    m.def(
        fesName.c_str(),
        [](const InMeshType& m,
           bool              dontDuplicateEdges,
           bool              saveBirthIndicesInCustomComponent) {
            return vcl::faceEdgeSelectionMeshFilter<OutMeshType>(
                m, dontDuplicateEdges, saveBirthIndicesInCustomComponent);
        },
        "mesh"_a,
        "dont_duplicate_edges"_a                  = true,
        "save_birth_indices_in_custom_component"_a = true);
}

} // namespace detail

void initFilterAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    auto fAllMeshes = []<MeshConcept MeshType>(
                          pybind11::module& m, MeshType = MeshType()) {
        m.def(
            "vertex_mesh_filter",
            [](const MeshType& m,
               py::function    vertexFilter,
               bool            saveBirthIndicesInCustomComponent) {
                std::function<bool(typename MeshType::ConstVertex)> cppFilter =
                    [&m,
                     &vertexFilter](typename MeshType::ConstVertex v) -> bool {
                    return vertexFilter(
                               const_cast<MeshType&>(m).vertex(v.index()))
                        .template cast<bool>();
                };
                return vcl::vertexMeshFilter<MeshType>(
                    m, cppFilter, saveBirthIndicesInCustomComponent);
            },
            "mesh"_a,
            "vertex_filter"_a,
            "save_birth_indices_in_custom_component"_a = true);

        m.def(
            "vertex_mesh_filter",
            [](const MeshType&        m,
               const Eigen::VectorXi& vertexFilter,
               bool                   saveBirthIndicesInCustomComponent) {
                return vcl::vertexMeshFilter<MeshType>(
                    m, vertexFilter, saveBirthIndicesInCustomComponent);
            },
            "mesh"_a,
            "vertex_filter"_a,
            "save_birth_indices_in_custom_component"_a = true);

        m.def(
            "vertex_mesh_filter",
            [](const MeshType&         m,
               const std::vector<int>& vertexFilter,
               bool                    saveBirthIndicesInCustomComponent) {
                return vcl::vertexMeshFilter<MeshType>(
                    m, vertexFilter, saveBirthIndicesInCustomComponent);
            },
            "mesh"_a,
            "vertex_filter"_a,
            "save_birth_indices_in_custom_component"_a = true);

        m.def(
            "vertex_selection_mesh_filter",
            [](const MeshType& m, bool saveBirthIndicesInCustomComponent) {
                return vcl::vertexSelectionMeshFilter<MeshType>(
                    m, saveBirthIndicesInCustomComponent);
            },
            "mesh"_a,
            "save_birth_indices_in_custom_component"_a = true);
    };

    defForAllMeshTypes(m, fAllMeshes);

    auto fFaceMeshes = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        m.def(
            "face_mesh_filter",
            [](const MeshType& m,
               py::function    faceFilter,
               bool            saveBirthIndicesInCustomComponent) {
                std::function<bool(typename MeshType::ConstFace)> cppFilter =
                    [&m, &faceFilter](typename MeshType::ConstFace f) -> bool {
                    return faceFilter(const_cast<MeshType&>(m).face(f.index()))
                        .template cast<bool>();
                };
                return vcl::faceMeshFilter<MeshType>(
                    m, cppFilter, saveBirthIndicesInCustomComponent);
            },
            "mesh"_a,
            "face_filter"_a,
            "save_birth_indices_in_custom_component"_a = true);

        m.def(
            "face_mesh_filter",
            [](const MeshType&        m,
               const Eigen::VectorXi& faceFilter,
               bool                   saveBirthIndicesInCustomComponent) {
                return vcl::faceMeshFilter<MeshType>(
                    m, faceFilter, saveBirthIndicesInCustomComponent);
            },
            "mesh"_a,
            "face_filter"_a,
            "save_birth_indices_in_custom_component"_a = true);

        m.def(
            "face_mesh_filter",
            [](const MeshType&         m,
               const std::vector<int>& faceFilter,
               bool                    saveBirthIndicesInCustomComponent) {
                return vcl::faceMeshFilter<MeshType>(
                    m, faceFilter, saveBirthIndicesInCustomComponent);
            },
            "mesh"_a,
            "face_filter"_a,
            "save_birth_indices_in_custom_component"_a = true);

        m.def(
            "face_selection_mesh_filter",
            [](const MeshType& m, bool saveBirthIndicesInCustomComponent) {
                return vcl::faceSelectionMeshFilter<MeshType>(
                    m, saveBirthIndicesInCustomComponent);
            },
            "mesh"_a,
            "save_birth_indices_in_custom_component"_a = true);

        // default OutMeshType = EdgeMesh
        detail::defFaceEdgeFilters<MeshType, vcl::EdgeMesh>(m);

        // typed variants for all EdgeMeshConcept output types
        auto fOutEdgeMeshes = []<EdgeMeshConcept OutMeshType>(
                                  pybind11::module& m,
                                  OutMeshType = OutMeshType()) {
            std::string suffix =
                "_" + camelCaseToSnakeCase(meshTypeName<OutMeshType>());
            detail::defFaceEdgeFilters<MeshType, OutMeshType>(m, suffix);
        };

        defForAllMeshTypes(m, fOutEdgeMeshes);
    };

    defForAllMeshTypes(m, fFaceMeshes);

    auto fEdgeMeshes = []<EdgeMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        m.def(
            "edge_mesh_filter",
            [](const MeshType& m,
               py::function    edgeFilter,
               bool            saveBirthIndicesInCustomComponent) {
                std::function<bool(typename MeshType::ConstEdge)> cppFilter =
                    [&m, &edgeFilter](typename MeshType::ConstEdge e) -> bool {
                    return edgeFilter(const_cast<MeshType&>(m).edge(e.index()))
                        .template cast<bool>();
                };
                return vcl::edgeMeshFilter<MeshType>(
                    m, cppFilter, saveBirthIndicesInCustomComponent);
            },
            "mesh"_a,
            "edge_filter"_a,
            "save_birth_indices_in_custom_component"_a = true);

        m.def(
            "edge_mesh_filter",
            [](const MeshType&        m,
               const Eigen::VectorXi& edgeFilter,
               bool                   saveBirthIndicesInCustomComponent) {
                return vcl::edgeMeshFilter<MeshType>(
                    m, edgeFilter, saveBirthIndicesInCustomComponent);
            },
            "mesh"_a,
            "edge_filter"_a,
            "save_birth_indices_in_custom_component"_a = true);

        m.def(
            "edge_mesh_filter",
            [](const MeshType&         m,
               const std::vector<int>& edgeFilter,
               bool                    saveBirthIndicesInCustomComponent) {
                return vcl::edgeMeshFilter<MeshType>(
                    m, edgeFilter, saveBirthIndicesInCustomComponent);
            },
            "mesh"_a,
            "edge_filter"_a,
            "save_birth_indices_in_custom_component"_a = true);

        m.def(
            "edge_selection_mesh_filter",
            [](const MeshType& m, bool saveBirthIndicesInCustomComponent) {
                return vcl::edgeSelectionMeshFilter<MeshType>(
                    m, saveBirthIndicesInCustomComponent);
            },
            "mesh"_a,
            "save_birth_indices_in_custom_component"_a = true);
    };

    defForAllMeshTypes(m, fEdgeMeshes);
}

} // namespace vcl::bind
