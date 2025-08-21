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

#ifndef VCL_BINDINGS_CORE_MESH_CONTAINERS_VERTEX_CONTAINER_H
#define VCL_BINDINGS_CORE_MESH_CONTAINERS_VERTEX_CONTAINER_H

#include "container.h"

#include <vclib/space/core.h>

#include <pybind11/stl.h>

namespace vcl::bind {

template<MeshConcept MeshType>
void initVertexContainer(pybind11::class_<MeshType>& ct)
{
    namespace py = pybind11;

    using VertexType = MeshType::VertexType;

    initContainer<VertexType>(ct, "vertex", "vertices");

    ct.def("add_vertex", [](MeshType& t, const Point3d& p) {
        return t.addVertex(p);
    });

    ct.def("add_vertex", [](MeshType& t, const py::list& v) {
        if (v.size() != 3) {
            throw std::invalid_argument(
                "Input list must have 3 elements for type vclib.Point3");
        }
        Point3d p;
        for (uint i = 0; const auto& d : v) {
            p(i++) = d.cast<double>();
        }
        return t.addVertex(p);
    });

    ct.def("add_vertices", [](MeshType& t, const std::vector<Point3d>& v) {
        return t.addVertices(v);
    });
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_CONTAINERS_VERTEX_CONTAINER_H
