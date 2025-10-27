/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#include <vclib/bindings/core/algorithms/mesh/clean.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh.h>

namespace vcl::bind {

void initCleanAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    auto fAllMeshes = []<MeshConcept MeshType>(
                         pybind11::module& m, MeshType = MeshType()) {

        m.def(
            "remove_unreferenced_vertices",
            [](MeshType& m) -> uint {
                return removeUnreferencedVertices(m);
            },
            "mesh"_a);

        m.def(
            "remove_duplicated_vertices",
            [](MeshType& m) -> uint {
                return removeDuplicatedVertices(m);
            },
            "mesh"_a);

        m.def(
            "remove_degenerated_vertices",
            [](MeshType& m, bool deleteAlsoFaces) -> uint {
                return removeDegeneratedVertices(m, deleteAlsoFaces);
            },
            "mesh"_a,
            "delete_also_faces"_a);

    };

    defForAllMeshTypes(m, fAllMeshes);

    auto fFaceMeshes = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {

        m.def(
            "remove_duplicated_faces",
            [](MeshType& m) -> uint {
                return removeDuplicatedFaces(m);
            },
            "mesh"_a);

        m.def(
            "remove_degenerated_faces",
            [](MeshType& m) -> uint {
                return removeDegenerateFaces(m);
            },
            "mesh"_a);
    };

    defForAllMeshTypes(m, fFaceMeshes);
}

} // namespace vcl::bind
