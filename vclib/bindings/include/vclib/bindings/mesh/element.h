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

#ifndef VCL_BINDINGS_MESH_ELEMENT_H
#define VCL_BINDINGS_MESH_ELEMENT_H

#include <vclib/concepts/mesh.h>
#include <vclib/space/core.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

template<ElementConcept ElementType>
void initElement(pybind11::class_<ElementType>& c)
{
    namespace py = pybind11;

    c.def("index", &ElementType::index);

    c.def("parent_mesh", [](ElementType& v) {
        return v.parentMesh();
    });

    if constexpr (comp::HasCoordinate<ElementType>) {
        c.def("coord", py::overload_cast<>(&ElementType::coord, py::const_));
        c.def("set_coord", [](ElementType& v, const Point3d& p) {
            v.coord() = p;
        });
    }
    if constexpr (comp::HasNormal<ElementType>) {
        c.def("normal", py::overload_cast<>(&ElementType::normal, py::const_));
        c.def("set_normal", [](ElementType& v, const Point3d& p) {
            v.normal() = p;
        });
    }
    if constexpr (comp::HasColor<ElementType>) {
        c.def("color", py::overload_cast<>(&ElementType::color, py::const_));
        c.def("set_color", [](ElementType& v, const Color& c) {
            v.color() = c;
        });
    }
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_MESH_ELEMENT_H
