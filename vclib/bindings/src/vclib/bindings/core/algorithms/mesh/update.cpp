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

#include <vclib/bindings/core/algorithms/mesh/update.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh.h>

#include <pybind11/stl.h>

namespace vcl::bind {

void initUpdateAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;

    auto fAllMeshes =
        []<MeshConcept MeshType>(pybind11::module& m, MeshType = MeshType()) {

            // bounding_box.h

            m.def("update_bounding_box", [](MeshType& m) {
                return vcl::updateBoundingBox(m);
            });

            // color.h

            m.def(
                "set_per_vertex_color",
                [](MeshType& m, Color c, bool onlySelected = false) {
                    return vcl::setPerVertexColor(m, c, onlySelected);
                },
                py::arg("mesh"),
                py::arg("color") = Color::White,
                py::arg("only_selected") = false);
        };

    defForAllMeshTypes(m, fAllMeshes);

    auto fFaceMeshes = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        // color.h

        m.def(
            "set_per_face_color",
            [](MeshType& m, Color c, bool onlySelected = false) {
                return vcl::setPerFaceColor(m, c, onlySelected);
            },
            py::arg("mesh"),
            py::arg("color") = Color::White,
            py::arg("only_selected") = false);

        m.def(
            "set_per_vertex_color_from_face_color",
            [](MeshType& m) {
                return vcl::setPerVertexColorFromFaceColor(m);
            });

        m.def(
            "set_per_face_color_from_vertex_color",
            [](MeshType& m) {
                return vcl::setPerFaceColorFromVertexColor(m);
            });

    };

    defForAllMeshTypes(m, fFaceMeshes);

    auto fEdgeMeshes = []<EdgeMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        // color.h

        m.def(
            "set_per_edge_color",
            [](MeshType& m, Color c, bool onlySelected = false) {
                return vcl::setPerEdgeColor(m, c, onlySelected);
            },
            py::arg("mesh"),
            py::arg("color") = Color::White,
            py::arg("only_selected") = false);

        m.def(
            "set_per_edge_color_from_vertex_color",
            [](MeshType& m) {
                return vcl::setPerEdgeColorFromVertexColor(m);
            });

    };

    defForAllMeshTypes(m, fEdgeMeshes);
}

} // namespace vcl::bind
