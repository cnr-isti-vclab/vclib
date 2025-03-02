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

#include <vclib/bindings/meshes/tri_mesh/vertex.h>

namespace vcl::bind {

void initTriMeshVertex(pybind11::class_<vcl::TriMesh>& ct)
{
    namespace py = pybind11;

    // Create the class
    pybind11::class_<TriMesh::Vertex> c(ct, "Vertex");

    c.def("coord", py::overload_cast<>(&TriMesh::Vertex::coord, py::const_));
    c.def("set_coord", [](TriMesh::Vertex& v, const Point3d& p) {
        v.coord() = p;
    });
    c.def("normal", py::overload_cast<>(&TriMesh::Vertex::normal, py::const_));
    c.def("set_normal", [](TriMesh::Vertex& v, const Point3d& p) {
        v.normal() = p;
    });
    c.def("color", py::overload_cast<>(&TriMesh::Vertex::color, py::const_));
    c.def("set_color", [](TriMesh::Vertex& v, const Color& c) {
        v.color() = c;
    });
}

} // namespace vcl::bind
