// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/render/mesh_viewer.h>
#include <vclib/bindings/utils.h>
#include <vclib/qt/mesh_viewer.h>
#include <vclib/render/drawable/drawable_mesh.h>

namespace vcl::bind {

void initMeshViewer(pybind11::module& m)
{
    namespace py = pybind11;

    py::class_<qt::MeshViewer> c(m, "MeshViewer");

    c.def(py::init<>());

    auto bindMeshFunctions = []<MeshConcept MeshType>(
                                 auto& c,
                                 MeshType = MeshType()) {
        c.def(
            "push_drawable_object",
            [](qt::MeshViewer& self, std::shared_ptr<DrawableMesh<MeshType>> mesh) {
                return self.pushDrawableObject(std::move(mesh));
            },
            py::arg("obj"));
        c.def(
            "insert_drawable_object",
            [](qt::MeshViewer& self,
               uint            pos,
               std::shared_ptr<DrawableMesh<MeshType>> mesh) {
                return self.insertDrawableObject(pos, std::move(mesh));
            },
            py::arg("pos"),
            py::arg("obj"));
    };
    defForAllMeshTypes(c, bindMeshFunctions);
    c.def(
        "remove_drawable_object",
        &qt::MeshViewer::removeDrawableObject,
        py::arg("id"));
    c.def("clear_drawable_objects", &qt::MeshViewer::clearDrawableObjects);
    c.def("selected_drawable_object", &qt::MeshViewer::selectedDrawableObject);

    c.def(
        "drawable_object",
        py::overload_cast<uint>(&qt::MeshViewer::drawableObject),
        py::arg("i"));
    c.def("drawable_objects_count", &qt::MeshViewer::drawableObjectsCount);

    c.def("__len__", &qt::MeshViewer::drawableObjectsCount);
    c.def("__getitem__", [](qt::MeshViewer& self, int i) {
        if (i < 0) {
            i += self.drawableObjectsCount();
        }
        if (i < 0 || i >= (int) self.drawableObjectsCount()) {
            throw py::index_error();
        }
        return self.drawableObject(i);
    });

    c.def("update_gui", &qt::MeshViewer::updateGUI);
    c.def("fit_scene", &qt::MeshViewer::fitScene);
    c.def("fit_view", &qt::MeshViewer::fitView);
    c.def("show", &qt::MeshViewer::show);
    c.def("show_maximized", &qt::MeshViewer::showMaximized);
}

} // namespace vcl::bind
