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

#ifndef VCL_BINDINGS_CORE_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
#define VCL_BINDINGS_CORE_MESH_COMPONENTS_WEDGE_TEX_COORDS_H

#include <vclib/bindings/utils.h>

#include <vclib/mesh.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

template<ElementConcept ElementType>
void initWedgeTexCoords(pybind11::class_<ElementType>& c)
{
    using WedgeTexCoordType = ElementType::WedgeTexCoordType;

    namespace py = pybind11;

    static const int N = ElementType::WEDGE_TEX_COORD_NUMBER;

    c.def(
        "wedge_tex_coord",
        [](ElementType& e, uint i) {
            return e.wedgeTexCoord(i);
        },
        py::return_value_policy::reference);
    c.def(
        "wedge_tex_coord_mod",
        [](ElementType& e, int i) {
            return e.wedgeTexCoordMod(i);
        },
        py::return_value_policy::reference);

    c.def("set_wedge_tex_coord", &ElementType::setWedgeTexCoord);

    c.def(
        "set_wedge_tex_coord_mod",
        [](ElementType& e, int i, const WedgeTexCoordType& w) {
            e.wedgeTexCoordMod(i) = w;
        });

    c.def(
        "set_wedge_tex_coords",
        [](ElementType& e, const std::vector<WedgeTexCoordType>& v) {
            e.setWedgeTexCoords(v);
        });

    c.def(
        "texture_index",
        py::overload_cast<>(&ElementType::textureIndex, py::const_));
    c.def("set_texture_index", [](ElementType& e, uint i) {
        e.textureIndex() = i;
    });

    using WedgeTexCoordsView = decltype(ElementType().wedgeTexCoords());

    if (!registeredTypes.contains(typeid(WedgeTexCoordsView))) {
        // inner class that allows to iterate over wedge tex coords
        pybind11::class_<WedgeTexCoordsView> v(c, "_WedgeTexCoordsRange");
        v.def(
            "__iter__",
            [](WedgeTexCoordsView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());
        registeredTypes.insert(typeid(WedgeTexCoordsView));
    }

    c.def(
        "wedge_tex_coords", py::overload_cast<>(&ElementType::wedgeTexCoords));
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
