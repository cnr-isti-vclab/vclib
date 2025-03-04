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

#include <vclib/bindings/mesh/container.h>

#include <pybind11/stl.h>

namespace vcl::bind {

void initTriMeshVertexContainer(pybind11::class_<TriMesh>& ct)
{
    namespace py = pybind11;

    initContainer<vcl::TriMesh::Vertex>(ct, "vertex", "vertices");

    ct.def("add_vertex", [](TriMesh& t, const Point3d& p) {
        return t.addVertex(p);
    });

    ct.def("add_vertices", [](TriMesh& t, const std::vector<Point3d>& v) {
        return t.addVertices(v);
    });
}

} // namespace vcl::bind
