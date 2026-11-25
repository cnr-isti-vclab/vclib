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

template<ElemId ELEM_ID, MeshConcept MeshType>
void defElementMeshFilter(
    pybind11::module&  m,
    const std::string& name,
    const std::string& selectionName,
    const std::string& filterArgName)
{
    using namespace pybind11::literals;
    using ConstElemType = typename MeshType::template ConstElementType<ELEM_ID>;

    m.def(
        name.c_str(),
        [](const MeshType&    m,
           pybind11::function elemFilter,
           bool               saveBirthIndicesInCustomComponent) {
            std::function<bool(ConstElemType)> cppFilter =
                [&m, &elemFilter](ConstElemType e) -> bool {
                return elemFilter(
                           const_cast<MeshType&>(m).template element<ELEM_ID>(
                               e.index()))
                    .template cast<bool>();
            };
            return vcl::elementMeshFilter<ELEM_ID>(
                m, cppFilter, saveBirthIndicesInCustomComponent);
        },
        "mesh"_a,
        pybind11::arg(filterArgName.c_str()),
        "save_birth_indices_in_custom_component"_a = true);

    m.def(
        name.c_str(),
        [](const MeshType&        m,
           const Eigen::VectorXi& elemFilter,
           bool                   saveBirthIndicesInCustomComponent) {
            return vcl::elementMeshFilter<ELEM_ID>(
                m, elemFilter, saveBirthIndicesInCustomComponent);
        },
        "mesh"_a,
        pybind11::arg(filterArgName.c_str()),
        "save_birth_indices_in_custom_component"_a = true);

    m.def(
        name.c_str(),
        [](const MeshType&         m,
           const std::vector<int>& elemFilter,
           bool                    saveBirthIndicesInCustomComponent) {
            return vcl::elementMeshFilter<ELEM_ID>(
                m, elemFilter, saveBirthIndicesInCustomComponent);
        },
        "mesh"_a,
        pybind11::arg(filterArgName.c_str()),
        "save_birth_indices_in_custom_component"_a = true);

    m.def(
        selectionName.c_str(),
        [](const MeshType& m, bool saveBirthIndicesInCustomComponent) {
            return vcl::elementSelectionMeshFilter<ELEM_ID>(
                m, saveBirthIndicesInCustomComponent);
        },
        "mesh"_a,
        "save_birth_indices_in_custom_component"_a = true);
}

template<FaceMeshConcept InMeshType, EdgeMeshConcept OutMeshType>
void defFaceEdgeFilters(pybind11::module& m, const std::string& suffix = "")
{
    using namespace pybind11::literals;
    using ConstFaceType = typename InMeshType::ConstFace;

    std::string feName  = "face_edge_mesh_filter" + suffix;
    std::string fesName = "face_edge_selection_mesh_filter" + suffix;

    m.def(
        feName.c_str(),
        [](const InMeshType&  m,
           pybind11::function faceEdgeFilter,
           bool               dontDuplicateEdges,
           bool               saveBirthIndicesInCustomComponent) {
            std::function<bool(ConstFaceType, uint)> cppFilter =
                [&m, &faceEdgeFilter](ConstFaceType f, uint ei) -> bool {
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

template<FaceMeshConcept InMeshType>
void defAllFaceEdgeFilters(pybind11::module& m)
{
    // default OutMeshType = EdgeMesh
    defFaceEdgeFilters<InMeshType, vcl::EdgeMesh>(m);

    // typed variants for all EdgeMeshConcept output types
    auto fOutEdgeMeshes = []<EdgeMeshConcept OutMeshType>(
                              pybind11::module& m,
                              OutMeshType = OutMeshType()) {
        std::string suffix =
            "_" + camelCaseToSnakeCase(meshTypeName<OutMeshType>());
        defFaceEdgeFilters<InMeshType, OutMeshType>(m, suffix);
    };

    defForAllMeshTypes(m, fOutEdgeMeshes);
}

} // namespace detail

void initFilterAlgorithms(pybind11::module& m)
{
    auto fAllMeshes = []<MeshConcept MeshType>(
                          pybind11::module& m, MeshType = MeshType()) {
        detail::defElementMeshFilter<ElemId::VERTEX, MeshType>(
            m,
            "vertex_mesh_filter",
            "vertex_selection_mesh_filter",
            "vertex_filter");
    };

    defForAllMeshTypes(m, fAllMeshes);

    auto fFaceMeshes = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        detail::defElementMeshFilter<ElemId::FACE, MeshType>(
            m, "face_mesh_filter", "face_selection_mesh_filter", "face_filter");

        detail::defAllFaceEdgeFilters<MeshType>(m);
    };

    defForAllMeshTypes(m, fFaceMeshes);

    auto fEdgeMeshes = []<EdgeMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        detail::defElementMeshFilter<ElemId::EDGE, MeshType>(
            m, "edge_mesh_filter", "edge_selection_mesh_filter", "edge_filter");
    };

    defForAllMeshTypes(m, fEdgeMeshes);
}

} // namespace vcl::bind
