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
                py::arg("color")         = Color::White,
                py::arg("only_selected") = false);

            m.def(
                "set_per_vertex_color_from_quality",
                [](MeshType& m, Color::ColorMap cm, double minQ, double maxQ) {
                    return vcl::setPerVertexColorFromQuality(m, cm, minQ, maxQ);
                },
                py::arg("mesh"),
                py::arg("color_map")   = Color::ColorMap::RedBlue,
                py::arg("min_quality") = 0,
                py::arg("max_quality") = 0);

            m.def(
                "set_per_vertex_color_perlin_noise",
                [](MeshType& m,
                   Point3d&  period,
                   Point3d&  offset,
                   bool      onlySelected) {
                    return vcl::setPerVertexColorPerlinNoise(
                        m, period, offset, onlySelected);
                },
                py::arg("mesh"),
                py::arg("period"),
                py::arg("offset")        = Point3d(0, 0, 0),
                py::arg("only_selected") = false);

            m.def(
                "set_per_vertex_perlin_color",
                [](MeshType& m,
                   double    period,
                   Point3d&  offset,
                   Color&    color1,
                   Color&    color2,
                   bool      onlySelected) {
                    return vcl::setPerVertexPerlinColor(
                        m, period, offset, color1, color2, onlySelected);
                },
                py::arg("mesh"),
                py::arg("period"),
                py::arg("offset")        = Point3d(0, 0, 0),
                py::arg("color1")        = Color::Black,
                py::arg("color2")        = Color::White,
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
            py::arg("color")         = Color::White,
            py::arg("only_selected") = false);

        m.def("set_per_vertex_color_from_face_color", [](MeshType& m) {
            return vcl::setPerVertexColorFromFaceColor(m);
        });

        m.def("set_per_face_color_from_vertex_color", [](MeshType& m) {
            return vcl::setPerFaceColorFromVertexColor(m);
        });

        m.def(
            "set_per_face_color_from_quality",
            [](MeshType& m, Color::ColorMap cm, double minQ, double maxQ) {
                return vcl::setPerFaceColorFromQuality(m, cm, minQ, maxQ);
            },
            py::arg("mesh"),
            py::arg("color_map")   = Color::ColorMap::RedBlue,
            py::arg("min_quality") = 0,
            py::arg("max_quality") = 0);

        m.def(
            "set_per_vertex_color_from_face_border_flag",
            [](MeshType& m,
               Color     borderColor   = Color::Blue,
               Color     internalColor = Color::White,
               Color     mixColor      = Color::Cyan) {
                return vcl::setPerVertexColorFromFaceBorderFlag(
                    m, borderColor, internalColor, mixColor);
            },
            py::arg("mesh"),
            py::arg("border_color")   = Color::Blue,
            py::arg("internal_color") = Color::White,
            py::arg("mix_color")      = Color::Cyan);

        m.def(
            "set_per_face_color_from_connected_components",
            [](MeshType&                          m,
               const std::vector<std::set<uint>>& connectedComponents) {
                return vcl::setPerFaceColorFromConnectedComponents(
                    m, connectedComponents);
            },
            py::arg("mesh"),
            py::arg("connected_components"));

        m.def(
            "set_per_face_color_from_connected_components",
            [](MeshType& m) {
                return vcl::setPerFaceColorFromConnectedComponents(m);
            },
            py::arg("mesh"));

        m.def(
            "set_per_face_color_scattering",
            [](MeshType& m, uint nColors = 50, bool checkFauxEdges = true) {
                return vcl::setPerFaceColorScattering(
                    m, nColors, checkFauxEdges);
            },
            py::arg("mesh"),
            py::arg("n_colors")         = 50,
            py::arg("check_faux_edges") = true);

        // curvature.h

        py::enum_<vcl::PrincipalCurvatureAlgorithm> pcaEnum(
            m, "PrincipalCurvatureAlgorithm");
        pcaEnum.value("TAUBIN95", vcl::PrincipalCurvatureAlgorithm::TAUBIN95);
        pcaEnum.value("PCA", vcl::PrincipalCurvatureAlgorithm::PCA);
        pcaEnum.export_values();

        // TODO: add logger

        m.def(
            "update_per_vertex_principal_curvature",
            [](MeshType&                   m,
               PrincipalCurvatureAlgorithm alg =
                   PrincipalCurvatureAlgorithm::TAUBIN95) {
                return vcl::updatePrincipalCurvature(m, alg);
            },
            py::arg("mesh"),
            py::arg("algorithm") = PrincipalCurvatureAlgorithm::TAUBIN95);
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
            py::arg("color")         = Color::White,
            py::arg("only_selected") = false);

        m.def("set_per_edge_color_from_vertex_color", [](MeshType& m) {
            return vcl::setPerEdgeColorFromVertexColor(m);
        });
    };

    defForAllMeshTypes(m, fEdgeMeshes);
}

} // namespace vcl::bind
