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

#ifndef VCL_BINDINGS_MESH_COMPONENTS_VERTEX_REFERENCES_H
#define VCL_BINDINGS_MESH_COMPONENTS_VERTEX_REFERENCES_H

#include <vclib/concepts/mesh.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

template<ElementConcept ElementType>
void initVertexReferences(pybind11::class_<ElementType>& c)
{
    using VertexType = ElementType::VertexType;

    namespace py = pybind11;

    c.def("vertex_number", &ElementType::vertexNumber);

    c.def("vertex", [](ElementType& e, uint i) {
        return e.vertex(i);
    });
    c.def("vertex_mod", [](ElementType& e, int i) {
        return e.vertexMod(i);
    });
    c.def("set_vertex", [](ElementType& e, uint i, VertexType* v) {
        e.setVertex(i, v);
    });
    c.def("set_vertex", [](ElementType& e, uint i, uint vi) {
        e.setVertex(i, vi);
    });
    c.def("set_vertex_mod", [](ElementType& e, int i, VertexType* v) {
        e.setVertexMod(i, v);
    });
    c.def("set_vertex_mod", [](ElementType& e, int i, uint vi) {
        e.setVertexMod(i, vi);
    });
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_MESH_COMPONENTS_VERTEX_REFERENCES_H
