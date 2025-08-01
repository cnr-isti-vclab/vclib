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

#ifndef VCL_BINDINGS_CORE_MESH_COMPONENTS_WEDGE_COLORS_H
#define VCL_BINDINGS_CORE_MESH_COMPONENTS_WEDGE_COLORS_H

#include <vclib/bindings/utils.h>

#include <vclib/mesh.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

template<ElementConcept ElementType>
void initWedgeColors(pybind11::class_<ElementType>& c)
{
    using WedgeColorType = ElementType::WedgeColorType;

    namespace py = pybind11;

    static const int N = ElementType::WEDGE_COLOR_NUMBER;

    c.def(
        "wedge_color",
        [](ElementType& e, uint i) {
            return e.wedgeColor(i);
        },
        py::return_value_policy::reference);
    c.def(
        "wedge_color_mod",
        [](ElementType& e, int i) {
            return e.wedgeColorMod(i);
        },
        py::return_value_policy::reference);

    c.def("set_wedge_color", &ElementType::setWedgeColor);

    c.def(
        "set_wedge_color_mod",
        [](ElementType& e, int i, const WedgeColorType& w) {
            e.wedgeColorMod(i) = w;
        });

    c.def(
        "set_wedge_colors",
        [](ElementType& e, const std::vector<WedgeColorType>& v) {
            e.setWedgeColors(v);
        });

    using WedgeColorsView = decltype(ElementType().wedgeColors());

    if (!registeredTypes.contains(typeid(WedgeColorsView))) {
        // inner class that allows to iterate over wedge tex coords
        pybind11::class_<WedgeColorsView> v(c, "_WedgeColorsRange");
        v.def(
            "__iter__",
            [](WedgeColorsView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());
        registeredTypes.insert(typeid(WedgeColorsView));
    }

    c.def("wedge_colors", py::overload_cast<>(&ElementType::wedgeColors));
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_COMPONENTS_WEDGE_COLORS_H
