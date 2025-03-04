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

#ifndef VCL_BINDINGS_MESH_COMPONENTS_H
#define VCL_BINDINGS_MESH_COMPONENTS_H

#include "components/bit_flags.h"
#include "components/polygon_bit_flags.h"
#include "components/triangle_bit_flags.h"
#include "components/vertex_references.h"

#include <vclib/concepts/mesh.h>
#include <vclib/space/core.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

template<ElementOrMeshConcept ElementType>
void initComponents(pybind11::class_<ElementType>& c)
{
    namespace py = pybind11;
    using enum py::return_value_policy;

    if constexpr (!MeshConcept<ElementType>) {
        c.def("index", &ElementType::index);

        c.def("parent_mesh", [](ElementType& v) {
            return v.parentMesh();
        });
    }

    // bit flags
    if constexpr (comp::HasTriangleBitFlags<ElementType>) {
        initTriangleBitFlags(c);
    }
    else if constexpr (comp::HasPolygonBitFlags<ElementType>) {
        initPolygonBitFlags(c);
    }
    else if constexpr (comp::HasBitFlags<ElementType>){
        initBitFlags(c);
    }

    if constexpr (comp::HasBoundingBox<ElementType>) {
        c.def(
            "bounding_box",
            py::overload_cast<>(&ElementType::boundingBox),
            reference);
        c.def("set_bounding_box", [](ElementType& v, const Box3d& b) {
            v.boundingBox() = b;
        });
    }
    if constexpr (comp::HasColor<ElementType>) {
        c.def("color", py::overload_cast<>(&ElementType::color), reference);
        c.def("set_color", [](ElementType& v, const Color& c) {
            v.color() = c;
        });
    }
    if constexpr (comp::HasCoordinate<ElementType>) {
        c.def("coord", py::overload_cast<>(&ElementType::coord), reference);
        c.def("set_coord", [](ElementType& v, const Point3d& p) {
            v.coord() = p;
        });
    }
    if constexpr (comp::HasName<ElementType>) {
        c.def("name", py::overload_cast<>(&ElementType::name));
        c.def("set_name", [](ElementType& v, const std::string& n) {
            v.name() = n;
        });
    }
    if constexpr (comp::HasNormal<ElementType>) {
        c.def("normal", py::overload_cast<>(&ElementType::normal), reference);
        c.def("set_normal", [](ElementType& v, const Point3d& p) {
            v.normal() = p;
        });
    }
    if constexpr (comp::HasPrincipalCurvature<ElementType>) {
        c.def(
            "principal_curvature",
            py::overload_cast<>(&ElementType::principalCurvature),
            reference);
        c.def(
            "set_principal_curvature",
            [](ElementType& v, const PrincipalCurvatured& p) {
                v.principalCurvature() = p;
            });
    }

    if constexpr (comp::HasQuality<ElementType>) {
        c.def("quality", py::overload_cast<>(&ElementType::quality));
        c.def("set_quality", [](ElementType& v, double q) {
            v.quality() = q;
        });
    }
    if constexpr (comp::HasTexCoord<ElementType>) {
        c.def(
            "tex_coord",
            py::overload_cast<>(&ElementType::texCoord),
            reference);
        c.def("set_tex_coord", [](ElementType& v, const TexCoordIndexedd& t) {
            v.texCoord() = t;
        });
    }

    if constexpr (comp::HasVertexReferences<ElementType>) {
        initVertexReferences(c);
    }
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_MESH_COMPONENTS_H
