// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/render/mesh_viewer.h>
#include <vclib/bindings/utils.h>
#include <vclib/qt/mesh_viewer.h>

namespace vcl::bind {

void initMeshViewer(pybind11::module& m)
{
    namespace py = pybind11;

    py::class_<vcl::qt::MeshViewer> c(m, "MeshViewer");

    c.def(py::init<>());

    auto bindMeshFunctions = []<MeshConcept MeshType>(
                                 pybind11::class_<vcl::qt::MeshViewer>& c,
                                 MeshType = MeshType()) {
        c.def(
            "push_mesh",
            [](vcl::qt::MeshViewer& self, const MeshType& mesh) {
                return self.pushMesh(mesh);
            },
            py::arg("mesh"));
        c.def(
            "insert_mesh",
            [](vcl::qt::MeshViewer& self, uint pos, const MeshType& mesh) {
                return self.insertMesh(pos, mesh);
            },
            py::arg("pos"),
            py::arg("mesh"));
    };
    defForAllMeshTypes(c, bindMeshFunctions);
    c.def("remove_mesh", &vcl::qt::MeshViewer::removeMesh, py::arg("id"));
    c.def("update_mesh", &vcl::qt::MeshViewer::updateMesh, py::arg("id"));
    c.def("clear_meshes", &vcl::qt::MeshViewer::clearMeshes);
    c.def("selected_mesh", &vcl::qt::MeshViewer::selectedMesh);
    c.def("update_gui", &vcl::qt::MeshViewer::updateGUI);
    c.def("fit_scene", &vcl::qt::MeshViewer::fitScene);
    c.def("fit_view", &vcl::qt::MeshViewer::fitView);
    c.def("show", [](vcl::qt::MeshViewer& self) {
        self.show();
    });
}

} // namespace vcl::bind
