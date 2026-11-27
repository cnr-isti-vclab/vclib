// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/algorithms/mesh/sort.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh.h>

#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace vcl::bind {

namespace detail {

template<ElemId ELEM_ID, MeshConcept MeshType>
void defElemSort(pybind11::module& m, const std::string& elemName)
{
    namespace py = pybind11;
    using namespace py::literals;
    using ConstElemType = typename MeshType::template ConstElementType<ELEM_ID>;

    m.def(
        ("sort_" + elemName + "_indices_by_function").c_str(),
        [](const MeshType& mesh,
           py::function    func,
           bool            getIndicesAsIfContainerCompact) {
            std::function<bool(ConstElemType, ConstElemType)> cppFunc =
                [&mesh, &func](ConstElemType a, ConstElemType b) -> bool {
                auto& mutMesh = const_cast<MeshType&>(mesh);
                return func(
                           mutMesh.template element<ELEM_ID>(a.index()),
                           mutMesh.template element<ELEM_ID>(b.index()))
                    .template cast<bool>();
            };
            return vcl::sortElemIndicesByFunction<ELEM_ID>(
                mesh, cppFunc, getIndicesAsIfContainerCompact);
        },
        "mesh"_a,
        "func"_a,
        "get_indices_as_if_container_compact"_a = false);
}

} // namespace detail

void initSortAlgorithms(pybind11::module& m)
{
    auto fAllMeshes =
        []<MeshConcept MeshType>(pybind11::module& m, MeshType = MeshType()) {
            detail::defElemSort<ElemId::VERTEX, MeshType>(m, "vertex");
        };
    defForAllMeshTypes(m, fAllMeshes);

    auto fFaceMeshes = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        detail::defElemSort<ElemId::FACE, MeshType>(m, "face");
    };
    defForAllMeshTypes(m, fFaceMeshes);

    auto fEdgeMeshes = []<EdgeMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        detail::defElemSort<ElemId::EDGE, MeshType>(m, "edge");
    };
    defForAllMeshTypes(m, fEdgeMeshes);
}

} // namespace vcl::bind
