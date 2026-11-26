// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/algorithms/mesh/shuffle.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh.h>

#include <pybind11/stl.h>

namespace vcl::bind {

namespace detail {

template<ElemId ELEM_ID, MeshConcept MeshType>
void defElemShuffle(pybind11::module& m, const std::string& elemName)
{
    namespace py = pybind11;
    using namespace py::literals;

    m.def(
        ("fill_and_shuffle_" + elemName + "_vector").c_str(),
        [](MeshType& mesh, std::optional<uint> seed) {
            py::handle meshHandle = py::detail::get_object_handle(
                &mesh, py::detail::get_type_info(typeid(MeshType)));
            auto vec = vcl::fillAndShuffleElementVector<ELEM_ID>(
                mesh, toRConfig(seed));
            py::list res(vec.size());
            for (std::size_t i = 0; i < vec.size(); ++i) {
                py::object pyElem = py::cast(vec[i]);
                if (meshHandle) {
                    py::detail::keep_alive_impl(pyElem, meshHandle);
                }
                res[i] = std::move(pyElem);
            }
            return res;
        },
        "mesh"_a,
        "seed"_a = py::none());

    m.def(
        ("fill_and_shuffle_" + elemName + "_index_vector").c_str(),
        [](const MeshType& mesh, std::optional<uint> seed) {
            return vcl::fillAndShuffleElementIndexVector<ELEM_ID>(
                mesh, toRConfig(seed));
        },
        "mesh"_a,
        "seed"_a = py::none());
}

} // namespace detail

void initShuffleAlgorithms(pybind11::module& m)
{
    auto fAllMeshes = []<MeshConcept MeshType>(
                          pybind11::module& m, MeshType = MeshType()) {
        detail::defElemShuffle<ElemId::VERTEX, MeshType>(m, "vertex");
    };
    defForAllMeshTypes(m, fAllMeshes);

    auto fFaceMeshes = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        detail::defElemShuffle<ElemId::FACE, MeshType>(m, "face");
    };
    defForAllMeshTypes(m, fFaceMeshes);

    auto fEdgeMeshes = []<EdgeMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        detail::defElemShuffle<ElemId::EDGE, MeshType>(m, "edge");
    };
    defForAllMeshTypes(m, fEdgeMeshes);
}

} // namespace vcl::bind
