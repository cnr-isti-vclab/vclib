// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/render/drawable/mesh/mesh_render_settings.h>
#include <vclib/render/drawable/mesh/mesh_render_settings.h>

namespace vcl::bind {

void initMeshRenderSettings(pybind11::module& m)
{
    namespace py = pybind11;

    py::class_<MeshRenderSettings>(m, "MeshRenderSettings")
        .def(py::init<>())

        .def("can_be_visible", &MeshRenderSettings::canBeVisible)
        .def("can_points", &MeshRenderSettings::canPoints, py::arg("p"))
        .def("can_surface", &MeshRenderSettings::canSurface, py::arg("s"))
        .def("can_wireframe", &MeshRenderSettings::canWireframe, py::arg("w"))
        .def("can_edges", &MeshRenderSettings::canEdges, py::arg("e"))

        .def("is_visible", &MeshRenderSettings::isVisible)
        .def("is_points", &MeshRenderSettings::isPoints, py::arg("p"))
        .def("is_surface", &MeshRenderSettings::isSurface, py::arg("s"))
        .def("is_wireframe", &MeshRenderSettings::isWireframe, py::arg("w"))
        .def("is_edges", &MeshRenderSettings::isEdges, py::arg("e"))

        .def("set_visibility", &MeshRenderSettings::setVisibility, py::arg("b"))
        .def(
            "set_points",
            [](MeshRenderSettings& self, MeshRenderInfo::Points p, bool b) {
                return self.setPoints(p, b);
            },
            py::arg("p"),
            py::arg("b") = true)
        .def(
            "set_surface",
            [](MeshRenderSettings& self, MeshRenderInfo::Surface s, bool b) {
                return self.setSurface(s, b);
            },
            py::arg("s"),
            py::arg("b") = true)
        .def(
            "set_wireframe",
            [](MeshRenderSettings& self, MeshRenderInfo::Wireframe w, bool b) {
                return self.setWireframe(w, b);
            },
            py::arg("w"),
            py::arg("b") = true)
        .def(
            "set_edges",
            [](MeshRenderSettings& self, MeshRenderInfo::Edges e, bool b) {
                return self.setEdges(e, b);
            },
            py::arg("e"),
            py::arg("b") = true)

        .def("point_width", &MeshRenderSettings::pointWidth)
        .def(
            "set_point_width",
            &MeshRenderSettings::setPointsWidth,
            py::arg("width"))

        .def(
            "set_point_user_color",
            [](MeshRenderSettings& self, float r, float g, float b, float a) {
                return self.setPointsUserColor(r, g, b, a);
            },
            py::arg("r"),
            py::arg("g"),
            py::arg("b"),
            py::arg("a") = 1.0f)
        .def(
            "set_point_user_color",
            [](MeshRenderSettings& self, const vcl::Color& c) {
                return self.setPointsUserColor(c);
            },
            py::arg("c"))
        .def("point_user_color", &MeshRenderSettings::pointUserColor)

        .def(
            "set_surface_user_color",
            [](MeshRenderSettings& self, float r, float g, float b, float a) {
                return self.setSurfaceUserColor(r, g, b, a);
            },
            py::arg("r"),
            py::arg("g"),
            py::arg("b"),
            py::arg("a") = 1.0f)
        .def(
            "set_surface_user_color",
            [](MeshRenderSettings& self, const vcl::Color& c) {
                return self.setSurfaceUserColor(c);
            },
            py::arg("c"))
        .def("surface_user_color", &MeshRenderSettings::surfaceUserColor)

        .def("wireframe_width", &MeshRenderSettings::wireframeWidth)
        .def(
            "set_wireframe_width",
            &MeshRenderSettings::setWireframeWidth,
            py::arg("width"))

        .def(
            "set_wireframe_user_color",
            [](MeshRenderSettings& self, float r, float g, float b, float a) {
                return self.setWireframeUserColor(r, g, b, a);
            },
            py::arg("r"),
            py::arg("g"),
            py::arg("b"),
            py::arg("a") = 1.0f)
        .def(
            "set_wireframe_user_color",
            [](MeshRenderSettings& self, const vcl::Color& c) {
                return self.setWireframeUserColor(c);
            },
            py::arg("c"))
        .def("wireframe_user_color", &MeshRenderSettings::wireframeUserColor)

        .def("edges_width", &MeshRenderSettings::edgesWidth)
        .def(
            "set_edges_width",
            &MeshRenderSettings::setEdgesWidth,
            py::arg("width"))

        .def(
            "set_edges_user_color",
            [](MeshRenderSettings& self, float r, float g, float b, float a) {
                return self.setEdgesUserColor(r, g, b, a);
            },
            py::arg("r"),
            py::arg("g"),
            py::arg("b"),
            py::arg("a") = 1.0f)
        .def(
            "set_edges_user_color",
            [](MeshRenderSettings& self, const vcl::Color& c) {
                return self.setEdgesUserColor(c);
            },
            py::arg("c"))
        .def("edges_user_color", &MeshRenderSettings::edgesUserColor);
}

} // namespace vcl::bind
