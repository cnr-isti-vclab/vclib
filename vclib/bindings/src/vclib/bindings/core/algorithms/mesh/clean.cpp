// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/algorithms/mesh/clean.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh.h>

namespace vcl::bind {

void initCleanAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    auto fAllMeshes =
        []<MeshConcept MeshType>(pybind11::module& m, MeshType = MeshType()) {
            m.def(
                "remove_unreferenced_vertices",
                [](MeshType& m) -> uint {
                    return removeUnreferencedVertices(m);
                },
                "mesh"_a);

            m.def(
                "remove_duplicate_vertices",
                [](MeshType& m) -> uint {
                    return removeDuplicateVertices(m);
                },
                "mesh"_a);

            m.def(
                "remove_degenerate_vertices",
                [](MeshType& m, bool deleteAlsoFaces) -> uint {
                    return removeDegenerateVertices(m, deleteAlsoFaces);
                },
                "mesh"_a,
                "delete_also_faces"_a);
        };

    defForAllMeshTypes(m, fAllMeshes);

    auto fFaceMeshes = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        m.def(
            "remove_duplicate_faces",
            [](MeshType& m) -> uint {
                return removeDuplicateFaces(m);
            },
            "mesh"_a);

        m.def(
            "remove_degenerate_faces",
            [](MeshType& m) -> uint {
                return removeDegenerateFaces(m);
            },
            "mesh"_a);
    };

    defForAllMeshTypes(m, fFaceMeshes);
}

} // namespace vcl::bind
