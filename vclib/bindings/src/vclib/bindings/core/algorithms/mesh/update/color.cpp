// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/algorithms/mesh/update.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh/update/color.h>

#include <pybind11/stl.h>

namespace vcl::bind::detail {

void initUpdateColorAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    auto fAllMeshes =
        []<MeshConcept MeshType>(pybind11::module& m, MeshType = MeshType()) {
            m.def(
                "set_vertex_color",
                [](MeshType& m, Color c, bool onlySelected = false) {
                    return setVertexColor(m, c, onlySelected);
                },
                "mesh"_a,
                "color"_a         = Color::White,
                "only_selected"_a = false);

            m.def(
                "set_vertex_color_from_quality",
                [](MeshType& m, Color::ColorMap cm, double minQ, double maxQ) {
                    return setVertexColorFromQuality(m, cm, minQ, maxQ);
                },
                "mesh"_a,
                "color_map"_a   = Color::ColorMap::RedBlue,
                "min_quality"_a = 0,
                "max_quality"_a = 0);

            m.def(
                "set_vertex_color_from_material",
                [](MeshType& m) { return setVertexColorFromMaterial(m); },
                "mesh"_a);

            m.def(
                "set_vertex_color_perlin_noise",
                [](MeshType& m,
                   Point3d&  period,
                   Point3d&  offset,
                   bool      onlySelected) {
                    return setVertexColorPerlinNoise(
                        m, period, offset, onlySelected);
                },
                "mesh"_a,
                "period"_a,
                "offset"_a        = Point3d(0, 0, 0),
                "only_selected"_a = false);

            m.def(
                "set_vertex_perlin_color",
                [](MeshType& m,
                   double    period,
                   Point3d&  offset,
                   Color&    color1,
                   Color&    color2,
                   bool      onlySelected) {
                    return setVertexPerlinColor(
                        m, period, offset, color1, color2, onlySelected);
                },
                "mesh"_a,
                "period"_a,
                "offset"_a        = Point3d(0, 0, 0),
                "color1"_a        = Color::Black,
                "color2"_a        = Color::White,
                "only_selected"_a = false);
        };

    defForAllMeshTypes(m, fAllMeshes);

    auto fFaceMeshes = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        m.def(
            "set_face_color",
            [](MeshType& m, Color c, bool onlySelected = false) {
                return vcl::setFaceColor(m, c, onlySelected);
            },
            "mesh"_a,
            "color"_a         = Color::White,
            "only_selected"_a = false);

        m.def(
            "set_vertex_color_from_face_color",
            [](MeshType& m) { return vcl::setVertexColorFromFaceColor(m); },
            "mesh"_a);

        m.def(
            "set_face_color_from_vertex_color",
            [](MeshType& m) { return vcl::setFaceColorFromVertexColor(m); },
            "mesh"_a);

        m.def(
            "set_face_color_from_quality",
            [](MeshType& m, Color::ColorMap cm, double minQ, double maxQ) {
                return vcl::setFaceColorFromQuality(m, cm, minQ, maxQ);
            },
            "mesh"_a,
            "color_map"_a   = Color::ColorMap::RedBlue,
            "min_quality"_a = 0,
            "max_quality"_a = 0);

        m.def(
            "set_face_color_from_material",
            [](MeshType& m) { return setFaceColorFromMaterial(m); },
            "mesh"_a);

        m.def(
            "set_vertex_color_from_face_border_flag",
            [](MeshType& m,
               Color     borderColor   = Color::Blue,
               Color     internalColor = Color::White,
               Color     mixColor      = Color::Cyan) {
                return vcl::setVertexColorFromFaceBorderFlag(
                    m, borderColor, internalColor, mixColor);
            },
            "mesh"_a,
            "border_color"_a   = Color::Blue,
            "internal_color"_a = Color::White,
            "mix_color"_a      = Color::Cyan);

        m.def(
            "set_face_color_from_connected_components",
            [](MeshType&                          m,
               const std::vector<std::set<uint>>& connectedComponents) {
                return vcl::setFaceColorFromConnectedComponents(
                    m, connectedComponents);
            },
            "mesh"_a,
            "connected_components"_a);

        m.def(
            "set_face_color_from_connected_components",
            [](MeshType& m) {
                return vcl::setFaceColorFromConnectedComponents(m);
            },
            "mesh"_a);

        m.def(
            "set_face_color_scattering",
            [](MeshType& m, uint nColors = 50, bool checkFauxEdges = true) {
                return vcl::setFaceColorScattering(m, nColors, checkFauxEdges);
            },
            "mesh"_a,
            "n_colors"_a         = 50,
            "check_faux_edges"_a = true);
    };

    defForAllMeshTypes(m, fFaceMeshes);

    auto fEdgeMeshes = []<EdgeMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        m.def(
            "set_edge_color",
            [](MeshType& m, Color c, bool onlySelected = false) {
                return vcl::setEdgeColor(m, c, onlySelected);
            },
            "mesh"_a,
            "color"_a         = Color::White,
            "only_selected"_a = false);

        m.def(
            "set_edge_color_from_vertex_color",
            [](MeshType& m) { return vcl::setEdgeColorFromVertexColor(m); },
            "mesh"_a);
    };

    defForAllMeshTypes(m, fEdgeMeshes);
}

} // namespace vcl::bind::detail
