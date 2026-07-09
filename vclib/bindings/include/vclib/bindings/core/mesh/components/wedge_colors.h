// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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

    static const int N = ElementType::WEDGE_COLOR_COUNT;

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
