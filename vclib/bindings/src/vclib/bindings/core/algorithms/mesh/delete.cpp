// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/algorithms/mesh/delete.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh.h>

#include <pybind11/eigen.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace vcl::bind {

namespace detail {

template<ElemId ELEM_ID, MeshConcept MeshType>
void defDeleteElementIf(
    pybind11::module&  m,
    const std::string& name,
    const std::string& filterArgName)
{
    using namespace pybind11::literals;
    using ConstElemType = typename MeshType::template ConstElementType<ELEM_ID>;

    m.def(
        name.c_str(),
        [](MeshType& m, pybind11::function elemFilter) -> uint {
            std::function<bool(ConstElemType)> cppFilter =
                [&m, &elemFilter](ConstElemType e) -> bool {
                return elemFilter(m.template element<ELEM_ID>(e.index()))
                    .template cast<bool>();
            };
            return vcl::deleteElementsIf<ELEM_ID>(m, cppFilter);
        },
        "mesh"_a,
        pybind11::arg(filterArgName.c_str()));

    m.def(
        name.c_str(),
        [](MeshType& m, const Eigen::VectorXi& elemFilter) -> uint {
            return vcl::deleteElementsIf<ELEM_ID>(m, elemFilter);
        },
        "mesh"_a,
        pybind11::arg(filterArgName.c_str()));

    m.def(
        name.c_str(),
        [](MeshType& m, const std::vector<int>& elemFilter) -> uint {
            return vcl::deleteElementsIf<ELEM_ID>(m, elemFilter);
        },
        "mesh"_a,
        pybind11::arg(filterArgName.c_str()));
}

} // namespace detail

void initDeleteAlgorithms(pybind11::module& m)
{
    auto fAllMeshes =
        []<MeshConcept MeshType>(pybind11::module& m, MeshType = MeshType()) {
            detail::defDeleteElementIf<ElemId::VERTEX, MeshType>(
                m, "delete_vertices_if", "vertex_filter");
        };

    defForAllMeshTypes(m, fAllMeshes);

    auto fFaceMeshes = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        detail::defDeleteElementIf<ElemId::FACE, MeshType>(
            m, "delete_faces_if", "face_filter");
    };

    defForAllMeshTypes(m, fFaceMeshes);

    auto fEdgeMeshes = []<EdgeMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        detail::defDeleteElementIf<ElemId::EDGE, MeshType>(
            m, "delete_edges_if", "edge_filter");
    };

    defForAllMeshTypes(m, fEdgeMeshes);
}

} // namespace vcl::bind
