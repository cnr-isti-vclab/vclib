// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/render/drawable/abstract_mesh.h>
#include <vclib/render/drawable/abstract_drawable_mesh.h>
#include <vclib/render/drawable/mesh/mesh_render_info.h>

namespace vcl::bind {

void initAbstractDrawableMesh(pybind11::module& m)
{
    namespace py = pybind11;

    py::class_<vcl::AbstractDrawableMesh,
               vcl::DrawableObject,
               std::shared_ptr<vcl::AbstractDrawableMesh>>
        c(m, "AbstractDrawableMesh");

    c.def(
        "update_buffers",
        [](vcl::AbstractDrawableMesh& self) {
            self.updateBuffers();
        },
        "Updates the mesh buffers using the default buffers mask.");

    c.def(
        "update_buffers",
        [](vcl::AbstractDrawableMesh& self/*, uint16_t buffersToUpdate*/) {
            self.updateBuffers(
                /*vcl::MeshRenderInfo::BuffersBitSet(buffersToUpdate)*/);
        },
        "Updates the mesh buffers."
        /*py::arg("buffers_to_update")*/);

    c.def("vertex_count", &vcl::AbstractDrawableMesh::vertexCount);
    c.def("face_count", &vcl::AbstractDrawableMesh::faceCount);
    c.def("edge_count", &vcl::AbstractDrawableMesh::edgeCount);
    c.def("model_matrix", &vcl::AbstractDrawableMesh::modelMatrix);
}

} // namespace vcl::bind
