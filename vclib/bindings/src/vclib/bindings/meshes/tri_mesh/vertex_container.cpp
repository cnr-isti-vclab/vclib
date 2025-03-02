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

#include <vclib/bindings/meshes/tri_mesh/vertex_container.h>

namespace vcl::bind {

class VertexRange
{
public:
    TriMesh& t;

    VertexRange(TriMesh& t) : t(t) {}
};

void initTriMeshVertexContainer(pybind11::class_<TriMesh>& ct)
{
    namespace py = pybind11;

    ct.def("vertex", [](TriMesh& t, uint i) -> TriMesh::Vertex& {
        return t.vertex(i);
    }, py::return_value_policy::reference);
    ct.def("vertex_number", &TriMesh::vertexNumber);
    ct.def("vertex_container_size", &TriMesh::vertexContainerSize);
    ct.def("deleted_vertex_number", &TriMesh::deletedVertexNumber);
    ct.def("add_vertex", [](TriMesh& t) {
        t.addVertex();
    });
    ct.def("add_vertex", [](TriMesh& t, const Point3d& p) {
        t.addVertex(p);
    });
    ct.def("add_vertices", [](TriMesh& t, uint n) {
        t.addVertices(n);
    });
    ct.def("clear_vertices", &TriMesh::clearVertices);
    ct.def("resize_vertices", &TriMesh::resizeVertices);
    ct.def("reserve_vertices", &TriMesh::reserveVertices);
    ct.def("compact_vertices", &TriMesh::compactVertices);
    ct.def("delete_vertex", [](TriMesh& t, uint i) {
        t.deleteVertex(i);
    });

    // inner class that allows to iterate over vertices
    pybind11::class_<VertexRange> v(ct, "_VertexRange");

    v.def(
        "__iter__",
        [](VertexRange& t) {
            return py::make_iterator(t.t.vertexBegin(), t.t.vertexEnd());
        },
        py::keep_alive<
            0,
            1>() /* Essential: keep object alive while iterator exists */);

    ct.def("vertices", [](TriMesh& t) {
        return VertexRange(t);
    });

    // optional components

    ct.def("is_per_vertex_color_enabled", &TriMesh::isPerVertexColorEnabled);
    ct.def("enable_per_vertex_color", &TriMesh::enablePerVertexColor);
    ct.def("disable_per_vertex_color", &TriMesh::disablePerVertexColor);
}

} // namespace vcl::bind
